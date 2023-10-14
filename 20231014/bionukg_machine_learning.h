
#pragma once

#ifndef _bionukg_machine_learning_h
#define _bionukg_machine_learning_h

#include <vector>
#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg
#include"./bionukg_concurrent_compute.h"
    // 聚类算法
    class Cluster
    {
    public:
        Cluster(){};
        ~Cluster(){};
        // K-Means, 变量group_num表示聚类的个数，max_iter表示最大迭代次数
        template <typename pointsT> // pointsT需要重载+=，/=，==，!=，distance，label
        static void KMeans(std::vector<pointsT> &points, size_t group_num, size_t max_iter = 1024)
        {
            size_t points_num = points.size();
            if (points_num < group_num)
                return;
            // 初始化聚类中心
            std::vector<pointsT> centers;
            for (size_t i = 0; i < group_num; ++i)
            {
                centers.push_back(points[i]);
            }
            // 迭代
            // pointsT.label 是一个int类型的变量，用来表示该点属于哪个类
            bool is_changed = true;
            while (is_changed && max_iter-- > 0)
            {
                is_changed = false;
                // 计算每个点到聚类中心的距离
                for (size_t i = 0; i < points_num; ++i)
                {
                    double min_distance = points[i].distance(centers[0]);
                    points[i].label = 0;
                    for (size_t j = 1; j < group_num; ++j)
                    {
                        double distance = points[i].distance(centers[j]);
                        if (distance < min_distance)
                        {
                            min_distance = distance;
                            points[i].label = j;
                        }
                    }
                }
                // 计算新的聚类中心
                std::vector<pointsT> new_centers;
                for (size_t i = 0; i < group_num; ++i)
                {
                    pointsT new_center;
                    size_t count = 0;
                    for (size_t j = 0; j < points_num; ++j)
                    {
                        if (points[j].label == i)
                        {
                            new_center += points[j];
                            ++count;
                        }
                    }
                    new_center /= count;
                    new_centers.push_back(new_center);
                }
                // 判断是否需要继续迭代
                for (size_t i = 0; i < group_num; ++i)
                {
                    if (new_centers[i] != centers[i])
                    {
                        is_changed = true;
                        break;
                    }
                }
                // 更新聚类中心
                if (is_changed)
                {
                    centers = new_centers;
                }
            }
        }
        // 给kmeans聚类结果上色，label视为24bit的颜色值
        template <typename pointsT>
        static void KMeans_dye(std::vector<pointsT> &points)
        {
            size_t points_num = points.size();
            // 使用颜色空间距离尽量大的颜色
            const uint32_t colors[] = {
                0xff0000,
                0x00ff00,
                0x0000ff,
                0xffff00,
                0xff00ff,
                0x00ffff,
                0x800000,
                0x008000,
                0x000080,
                0x808000,
                0x800080,
                0x008080,
                0x0080ff,
                0x8000ff,
                0x00ff80,
                0x80ff00,
                0xff0080,
                0xff8000,
            };
            const size_t colors_num = sizeof(colors) / sizeof(colors[0]);

            for (size_t i = 0; i < points_num; ++i)
            {
                points[i].label = colors[points[i].label % colors_num];
            }
        }
        template <typename pointsT>
        static void KMeans_and_dye(std::vector<pointsT> &points, size_t group_num, size_t max_iter = 1024)
        {
            KMeans(points, group_num, max_iter);
            KMeans_dye(points);
            return;
        }

        // DBSCAN, 变量eps表示邻域半径，min_pts表示邻域内最少的点的个数
        template <typename pointsT>
        static void DBSCAN(std::vector<pointsT> &points, double eps, size_t min_pts)
        {

            size_t points_num = points.size();

            // create the neighbors vector
            std::vector<std::vector<size_t>> neighbors(points_num);
            // 计算每个点的邻域
            for (size_t i = 0; i < points_num; ++i)
            {
                for (size_t j = 0; j < i; ++j)
                {
                    // if (points[i].distance(points[j]) <= eps)
                    // use nearer instead of distance, which is faster
                    if (points[i].nearer(points[j], eps))
                    {
                        neighbors[i].push_back(j);
                        neighbors[j].push_back(i);
                    }
                }
            }
            // 初始化所有点的label为0，表示未分类
            for (size_t i = 0; i < points_num; ++i)
            {
                points[i].label = 0;
            }
            // 标记核心点, label = 1表示核心点，
            for (size_t i = 0; i < points_num; ++i)
            {
                if (neighbors[i].size() >= min_pts)
                {
                    points[i].label = 1;
                }
            }
            // 标记边界点和噪声点，label = 2表示边界点，label = 3表示噪声点
            for (size_t i = 0; i < points_num; ++i)
            {
                if (points[i].label == 0)
                {
                    for (size_t j = 0; j < neighbors[i].size(); ++j)
                    {
                        if (points[neighbors[i][j]].label == 1)
                        {
                            points[i].label = 2;
                            break;
                        }
                    }
                    if (points[i].label == 0)
                    {
                        points[i].label = 3;
                    }
                }
            }
            // 标记聚类，label高于2bit的部分用于表示聚类，低2bit不变
            int cluster = 0;
            for (size_t i = 0; i < points_num; ++i)
            {
                if (points[i].label == 1) // 如果是核心点
                {
                    ++cluster;
                    points[i].label = cluster << 2;
                    std::vector<size_t> seeds;
                    seeds.push_back(i);                       // 将该点加入种子集合
                    for (size_t j = 0; j < seeds.size(); ++j) // 对种子集合中的每个点
                    {
                        for (size_t k = 0; k < neighbors[seeds[j]].size(); ++k) // 对该点的邻域中的每个点
                        {
                            size_t index = neighbors[seeds[j]][k];
                            if (points[index].label == 1)
                            {
                                seeds.push_back(index);
                            }
                            points[index].label |= cluster << 2;
                        }
                    }
                }
            }
        }
        // 给DBSCAN聚类结果上色，label视为24bit的颜色值
        template <typename pointsT>
        static void DBSCAN_dye(std::vector<pointsT> &points)
        {
            size_t points_num = points.size();
            // 使用颜色空间距离尽量大的颜色
            const uint32_t colors[] = {
                0xff0000,
                0x00ff00,
                0x0000ff,
                0xffff00,
                0xff00ff,
                0x00ffff,
            };
            const size_t colors_num = sizeof(colors) / sizeof(colors[0]);

            for (size_t i = 0; i < points_num; ++i)
            {
                // 噪声点label为3，上色为灰色0x808080
                if (points[i].label == 3)
                {
                    points[i].label = 0x808080;
                } // 核心点label低位为0b01，原样上色
                else if (points[i].label & 0x1)
                {
                    points[i].label = colors[(points[i].label >> 2) % colors_num];
                } // 边界点label低位为0b10，颜色是核心点加入灰色
                else if (points[i].label & 0x2)
                {
                    uint32_t core_color = colors[(points[i].label >> 2) % colors_num];
                    uint32_t edge_color = (core_color & 0xfefefe) >> 1;
                    points[i].label = edge_color;
                }
            }
        }
        template <typename pointsT>
        static void DBSCAN_and_dye(std::vector<pointsT> &points, double eps, size_t min_pts)
        {
            DBSCAN(points, eps, min_pts);
            DBSCAN_dye(points);
            return;
        }
#if 1// bionukg_concurrent_compute_h

        template <typename pointsT>
        static void GPU_KMeans(std::vector<pointsT>& points, size_t group_num = 3, size_t max_iter = 1024)
        {
            hlsl_code code;
            code.code_kmeans(group_num);
            compute_GPU cg;

            cg.set_code_segment(code);
            struct kmeans_data
            {
                float x;
                float y;
            };
            //copy to GPU
            size_t count = points.size();
            if(count>=65536)
                throw "points too many, must less than 65536";

            kmeans_data* data = new kmeans_data[count * 4];
            uint32_t* labels = new uint32_t[count * 4];
            for (size_t i = 0; i < count; i++)
            {
                data[i].x = points[i].x;
                data[i].y = points[i].y;
                labels[i] = points[i].label;
            }
            //set points
            cg.set_input_data_segment_u32(data, count * sizeof(kmeans_data) / sizeof(uint32_t));
            //set labels
            cg.set_output_data_segment_u32(labels, count * sizeof(uint32_t) / sizeof(uint32_t) * 4);
            //set centroids

            kmeans_data* centroids = new kmeans_data[group_num*4];
            for (size_t i0 = 0; i0 < group_num; i0++)
            {
                int i = rand() % count;
                centroids[i0].x = points[i].x;
                centroids[i0].y = points[i].y;
            }
            cg.set_output_data_segment_u32(centroids, group_num * sizeof(kmeans_data) / sizeof(uint32_t)*4);
            cg.load_data();
            cg.run(count);
            cg.unload_data();
            cg.get_data(labels, count * sizeof(uint32_t) / sizeof(uint32_t), 0);

            //copy labels to points
            for (size_t i = 0; i < count; i++)
            {
                points[i].label = labels[i];
            }

            //free memory
            delete[] data;
            delete[] centroids;
            delete[] labels;
            return;


        }

        template <typename pointsT>
        static void GPU_KMeanstest(std::vector<pointsT> &points, size_t group_num = 3, size_t max_iter = 1024)
        {
            hlsl_code code;
            code.code_kmeanstest();
            compute_GPU cg;

            cg.set_code_segment(code);
            struct kmeans_data
            {
                float x;
                float y;
            };
            //copy to GPU
            size_t count = points.size();
            kmeans_data* data = new kmeans_data[count*4];
            uint32_t* labels = new uint32_t[count*4];
            for (size_t i = 0; i < count; i++)
            {
                data[i].x = points[i].x;
                data[i].y = points[i].y;
                labels[i] = points[i].label;
            }
            //set points
            cg.set_input_data_segment_u32(data, count * sizeof(kmeans_data) / sizeof(uint32_t));
            //set labels
            cg.set_output_data_segment_u32(labels, count * sizeof(uint32_t) / sizeof(uint32_t)*4);
            //set centroids

            kmeans_data* centroids = new kmeans_data[group_num];
            for (size_t i0 = 0; i0 < group_num; i0++)
            {
                int i = rand() % count;
                centroids[i0].x = points[i].x;
                centroids[i0].y = points[i].y;
            }
            cg.set_input_data_segment_u32(centroids, group_num * sizeof(kmeans_data) / sizeof(uint32_t));
            cg.load_data();
            cg.run(count);
            cg.unload_data();
            cg.get_data(labels, count * sizeof(uint32_t)/ sizeof(uint32_t) , 0);

            //copy labels to points
            for (size_t i = 0; i < count; i++)
            {
                points[i].label = labels[i];
            }

            //free memory
            delete[] data;
            delete[] centroids;
            delete[] labels;
            return;


        }


#endif
    };

