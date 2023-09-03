#pragma once

#ifndef _bionukg_phys_eng_h
#define _bionukg_phys_eng_h
#include "./bionukg_datastruct.h"
#include <stdint.h>
#ifdef namespace_bionukg
namespace bionukg
{
#ifdef namespace_phy
    namespace phy
    {
#endif // namespace_phy
#endif // namespace_bionukg
        // 有些是我教newbing写的。注释越少，我写的占比越大

        template <int N, typename dataType>
        class Particle; // 质点，具有坐标、速度、质量

        template <int N = 2, typename dataType = double> // 相互作用，作用于两个质点之间，或者一个粒子与场之间。
        class interaction_between
        {
        public:
            Particle<N, dataType> *p1;
            Particle<N, dataType> *p2;

        public:
            /// <summary>
            /// 必备，返回这个东西对粒子的作用力
            /// </summary>
            /// <param name="thisp">传入粒子本身的指针，以便判断力的方向</param>
            /// <returns>作用力矢量</returns>
            inline virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const { return vector_<N, dataType>(); };
            /// <summary>
            /// 能量守恒必备，返回这个系统存储（或耗散）的能量
            /// </summary>
            /// <returns>当前系统能量</returns>
            inline virtual dataType energy() const { return dataType(); };
            /// <summary>
            /// 可选，用于解除它对质点的连接，如果从不解除连接就不需要
            /// </summary>
            inline virtual void detach()
            {
                this->p1 = 0;
                this->p2 = 0;
                return;
            };
            /// <summary>
            /// 立刻清除其内容，可能造成内存泄露，只用于迅速析构质点系
            /// </summary>
            void imm_clear()
            {
                this->p1 = 0;
                this->p2 = 0;
            }
            /// <summary>
            /// 返回其颜色（灰度），用于绘制
            /// </summary>
            inline virtual uint8_t gray_color() const { return 0; };
            inline virtual bool visible() const { return true; };
            inline Particle<N, dataType> *another_particle(Particle<N, dataType> *thisp) const
            {
                if (p1 == thisp)
                    return p2;
                return p1;
            }
            interaction_between()
            {
                this->p1 = 0;
                this->p2 = 0;
            };
        };

        // 弹簧
        template <int N = 2, typename dataType = double>
        class spring_ : public interaction_between<N, dataType>
        {
        private:
            dataType k;
            dataType original_length;

        public:
            // F=-k*dx
            virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const
            {
                vector_<N, dataType> dx = (thisp == this->p1) ? (this->p1->get_position() - this->p2->get_position()) : (this->p2->get_position() - this->p1->get_position());
                dataType modulus = dx.modulus();
                return dx * (dataType(-1) * this->k * (1.0 - this->original_length / modulus));
            }

            virtual dataType energy() const
            {
                vector_<N, dataType> dx = this->p2->get_position() - this->p1->get_position();
                dataType modulus = dx.modulus() - this->original_length;
                return modulus * modulus * k / 2;
            }
            void print() const
            {
                printf("spring force:");
                force(0).print();
                printf("\nenergy:%4.4e\n", (double)energy());
            }
            void init(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                      dataType k, dataType original_length)
            {
                this->p1 = p1;
                this->p2 = p2;
                this->k = k;
                this->original_length = original_length;
                p1->particle_interact_list.add(this);
                p2->particle_interact_list.add(this);
            }
            void init(Particle<N, dataType> *p1, Particle<N, dataType> *p2, const spring_ &another)
            {
                init(p1, p2, another.k, another.original_length);
            }

            virtual void detach()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                if (this->p2 != 0)
                    this->p2->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
                return;
            };
            spring_(dataType k, dataType original_length)
            {
                this->p1 = 0;
                this->p2 = 0;
                this->k = k;
                this->original_length = original_length;
            };

            spring_(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                    dataType k, dataType original_length)
            {
                init(p1, p2, k, original_length);
            };
            spring_(Particle<N, dataType> *p1, Particle<N, dataType> *p2, const spring_ &another)
            {
                init(p1, p2, another);
            }
            ~spring_()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                if (this->p2 != 0)
                    this->p2->particle_interact_list.remove(this);
            }
        };
        // 恒力牵引
        template <int N = 2, typename dataType = double>
        class constant_F : public interaction_between<N, dataType>
        {
        private:
            dataType F;

        public:
            // F=-k*dx
            virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const
            {
                vector_<N, dataType> dx = (thisp == this->p1) ? (this->p1->get_position() - this->p2->get_position()) : (this->p2->get_position() - this->p1->get_position());
                dataType modulus = dx.modulus();
                return dx / modulus * F;
            }

            virtual dataType energy() const
            {
                vector_<N, dataType> dx = this->p2->get_position() - this->p1->get_position();
                dataType modulus = dx.modulus();
                return modulus * F;
            }

            inline virtual uint8_t gray_color() const { return F > 0 ? 0 : 0x80; };

            void init(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                      dataType F)
            {
                this->p1 = p1;
                this->p2 = p2;
                this->F = F;
                p1->particle_interact_list.add(this);
                p2->particle_interact_list.add(this);
            }
            void init(Particle<N, dataType> *p1, Particle<N, dataType> *p2, const constant_F &another)
            {
                init(p1, p2, another.F);
            }

            virtual void detach()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                if (this->p2 != 0)
                    this->p2->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
                return;
            };
            constant_F(dataType F)
            {
                this->p1 = 0;
                this->p2 = 0;
                this->F = F;
            };

            constant_F(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                       dataType F)
            {
                init(p1, p2, F);
            };
            constant_F(Particle<N, dataType> *p1, Particle<N, dataType> *p2, const constant_F &another)
            {
                init(p1, p2, another);
            }
            ~constant_F()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                if (this->p2 != 0)
                    this->p2->particle_interact_list.remove(this);
            }
        };
        // 范德华力
        template <int N = 2, typename dataType = double>
        class VDW_ : public interaction_between<N, dataType>
        {
        private:
            dataType B;
            dataType A;

        public:
            // F =B/(r^12)- A/(r^6)
            virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const
            {
                vector_<N, dataType> dx = (thisp == this->p1) ? (this->p1->get_position() - this->p2->get_position()) : (this->p2->get_position() - this->p1->get_position());
                dataType modulus = dx.modulus(), r6 = modulus * modulus * modulus;
                r6 *= r6;
                return dx * (((B / r6 - A) / r6) / modulus);
            }
            void init(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                      dataType B, dataType A)
            {
                this->p1 = p1;
                this->p2 = p2;
                this->B = B;
                this->A = A;
                p1->particle_interact_list.add(this);
                p2->particle_interact_list.add(this);
            };
            void init(Particle<N, dataType> *p1, Particle<N, dataType> *p2, const VDW_ &another)
            {
                init(p1, p2, another.B, another.A);
            }
            VDW_(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                 dataType B, dataType A)
            {
                init(p1, p2, B, A);
            }
            VDW_(dataType B, dataType A)
            {
                this->p1 = 0;
                this->p2 = 0;
                this->B = B;
                this->A = A;
            }
            VDW_(Particle<N, dataType> *p1, Particle<N, dataType> *p2, const VDW_ &another)
            {
                init(p1, p2, another);
            };
            virtual void detach()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                if (this->p2 != 0)
                    this->p2->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
                return;
            };

            ~VDW_()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                if (this->p2 != 0)
                    this->p2->particle_interact_list.remove(this);
            };
        };
        // 类引力
        template <int N = 2, typename dataType = double>
        class quasi_gravity_ : public interaction_between<N, dataType>
        {
        private:
            dataType M = 0; // positive to attract. m1*m2*G in gravity.
        public:
            virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const
            {
                vector_<N, dataType> dx = (thisp == this->p1) ? (this->p1->get_position() - this->p2->get_position()) : (this->p2->get_position() - this->p1->get_position());
                dataType modulus = dx.modulus();
                return dx * (dataType(-1) * M / (modulus * modulus * modulus));
            };
            void init(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                      dataType M)
            {
                this->p1 = p1;
                this->p2 = p2;
                this->M = M;
                p1->particle_interact_list.add(this);
                p2->particle_interact_list.add(this);
            }
            void init(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                      const quasi_gravity_ &another)
            {
                init(p1, p2, another.M);
            }

            quasi_gravity_(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                           dataType M)
            {
                init(p1, p2, M);
            };
            quasi_gravity_(dataType M)
            {
                this->p1 = 0;
                this->p2 = 0;
                this->M = M;
            };

            quasi_gravity_(Particle<N, dataType> *p1, Particle<N, dataType> *p2, const quasi_gravity_ &another)
            {
                init(p1, p2, another);
            };
            virtual void detach()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                if (this->p2 != 0)
                    this->p2->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
                return;
            };

            ~quasi_gravity_()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                if (this->p2 != 0)
                    this->p2->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
            }
        };
        template <int N = 2, typename dataType = double>
        class quasi_gravity_n_ : public interaction_between<N, dataType>
        {
        private:
            dataType M = 0; // positive to attract. m1*m2*G in gravity.
        public:
            virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const
            {
                vector_<N, dataType> dx = (thisp == this->p1) ? (this->p1->get_position() - this->p2->get_position()) : (this->p2->get_position() - this->p1->get_position());
                dataType modulus = dx.modulus();
                return dx * (dataType(-1) * M / (modulus * modulus * modulus));
            };
            void init(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                      dataType M)
            {
                this->p1 = p1;
                this->p2 = p2;
                this->M = M;
                p1->particle_interact_list.add(this);
                p2->particle_interact_list.add(this);
            }
            void init(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                      const quasi_gravity_n_ &another)
            {
                init(p1, p2, another.M);
            }

            quasi_gravity_n_(Particle<N, dataType> *p1, Particle<N, dataType> *p2,
                             dataType M)
            {
                init(p1, p2, M);
            };
            quasi_gravity_n_(dataType M)
            {
                this->p1 = 0;
                this->p2 = 0;
                this->M = M;
            };

            quasi_gravity_n_(Particle<N, dataType> *p1, Particle<N, dataType> *p2, const quasi_gravity_n_ &another)
            {
                init(p1, p2, another);
            };
            virtual void detach()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                if (this->p2 != 0)
                    this->p2->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
                return;
            };

            ~quasi_gravity_n_()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                if (this->p2 != 0)
                    this->p2->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
            }
            inline virtual bool visible() const
            {
                return false;
            };
        };
        // 盒子
        template <int N = 2, typename dataType = double>
        class box_ : public interaction_between<N, dataType>
        {
        private:
            vector_<N, dataType> l_limit;
            vector_<N, dataType> h_limit;

        public:
            virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const
            {
                vector_<N, dataType> posi = thisp->get_position();
                vector_<N, dataType> v = thisp->get_velocity();
                vector_<N, dataType> impu;
                dataType m = thisp->get_mass();
                vector_<N, bool> comp1 = posi < l_limit;
                vector_<N, bool> comp2 = posi > h_limit;
                for (size_t i = 0; i < N; i++)
                {
                    if ((comp1.vec[i] && v.vec[i] < 0) || (comp2.vec[i] && v.vec[i] > 0))
                    { // 负速度撞下界，正方向冲量；正速度撞上界，负方向冲量
                        impu.vec[i] = -m * v.vec[i];
                    }
                    else
                    { // ；否则没有
                        impu.vec[i] = dataType(0);
                    }
                }
                impu += impu * 1.0; // 完全弹性碰撞是1，否则是0；
                thisp->impulse(impu);

                return vector_<N, dataType>();
            };
            void init(Particle<N, dataType> *p1,
                      vector_<N, dataType> l_limit, vector_<N, dataType> h_limit)
            {
                this->p1 = p1;
                this->p2 = 0;
                for (size_t i = 0; i < N; i++)
                {
                    if (l_limit.vec[i] < h_limit.vec[i])
                    {
                        this->l_limit.vec[i] = l_limit.vec[i];
                        this->h_limit.vec[i] = h_limit.vec[i];
                    }
                    else
                    {
                        this->l_limit.vec[i] = h_limit.vec[i];
                        this->h_limit.vec[i] = l_limit.vec[i];
                    }
                }
                p1->particle_interact_list.add(this);
            };
            void init(Particle<N, dataType> *p1,
                      const box_ &another)
            {
                init(p1, another.l_limit, another.h_limit);
            }

            box_(Particle<N, dataType> *p1,
                 vector_<N, dataType> l_limit, vector_<N, dataType> h_limit)
            {
                init(p1, l_limit, h_limit);
            };
            box_(vector_<N, dataType> l_limit, vector_<N, dataType> h_limit)
            {
                this->p1 = 0;
                this->p2 = 0;
                for (size_t i = 0; i < N; i++)
                {
                    if (l_limit.vec[i] < h_limit.vec[i])
                    {
                        this->l_limit.vec[i] = l_limit.vec[i];
                        this->h_limit.vec[i] = h_limit.vec[i];
                    }
                    else
                    {
                        this->l_limit.vec[i] = h_limit.vec[i];
                        this->h_limit.vec[i] = l_limit.vec[i];
                    }
                }
            };

            box_(Particle<N, dataType> *p1, const box_ &another)
            {
                init(p1, another);
            };
            virtual void detach()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                this->p1 = 0;
                return;
            };

            ~box_()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                this->p1 = 0;
            }
        };
        // 盒子II型，更加精确
        template <int N = 2, typename dataType = double>
        class box_II : public interaction_between<N, dataType>
        {
        private:
            vector_<N, dataType> l_limit;
            vector_<N, dataType> h_limit;

        public:
            virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const
            {
                vector_<N, dataType> *posip = (vector_<N, dataType> *)&thisp->get_position();
                vector_<N, dataType> &posi = *posip;
                vector_<N, dataType> v = thisp->get_velocity();
                vector_<N, dataType> impu;
                dataType m = thisp->get_mass();
                vector_<N, bool> comp1 = posi < l_limit;
                vector_<N, bool> comp2 = posi > h_limit;
                for (size_t i = 0; i < N; i++)
                {
                    if (comp1.vec[i])
                    { // 负速度撞下界，正方向冲量，预位移为正
                        impu.vec[i] = -m * v.vec[i];
                        posi.vec[i] += (l_limit.vec[i] - posi.vec[i]) * 2;
                    }
                    else if (comp2.vec[i])
                    { // 正速度撞上界，负方向冲量，预位移为负
                        impu.vec[i] = -m * v.vec[i];
                        posi.vec[i] -= (posi.vec[i] - h_limit.vec[i]) * 2;
                    }
                    else
                    { // ；否则没有
                        impu.vec[i] = dataType(0);
                    }
                }
                impu += impu * 1.0; // 完全弹性碰撞是1，否则是0；
                thisp->impulse(impu);

                return vector_<N, dataType>();
            };
            void init(Particle<N, dataType> *p1,
                      vector_<N, dataType> l_limit, vector_<N, dataType> h_limit)
            {
                this->p1 = p1;
                this->p2 = 0;
                for (size_t i = 0; i < N; i++)
                {
                    if (l_limit.vec[i] < h_limit.vec[i])
                    {
                        this->l_limit.vec[i] = l_limit.vec[i];
                        this->h_limit.vec[i] = h_limit.vec[i];
                    }
                    else
                    {
                        this->l_limit.vec[i] = h_limit.vec[i];
                        this->h_limit.vec[i] = l_limit.vec[i];
                    }
                }
                p1->particle_interact_list.add(this);
            };
            void init(Particle<N, dataType> *p1,
                      const box_II &another)
            {
                init(p1, another.l_limit, another.h_limit);
            }

            box_II(Particle<N, dataType> *p1,
                   vector_<N, dataType> l_limit, vector_<N, dataType> h_limit)
            {
                init(p1, l_limit, h_limit);
            };
            box_II(vector_<N, dataType> l_limit, vector_<N, dataType> h_limit)
            {
                this->p1 = 0;
                this->p2 = 0;
                for (size_t i = 0; i < N; i++)
                {
                    if (l_limit.vec[i] < h_limit.vec[i])
                    {
                        this->l_limit.vec[i] = l_limit.vec[i];
                        this->h_limit.vec[i] = h_limit.vec[i];
                    }
                    else
                    {
                        this->l_limit.vec[i] = h_limit.vec[i];
                        this->h_limit.vec[i] = l_limit.vec[i];
                    }
                }
            };

            box_II(Particle<N, dataType> *p1, const box_II &another)
            {
                init(p1, another);
            };
            virtual void detach()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                this->p1 = 0;
                return;
            };

            ~box_II()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                this->p1 = 0;
            }
        };

        // 阻尼器一型-速度线性阻力
        template <int N = 2, typename dataType = double>
        class damper_I : public interaction_between<N, dataType>
        {
        private:
            dataType D;

        public:
            virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const
            {
                return thisp->get_velocity() * (-D);
            };
            void init(Particle<N, dataType> *p1,
                      dataType D)
            {
                this->p1 = p1;
                this->p2 = 0;
                this->D = D;
                p1->particle_interact_list.add(this);
            };
            void init(Particle<N, dataType> *p1,
                      const damper_I &another)
            {
                init(p1, another.D);
            }
            damper_I(Particle<N, dataType> *p1,
                     dataType D)
            {
                init(p1, D);
            };
            damper_I(dataType D)
            {
                this->p1 = 0;
                this->p2 = 0;
                this->D = D;
            };

            damper_I(Particle<N, dataType> *p1, const damper_I &another)
            {
                init(p1, another);
            };
            virtual void detach()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
                return;
            };

            ~damper_I()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
            }
        };
        // 阻尼器二型-速度指数衰减
        template <int N = 2, typename dataType = double>
        class damper_II : public interaction_between<N, dataType>
        {
        private:
            dataType D;

        public:
            virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const
            {
                thisp->impulse(thisp->get_velocity() * thisp->get_mass() * (-D));
                return vector_<N, dataType>();
            };
            void init(Particle<N, dataType> *p1,
                      dataType D)
            {
                this->p1 = p1;
                this->p2 = 0;
                this->D = D;
                p1->particle_interact_list.add(this);
            };
            void init(Particle<N, dataType> *p1,
                      const damper_II &another)
            {
                init(p1, another.D);
            }
            damper_II(Particle<N, dataType> *p1,
                      dataType D)
            {
                init(p1, D);
            };
            damper_II(dataType D)
            {
                this->p1 = 0;
                this->p2 = 0;
                this->D = D;
            };

            damper_II(Particle<N, dataType> *p1, const damper_II &another)
            {
                init(p1, another);
            };
            virtual void detach()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
                return;
            };

            ~damper_II()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                this->p1 = 0;
                this->p2 = 0;
            }
        };