#ifdef _bionukg_graphics_h
    class pixel_datapoint
    {
    public:
        float r;
        float g;
        float b;
        float a;
        double x;
        double y;
        float rate = 1.0f;
        uint32_t label;

        pixel_datapoint()
        {
            r = 0;
            g = 0;
            b = 0;
            a = 0;
            x = 0;
            y = 0;
            label = 0;
        }

        pixel_datapoint(uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint32_t x, uint32_t y, float xyWeight = 1.0f, uint32_t label = 0)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
            this->x = x;
            this->y = y;
            this->rate = xyWeight;
            this->label = label;
        }
        double distance(pixel_datapoint &other)
        {
            // 包含颜色和坐标的距离
            double distance = 0, r2, g2, b2, a2, x2, y2;
            double rgba2, xy2;
            r2 = this->r - other.r;
            g2 = this->g - other.g;
            b2 = this->b - other.b;
            a2 = this->a - other.a;
            x2 = this->x - other.x;
            y2 = this->y - other.y;

            rgba2 = r2 * r2 + g2 * g2 + b2 * b2 + a2 * a2;
            xy2 = x2 * x2 + y2 * y2;

            distance = (xy2 * rate + rgba2);
            return sqrt(distance);
        }
        bool nearer(pixel_datapoint &other, double eps)
        {
            // 包含颜色和坐标的距离
            double distance = 0, r2, g2, b2, a2, x2, y2;
            x2 = this->x - other.x;
            y2 = this->y - other.y;
            distance = x2 * x2 + y2 * y2;
            distance *= rate;
            if (distance > eps * eps)
                return false;
            r2 = this->r - other.r;
            g2 = this->g - other.g;
            b2 = this->b - other.b;
            a2 = this->a - other.a;
            distance += r2 * r2 + g2 * g2 + b2 * b2 + a2 * a2;
            return distance <= eps * eps;
        }
        // 运算符重载
        pixel_datapoint &operator+=(const pixel_datapoint &other)
        {
            this->r += other.r;
            this->g += other.g;
            this->b += other.b;
            this->a += other.a;
            this->x += other.x;
            this->y += other.y;
            return *this;
        }
        pixel_datapoint &operator/=(const size_t &num)
        {
            this->r /= num;
            this->g /= num;
            this->b /= num;
            this->a /= num;
            this->x /= num;
            this->y /= num;
            return *this;
        }
        bool operator==(const pixel_datapoint &other)
        {
            return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a && this->x == other.x && this->y == other.y;
        }
        bool operator!=(const pixel_datapoint &other)
        {
            return this->r != other.r || this->g != other.g || this->b != other.b || this->a != other.a || this->x != other.x || this->y != other.y;
        }
    };

    class pixels : public std::vector<pixel_datapoint>
    {
    public:
        uint32_t width;
        uint32_t height;
        pixels(bmp *pic, float xyWeight = 1.0f)
        {
            uint8_t rgbx[4] = {0};
            for (uint32_t i = 0; i < pic->height; i++)
            {
                for (uint32_t j = 0; j < pic->width; j++)
                {
                    pic->pointread(j, i, rgbx);
                    this->push_back(pixel_datapoint(rgbx[0], rgbx[1], rgbx[2], rgbx[3], j, i, xyWeight));
                }
            }
            width = pic->width;
            height = pic->height;
        }
        void save_as_labeled_color(const char *path)
        {
            bmp pic(bmphd(width, height, 24));
            uint8_t rgbx[4];
            for (uint32_t i = 0; i < height; i++)
            {
                for (uint32_t j = 0; j < width; j++)
                {
                    auto label = this->at((size_t)i * width + j).label;
                    rgbx[0] = label & 0xff;
                    rgbx[1] = (label >> 8) & 0xff;
                    rgbx[2] = (label >> 16) & 0xff;
                    rgbx[3] = 0;
                    pic.pointwrite(j, i, rgbx);
                }
            }
            pic.save_as(path);
        }
    };

#endif // _bionukg_graphics_h

#ifdef namespace_bionukg
};
#endif // namespace_bionukg
#endif //_bionukg_machine_learning_h