#ifdef _bionukg_inputs_h
        // 鼠标指针
        template <int N = 2, typename dataType = double>
        class mouse_bind_particle : public interaction_between<N, dataType>
        {
        private:
            void (*point_translater)(POINT *p);

        public:
            virtual vector_<N, dataType> force(Particle<N, dataType> *thisp) const
            {
                POINT p;
                GetCursorPos(&p);
                if (point_translater != NULL)
                    this->point_translater(&p);
                thisp->move_position(vector_<2, double>(p.x, p.y));
                thisp->impulse(-thisp->get_momentum());
                return vector_<N, dataType>();
            };
            void init(Particle<N, dataType> *p1, void (*point_translater)(POINT *p))
            {
                this->p1 = p1;
                this->p2 = 0;
                this->point_translater = point_translater;
                p1->particle_interact_list.add(this);
            };
            void init(Particle<N, dataType> *p1,
                      const mouse_bind_particle &another)
            {
                init(p1, another.point_translater);
            }

            mouse_bind_particle(Particle<N, dataType> *p1, void (*point_translater)(POINT *p))
            {
                init(p1, point_translater);
            };

            mouse_bind_particle(void (*point_translater)(POINT *p))
            {
                this->p1 = 0;
                this->p2 = 0;
                this->point_translater = point_translater;
            };
            mouse_bind_particle(Particle<N, dataType> *p1, const mouse_bind_particle &another)
            {
                init(p1, another);
            };
            virtual void detach()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                this->p1 = 0;
                return;
            };

            ~mouse_bind_particle()
            {
                if (this->p1 != 0)
                    this->p1->particle_interact_list.remove(this);
                this->p1 = 0;
            }
        };

#endif
        // <"dataType">存储速度、质量等信息的数据类型，建议使用double或float
        // <"N">在N维空间中运动
        template <int N = 2, typename dataType = double>
        class Particle
        {
        private:
            vector_<N, dataType> position; // 位置
            dataType mass;                 // 质量
            vector_<N, dataType> velocity; // 速度
        public:
            list_<interaction_between<N, dataType> *> particle_interact_list;
            void detach_all()
            {
                size_t len = particle_interact_list.get_length();

                for (size_t i = 0; i < len; i++)
                {
                    interaction_between<N, dataType> *inter = particle_interact_list[0];
                    inter->detach();
                    delete inter;
                }
                return;
            }
            // 获得位置
            const vector_<N, dataType> &get_position() const { return position; };
            const dataType &get_position_X() const
            {
                static_assert(N >= 1, "X is not exist");
                return position.vec[0];
            }
            const dataType &get_position_Y() const
            {
                static_assert(N >= 2, "Y is not exist");
                return position.vec[1];
            }
            // 设置位置
            void move_position(const vector_<N, dataType> &new_position) { this->position = new_position; };
            // 获得速度
            const vector_<N, dataType> &get_velocity() const { return velocity; };
            // 施加冲量
            void impulse(const vector_<N, dataType> &impu)
            {
                // 根据冲量-动量定理，冲量等于动量的变化量
                // 动量等于质量乘以速度
                // 因此，速度的变化量等于冲量除以质量
                vector_<N, dataType> dv = impu / mass; // 使用标量除法运算符重载
                // 更新速度
                velocity += dv; // 使用向量加法运算符重载
            }
            // 获得质量
            const dataType &get_mass() const { return mass; };

        public:
            // 所有相互作用的列表

            void init(dataType x, dataType y, dataType m)
            {
                static_assert(N == 2, "N==2");
                position = vector_<N, dataType>(x, y);
                mass = m;
                velocity = vector_<N, dataType>(); // 默认构造函数，所有分量为0
                particle_interact_list.imm__clear();
            }
            Particle(vector_<N, dataType> p, dataType m)
            {
                position = p;
                mass = m;
                velocity = vector_<N, dataType>(); // 默认构造函数，所有分量为0
                particle_interact_list.imm__clear();
            }
            Particle(dataType x, dataType y, dataType m)
            {
                static_assert(N == 2, "N==2");
                position = vector_<N, dataType>(x, y);
                mass = m;
                velocity = vector_<N, dataType>(); // 默认构造函数，所有分量为0
                particle_interact_list.imm__clear();
            }
            Particle(dataType x, dataType y, dataType vx, dataType vy,dataType m)
            {
                static_assert(N == 2, "N==2");
                position = vector_<N, dataType>(x, y);
                mass = m;
                velocity = vector_<N, dataType>(vx,vy);
                particle_interact_list.imm__clear();
            }

            Particle()
            {
                position = vector_<N, dataType>();
                mass = 1;
                velocity = vector_<N, dataType>();
                particle_interact_list.imm__clear();
            }
            // 拷贝构造函数
            Particle(const Particle &another)
            {
                *this = another;
            }
            // void print() const {
            //     // 使用标准输出流输出位置和速度
            //     std::cout << "Position: (";
            //     for (int i = 0; i < N; i++) { // 遍历向量的所有分量
            //         printf("%4.4e", position.vec[i]); // 使用[]访问向量的元素
            //         if (i < N - 1) { // 如果不是最后一个分量，输出逗号和空格
            //             std::cout << ", ";
            //         }
            //     }
            //     std::cout << ")" << std::endl; // 输出右括号和换行符

            //    std::cout << "Velocity: (";
            //    for (int i = 0; i < N; i++) { // 遍历向量的所有分量
            //        printf("%4.4e", velocity.vec[i]);// 使用[]访问向量的元素
            //        if (i < N - 1) { // 如果不是最后一个分量，输出逗号和空格
            //            std::cout << ", ";
            //        }
            //    }
            //    std::cout << ")" << std::endl; // 输出右括号和换行符
            //    for (int i = 0; i < particle_interact_list.get_length(); i++)
            //    {
            //        printf("interact %d:%p\n", i, particle_interact_list.list[i]);
            //    }
            //}
            // 刷新速度
            void refresh_velocity(dataType dt)
            {
                // 遍历列表得到所受合力
                vector_<N, dataType> force; // 默认构造函数，所有分量为0
                interaction_between<N, dataType> *p = 0;
                for (size_t i = 0; i < particle_interact_list.get_length(); i++)
                {
                    p = particle_interact_list[i];
                    const vector_<N, dataType> &tem = p->force(this);
                    if (tem.is_all_finite())
                        force += tem;
                }
                // 使用牛顿第二定律计算加速度、速度和位置
                // 计算加速度
                vector_<N, dataType> acceleration = force / mass; // 使用标量除法运算符重载
                // 更新速度
                velocity += acceleration * dt; // 使用向量加法和标量乘法运算符重载
                if (!(isfinite(velocity.vec[0]) && isfinite(velocity.vec[1])))
                {
                    system("pause");
                }
            }
            // 刷新速度，使用狭义相对论
            void refresh_velocity_relativity(dataType dt, dataType c = 3e8)
            {

                // 遍历列表得到所受合力

                vector_<N, dataType> force; // 默认构造函数，所有分量为0
                interaction_between<N, dataType> *p = 0;
                for (size_t i = 0; i < particle_interact_list.get_length(); i++)
                {
                    p = particle_interact_list[i];
                    force += p->force(this);
                }
                // 使用狭义相对论计算加速度、速度和位置
                //  计算加速度
                auto mo = velocity.modulus();
                dataType gamma = 1 / sqrt(1 - mo * mo / (c * c)); // 计算洛伦兹因子

                vector_<N, dataType> acceleration = force / (mass * gamma); // 使用标量除法运算符重载
                // 更新速度
                auto nvelocity = velocity + acceleration * dt; // 使用向量加法和标量乘法运算符重载

                // 限制速度不超过光速或指定的上限

                if (isfinite(nvelocity.modulus()))
                {
                    while (nvelocity.modulus() >= c)
                    {
                        nvelocity += velocity;
                        nvelocity /= 2;
                    }
                }
                else
                {
                    for (size_t i = 0; i < N; i++)
                    {
                        nvelocity.vec[i] = dataType(0.5) * c;
                    }
                }

                velocity = nvelocity;
            };

            //void refresh_velocity_impact(dataType dt)
            //{
            //    interaction_between<N, dataType> *p = 0;
            //    for (size_t i = 0; i < particle_interact_list.get_length(); i++)
            //    {
            //        p = particle_interact_list[i];
            //        const vector_<N, dataType> &temF = p->force(this);
            //        if (temF.is_all_finite())
            //        {
            //        }
            //    }
            //    // 使用牛顿第二定律计算加速度、速度和位置
            //    // 计算加速度
            //    vector_<N, dataType> acceleration = force / mass; // 使用标量除法运算符重载
            //    // 更新速度
            //    velocity += acceleration * dt; // 使用向量加法和标量乘法运算符重载
            //    if (!(isfinite(velocity.vec[0]) && isfinite(velocity.vec[1])))
            //    {
            //        system("pause");
            //    }
            //}

            // 刷新位置
            void refresh_position(dataType dt)
            {
                // 更新位置
                position += velocity * dt; // 使用向量加法和标量乘法运算符重载
                if (!(isfinite(position.vec[0]) && isfinite(position.vec[1])))
                {
                    system("pause");
                }
            }
            vector_<N, dataType> get_momentum() const
            {
                // 获得其动量，用于动量守恒
                return velocity * mass;
            }
            dataType get_kinetic_energy() const
            {
                // 获得其动能（非相对论），用于能量守恒
                dataType vmod = velocity.modulus();
                return vmod * vmod * mass / 2;
            }
            void operator=(const Particle &another)
            {
                position = another.position;
                mass = another.mass;
                velocity = another.velocity;
                particle_interact_list = another.particle_interact_list;
            }
        };

        typedef Particle<2, double> Particle_2df64;

        template <int N = 2, typename dataType = double>
        class Particle_system
        {
        private:
            list_<Particle<N, dataType>> list_of_particles;
            list_<interaction_between<N, dataType> *> list_of_interactions;

        public:
            const Particle<N, dataType> &get_particle(size_t idx) const
            {
                const list_<Particle<N, dataType>> &r = this->list_of_particles;
                return r.operator[](idx);
            };
            size_t get_particle_count() const { return list_of_particles.get_length(); }
            typedef interaction_between<N, dataType> *tptest;
            typedef const tptest ctptest;
            interaction_between<N, dataType> *const &get_interactions(size_t idx) const
            {
                return this->list_of_interactions[idx];
            };
            size_t get_interaction_count() const { return list_of_interactions.get_length(); }

        public:
            void run(const dataType &dt)
            {
                for (size_t i = 0; i < list_of_particles.get_length(); i++)
                {
                    const Particle<N, dataType> &cp = list_of_particles[i];
                    Particle<N, dataType> &p = *(Particle<N, dataType> *)&cp;
                    p.refresh_velocity(dt);
                }
                for (size_t i = 0; i < list_of_particles.get_length(); i++)
                {
                    const Particle<N, dataType> &cp = list_of_particles[i];
                    Particle<N, dataType> &p = *(Particle<N, dataType> *)&cp;
                    p.refresh_position(dt);
                }
            }
            void run_rel(const dataType &dt, const dataType &c)
            {
                for (size_t i = 0; i < list_of_particles.get_length(); i++)
                {
                    const Particle<N, dataType> &cp = list_of_particles[i];
                    Particle<N, dataType> &p = *(Particle<N, dataType> *)&cp;
                    p.refresh_velocity_relativity(dt, c);
                }
                for (size_t i = 0; i < list_of_particles.get_length(); i++)
                {
                    const Particle<N, dataType> &cp = list_of_particles[i];
                    Particle<N, dataType> &p = *(Particle<N, dataType> *)&cp;
                    p.refresh_position(dt);
                }
            }

            void add_particle(const Particle<N, dataType> &p)
            {
                list_of_particles.add(p);
            }
            template <class interaction_Ty>
            void connect_with(size_t p1_idx, size_t p2_idx, const interaction_Ty &ita)
            {
                const Particle<N, dataType> &cp1 = list_of_particles[p1_idx];
                Particle<N, dataType> &p1 = *(Particle<N, dataType> *)&cp1;
                const Particle<N, dataType> &cp2 = list_of_particles[p2_idx];
                Particle<N, dataType> &p2 = *(Particle<N, dataType> *)&cp2;

                interaction_Ty *con = new interaction_Ty(&p1, &p2, ita);
                this->list_of_interactions.add(con);
            }
            void disconnect(size_t p1_idx, size_t p2_idx)
            {
                Particle<N, dataType> *cp1 = (Particle<N, dataType> *)&list_of_particles[p1_idx];
                Particle<N, dataType> *cp2 = (Particle<N, dataType> *)&list_of_particles[p2_idx];
                list_<interaction_between<N, dataType> *> &list1 = cp1->particle_interact_list;
                list_<interaction_between<N, dataType> *> &list2 = cp2->particle_interact_list;

                list_<interaction_between<N, dataType> *> &list = list1.get_length() > list2.get_length() ? list2 : list1;
                for (size_t i = 0; i < list.get_length(); i++)
                {
                    interaction_between<N, dataType> *inter = list[i];
                    if ((inter->p1 == cp1 && inter->p2 == cp2) || (inter->p1 == cp2 && inter->p2 == cp1))
                    {
                        delete inter;
                        this->list_of_interactions.remove(inter);
                        i--;
                    }
                }

                return;
            }
            template <class field_Ty>
            void field_set(size_t p1_idx, const field_Ty &ita)
            {
                const Particle<N, dataType> &cp = list_of_particles[p1_idx];
                Particle<N, dataType> &p = *(Particle<N, dataType> *)&cp;
                field_Ty *con = new field_Ty(&p, ita);
                this->list_of_interactions.add(con);
            }

            void remove_particle(size_t p_idx)
            {
                Particle<N, dataType> *p = (Particle<N, dataType> *)&(this->list_of_particles[p_idx]);
                p->detach_all();
                this->list_of_particles.remove(p_idx);
            }
#ifdef _bionukg_graphics_h
            void draw(bmp *pic, double line_w = 1.5, double point_r = 1.5)
            {
                static_assert(N == 2, "this is 2D drawer");
                pic->clear(0xffffffff);
                for (size_t i = 0; i < this->list_of_interactions.get_length(); i++)
                {
                    interaction_between<N, dataType> *ibp = this->list_of_interactions[i];
                    if (ibp->p2 != 0 && ibp->visible())
                    {
                        auto p1x = ibp->p1->get_position().vec[0];
                        auto p1y = ibp->p1->get_position().vec[1];
                        auto p2x = ibp->p2->get_position().vec[0];
                        auto p2y = ibp->p2->get_position().vec[1];

                        pic->draw_line_segment_uncovered_o1(p1x, p1y, p2x, p2y,
                                                            line_w, ibp->gray_color());
                    }
                }
                for (size_t i = 0; i < this->list_of_particles.get_length(); i++)
                {
                    const Particle<N, dataType> &p = this->list_of_particles[i];
                    pic->draw_solid_circle(p.get_position().vec[0], p.get_position().vec[1], point_r / 2, point_r);
                }
            }
            void draw_at(bmp* pic, double line_w = 1.5, double point_r = 1.5)
            {
                draw(pic, line_w, point_r);
            }


#endif // _bionukg_graphics_h
            ~Particle_system()
            {
                for (size_t i = 0; i < this->list_of_interactions.get_length(); i++)
                {
                    interaction_between<N, dataType> *ibp = this->list_of_interactions[i];
                    ibp->imm_clear();
                    delete ibp;
                }
            }
        };

        // template<int N, typename dataType>
        // vector_<N, dataType> F_spring(Particle<N, dataType>* p1, Particle<N, dataType>* p2, void* arg_ = 0)
        //{
        //     F_spring_arg<dataType>& arg = *(F_spring_arg<dataType>*) arg_;
        //     vector_<N, dataType> dx = (p2->get_position() - p1->get_position());
        //     dataType modulus = dx.modulus();
        //     return dx * arg.k * (1.0 - arg.original_length / modulus);
        // };

#ifdef namespace_bionukg
#ifdef namespace_phy
    }
#endif // namespace_phy
}
#endif // namespace_bionukg

#endif //_bionukg_phys_eng_h
