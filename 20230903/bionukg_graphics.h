//////////////////////////////////ANSI
#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS
// #define fwrite(buffer,size,leng,fp) {fwrite(buffer,size,leng,fp);fw++;}

#ifndef _bionukg_graphics_h
#define _bionukg_graphics_h

#include <iostream>

// #include<Windows.h>
#ifndef MAX_PATH
#define MAX_PATH 260
#endif // !MAX_PATH

// #include"./bionukg_optimize.h"

#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

	typedef class bmp_header
	{
	public:
		uint32_t width = 0;	   // ���ؿ���
		uint32_t memwidth = 0; // �ڴ���һ�е��ֽ���
		uint32_t height = 0;   // ���ظ߶�
		uint16_t bitdepth = 0; // λ���?(24��32)
		uint32_t size = 0;	   // �ܴ�С=���ݴ�С+bmp_headerlength
		uint32_t datasize = 0; // ���ݿ���?

		bmp_header(uint32_t wid, uint32_t hei, uint32_t bitdep)
		{
			init(wid, hei, bitdep);
		}
		bmp_header()
		{
		}
		void init(uint32_t wid, uint32_t hei, uint32_t bitdep)
		{
			width = wid;
			height = hei;
			bitdepth = bitdep;
			while (wid % 4 != 0)
			{
				wid++;
			}
			memwidth = wid * (bitdepth / 8);
			datasize = memwidth * height;
			size = datasize + 54;
		}
	} bmphd;

	typedef struct point
	{
		uint32_t x;
		uint32_t y;
	};

	/// <summary>
	/// ���½�Ϊԭ�㣬����ϵ�ڵڶ����ޣ��ӵ�һ��
	/// </summary>
	class bmp
	{

#define bmp_headerlength 54

	public:
		uint32_t width;	   // ���ؿ���
		uint32_t memwidth; // �ڴ���һ�е��ֽ���
		uint32_t height;   // ���ظ߶�
		uint16_t bitdepth; // λ���?(24��32)
		uint32_t size;	   // �ܴ�С
		uint32_t datasize; // ���ݿ���?

		uint8_t *file = 0; // �ļ����ݵ�ַ
		uint8_t *data = 0; // �������ݵ�ַ= file + bmp_headerlength;

		char path[MAX_PATH] = {0}; // �ļ�·��
		// ����Ϊ��ֵ̬
		// ����Ϊ��ֵ̬
		uint32_t x = 0;			// �����������?���½�ԭ�㣩
		uint32_t y = 0;			// �����������꣨���½�ԭ�㣩
		uint32_t memoffset = 0; // �������ڴ�����
		uint8_t rgb[4] = {0};	// ������ͨλͼ�ĵ������ص���ɫRGBֵ���?

		// ����һ����ʽ��Сһģһ�����µĿ�bmp
		bmp(bmp *formatted)
		{
			this->width = formatted->width;
			this->memwidth = formatted->memwidth;
			this->height = formatted->height;
			this->bitdepth = formatted->bitdepth;
			this->size = formatted->size;
			this->datasize = formatted->datasize;
			this->file = new uint8_t[size];
			this->data = this->file + bmp_headerlength;
		}
		// ����һ��bmp
		bmp(const bmp &copyed)
		{
			this->width = copyed.width;
			this->memwidth = copyed.memwidth;
			this->height = copyed.height;
			this->bitdepth = copyed.bitdepth;
			this->size = copyed.size;
			this->datasize = copyed.datasize;
			this->file = new uint8_t[this->size];
			this->data = this->file + bmp_headerlength;
			memcpy(this->file, copyed.file, this->size);
		}
		// ����bmp�ļ�ͷ�ṹ������bmp
		bmp(bmphd hd)
		{
			width = hd.width;		//
			memwidth = hd.memwidth; // �ڴ���һ�е��ֽ���
			height = hd.height;		// ���ظ߶�
			bitdepth = hd.bitdepth; // λ���?(24��32)
			size = hd.size;			// �ܴ�С
			datasize = hd.datasize; // ���ݿ���?
			this->file = (uint8_t *)malloc(sizeof(uint8_t) * size);
			this->data = this->file + bmp_headerlength;
		}
		// ɶ��û�г�ʼ��
		bmp(void){};

		// ��·���µ�.bmp�ļ�
		bmp(const char *newpath)
		{
			if (0 != this->open(newpath))
			{
				printf_s("\nbmp open exception\n");
				system("pause");
			}
		}
		~bmp()
		{
			close();
		}

		void close()
		{
			if (file != NULL)
			{
				free(file);
				file = NULL;
				data = NULL;
			}
		}

		// ���ļ�newpathд���ڴ�file(ԭ�ڴ�δɾ��)
		int32_t // -1:·�������� -3:�ڴ治��
		open(const char *newpath)
		{
			if (newpath == 0)
				return -1;
			if (path == 0)
				return -1;
			FILE *fp;
			strcpy_s(path, newpath);
			fopen_s(&fp, path, "rb");
			if (fp == 0)
			{
				return -2;
			}
			uint8_t headerdata[bmp_headerlength] = {0};
			fread(headerdata, 1, bmp_headerlength, fp);

			size = *(uint32_t *)&headerdata[2];
			width = *(uint32_t *)&headerdata[18];
			height = *(uint32_t *)&headerdata[22];
			bitdepth = *(uint16_t *)&headerdata[28];
			datasize = *(uint32_t *)&headerdata[34];
			file = new uint8_t[size];
			if (file == 0)
			{
				fclose(fp);
				return -3;
			}
			data = file + bmp_headerlength;
			fseek(fp, 0, SEEK_SET);
			fread(file, 1, size, fp);

			fclose(fp);
			memwidth = width * bitdepth / 8;
			while (memwidth % 4 != 0)
			{
				memwidth++;
			}
			return 0;
		}
		// ���ļ�д��Ӳ��
		uint32_t //-1:·�������ڣ�-2:
		save_as(
			const char *newpath // ����0ʱ����ԭ·���������޸�·��Ϊ����·��
		)
		{
			if (newpath == 0)
				return -1;
			strcpy_s(path, newpath);
			if (path == 0)
				return -1;
			FILE *fp;
			fopen_s(&fp, path, "wb+");
			if (fp == 0)
			{
				return -2;
			}
			uint8_t def[bmp_headerlength] = {
				66, 77,		 // �̶�BM 0~1
				0, 0, 0, 0,	 // �ļ���Сsize=datasize+bmp_headerlength 2~5
				0, 0, 0, 0,	 // �̶� 6~9
				54, 0, 0, 0, // �̶��ļ���ͷ���� 10~13
				40, 0, 0, 0, // �̶� 14~17
				0, 0, 0, 0,	 // ��width 18~21
				0, 0, 0, 0,	 // ��height 22~25
				1, 0,		 // �̶� 26~27
				24, 0,		 // 24λͼbitdepth24���̶� 28~29
				0, 0, 0, 0,	 // �̶� 30~33
				0, 0, 0, 0,	 // �������ݴ�Сdatasize 34~37
				0, 0, 0, 0,	 // �̶� 38~41
				0, 0, 0, 0,	 // �̶� 42~45
				0, 0, 0, 0,	 // �̶� 46~49
				0, 0, 0, 0,	 // �̶� 50~53
							// Ĭ���ļ���ͷ
			};
			*(uint32_t *)&def[2] = size;
			*(uint32_t *)&def[18] = width;
			*(uint32_t *)&def[22] = height;
			*(uint16_t *)&def[28] = bitdepth;
			*(uint32_t *)&def[34] = datasize;
			fwrite(def, 1, bmp_headerlength, fp);
			fwrite(data, 1, datasize, fp);
			fclose(fp);
			return 0;

			// fopen_s(&fp, path, "wb+");
			// if (fp == 0)
			//{
			//	return -2;
			// }

			//*(uint32_t*)&def[2] = size;
			//*(uint32_t*)&def[18] = width;
			//*(uint32_t*)&def[22] = height;
			//*(uint16_t*)&def[28] = bitdepth;
			//*(uint32_t*)&def[34] = datasize;
			// fwrite(def, 1, bmp_headerlength, fp);
			// fwrite(data, 1, datasize, fp);
			// fclose(fp);
			// return 0;
		}

		/// ����Ϊpng
		// �ο����ף�
		//  [1]PNG�ļ���ʽ��Ƥ�� W3C https://blog.csdn.net/yongzhewuwei_2008/article/details/1007204 [EB/OL]2022/7/31
		//  [2]svpng miloyip http://github.com/miloyip/svpng [EB/OL]2022/7/31
		int32_t		 //-1����·����Ч,-2�ļ���ʧ�ܣ�-3�ļ�λ��Ȳ��?24��32
		save_as_png( // �ο���
			_In_ const char *newpath)
		{
#ifndef SVPNG_INC_
#ifdef bionukg_optimize
#define fwriteFp(ptr, len) fwrite_biopt(ptr, size, fp, buffer, idx)
#else // bionukg_optimize
#define fwriteFp(ptr, len) fwrite(ptr, 1, len, fp)
#endif
#define fwrite32swap(u32p)                                      \
	{                                                           \
		*(uint32_t *)u32p = _byteswap_ulong(*(uint32_t *)u32p); \
		fwriteFp(u32p, 4);                                      \
		*(uint32_t *)u32p = _byteswap_ulong(*(uint32_t *)u32p); \
	}
#define fwriteFpCRC(ptr, len)                      \
	{                                              \
		fwriteFp(ptr, len);                        \
		for (i = 0; i < len; i++)                  \
		{                                          \
			crc ^= ((uint8_t *)ptr)[i];            \
			crc = (crc >> 4) ^ CRCtable[crc & 15]; \
			crc = (crc >> 4) ^ CRCtable[crc & 15]; \
		}                                          \
	}
#define fwrite32CRCswap(u32p)                                   \
	{                                                           \
		*(uint32_t *)u32p = _byteswap_ulong(*(uint32_t *)u32p); \
		fwriteFpCRC(u32p, 4);                                   \
		*(uint32_t *)u32p = _byteswap_ulong(*(uint32_t *)u32p); \
	}
#define fwrite8CRCadler(u8p)                    \
	{                                           \
		fwriteFpCRC(u8p, 1);                    \
		ADLER_a = (ADLER_a + (*u8p)) % 0xfff1;  \
		ADLER_b = (ADLER_b + ADLER_a) % 0xfff1; \
	}
#define fwriteChunkBegin(name32p, leng32p) \
	{                                      \
		fwrite32swap(leng32p);             \
		crc = ~0U;                         \
		fwriteFpCRC(name32p, 4);           \
	}
#define fwriteChunkEnd()   \
	{                      \
		crc = ~crc;        \
		fwrite32swap(&crc) \
	};
			if (newpath == 0)
				return -1;
			bool alpha = false;

			if (this->bitdepth == 24)
				alpha = false;
			else if (this->bitdepth == 32)
				alpha = true;
			else
				return -3;
			FILE *fp;
			fopen_s(&fp, newpath, "wb+");
			if (fp == 0)
				return -2;
			uint8_t pngStart[] =
				{
					0x89, 'P', 'N', 'G',
					'\r', '\n', 0x1A, '\n'};
			static const unsigned CRCtable[] = {// CRCУ���?
												0, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
												0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
												0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
												0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c};
			unsigned ADLER_a = 1, ADLER_b = 0, crc;
			uint16_t MemWidthP = this->width * (alpha ? 4 : 3) + 1, x, y, i;
			uint32_t uint32;
			uint16_t uint16, idx = 0;
			uint8_t uint8, buffer[256] = {0};

			fwriteFp(pngStart, 8);

			uint32 = 13;

			fwriteChunkBegin("IHDR", &uint32);
			fwrite32CRCswap(&(this->width));
			fwrite32CRCswap(&(this->height));
			uint8 = 8;
			fwriteFpCRC(&uint8, 1);
			uint8 = (alpha ? 6 : 2);
			fwriteFpCRC(&uint8, 1);
			uint32 = 0;
			fwriteFpCRC(&uint32, 3);
			fwriteChunkEnd();

			uint32 = 2 + this->height * (5 + MemWidthP) + 4;

			fwriteChunkBegin("IDAT", &uint32);
			uint16 = 0x0178;
			fwriteFpCRC(&uint16, 2);
			uint8_t *img = this->data;

			uint32 = ((~MemWidthP) << 16) | MemWidthP;
			MemWidthP -= 1;
			for (y = 0; y < this->height; y++)
			{
				uint8 = (y == height - 1) ? 1 : 0;
				fwriteFpCRC(&uint8, 1);
				fwriteFpCRC(&uint32, 4);
				uint8 = 0;
				fwriteFpCRC(&uint8, 1);
				fwriteFpCRC(img, MemWidthP);

				ADLER_a %= 0xfff1;
				ADLER_b = (ADLER_b + ADLER_a) % 0xfff1;
				for (x = 0; x < MemWidthP; x++, img++)
				{
					ADLER_a = (ADLER_a + (*img)) % 0xfff1;
					ADLER_b = (ADLER_b + ADLER_a) % 0xfff1;
				}
			}
			uint32 = (ADLER_b << 16) | ADLER_a;
			fwrite32CRCswap(&uint32);
			fwriteChunkEnd();

			uint32 = 0;

			fwriteChunkBegin("IEND", &uint32);
			fwriteChunkEnd();

			fclose(fp);
			// printf("2:%lld\n", fw);

			return 0;
			// #ifdef fwriteFp
			// #undef fwriteFp
			// #endif
#ifdef fwrite32swap
#undef fwrite32swap
#endif
#else
		if (newpath == 0)
			return -1;
		bool alpha = false;

		if (this->bitdepth == 24)
			alpha = false;
		else if (this->bitdepth == 32)
			alpha = true;
		else
			return -3;
		FILE *fp = fopen(newpath, "wb+");
		if (fp == 0)
			return -2;
		svpng(fp, this->width, this->height, this->data, alpha);
		fclose(fp);
#endif
		}
		// ���������ڴ�
		// uint(& open)(const char*newpath) = memloadfile;
		// uint(bmp::* save)(const char*) = diskloadfile;

		void pointfind(void)
		{
			if (x >= width)
				x = width - 1;
			if (x < 0)
				x = 0;
			if (y >= height)
				y = height - 1;
			if (y < 0)
				y = 0;

			memoffset = y * memwidth + x * (bitdepth / 8) + bmp_headerlength;
			return;
		}
		uint32_t pointfind(uint32_t x, uint32_t y)
		{
			if (x >= width)
				x = width - 1;
			if (x < 0)
				x = 0;
			if (y >= height)
				y = height - 1;
			if (y < 0)
				y = 0;
			return y * memwidth + x * (bitdepth / 8) + bmp_headerlength;
		}
		void pointfind_r(void)
		{
			if (x > width)
				x = width;
			if (x < 0)
				x = 0;
			if (y > height)
				y = height;
			if (y < 0)
				y = 0;

			memoffset = (height - y - 1) * memwidth + x * (bitdepth / 8) + bmp_headerlength;
			return;
		}
		uint32_t pointfind_r(uint32_t x, uint32_t y)
		{
			if (x > width)
				x = width;
			if (x < 0)
				x = 0;
			if (y > height)
				y = height;
			if (y < 0)
				y = 0;
			return (height - y - 1) * memwidth + x * (bitdepth / 8) + bmp_headerlength;
		}

		void pointread(uint32_t x, uint32_t y, uint8_t *rgb)
		{
			memcpy(rgb, file + pointfind(x, y), bitdepth / 8);
		}
		void pointread(uint32_t x, uint32_t y)
		{
			memcpy(rgb, file + pointfind(x, y), bitdepth / 8);
		}
		void pointread(void)
		{
			pointfind();
			memcpy(rgb, file + memoffset, bitdepth / 8);
		}
		void pointread_r(uint32_t x, uint32_t y, uint8_t *rgb)
		{
			memcpy(rgb, file + pointfind_r(x, y), bitdepth / 8);
		}
		// �����ȡ��rgbs
		void pointread_r(uint32_t x, uint32_t y)
		{
			memcpy(rgb, file + pointfind_r(x, y), bitdepth / 8);
		}
		void pointread_r(void)
		{
			pointfind_r();
			memcpy(rgb, file + memoffset, bitdepth / 8);
		}
		void pointwrite(uint32_t x, uint32_t y, uint8_t *rgb)
		{
			memcpy(file + pointfind(x, y), rgb, bitdepth / 8);
		}
		void pointwrite(uint32_t x, uint32_t y)
		{
			memcpy(file + pointfind(x, y), rgb, bitdepth / 8);
		}
		void pointwrite(void)
		{
			pointfind();
			memcpy(file + memoffset, rgb, bitdepth / 8);
		}
		void pointwrite_r(uint32_t x, uint32_t y, uint8_t *rgb)
		{
			memcpy(file + pointfind_r(x, y), rgb, bitdepth / 8);
		}
		void pointwrite_r(uint32_t x, uint32_t y)
		{
			memcpy(file + pointfind_r(x, y), rgb, bitdepth / 8);
		}
		void pointwrite_r(void)
		{
			pointfind_r();
			memcpy(file + memoffset, rgb, bitdepth / 8);
		}

		// uint pixelfunc(uint32_t x,uint32_t y,void(*fun)(uint*))
		void traversalpoints(void (*func)(bmp *))
		{
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < width; x++)
				{
					pointfind();
					memcpy(rgb, file + memoffset, bitdepth / 8);
					;
					func(this);
					memcpy(file + memoffset, rgb, bitdepth / 8);
				}
			}
		}
		void printtextfile(void)
		{
			// ix = 0;
			for (uint32_t i = 0; i < bmp_headerlength; i++)
			{

				printf("%3d,", file[i]);
				i++;
				printf("%3d,", file[i]);
				i++;
				printf("%3d\t", file[i]);
				// ix++;
				/*if (ix == 4)
				{
					ix = 0;
					printf("\n");
				}*/
			}
			// ix = 0;
			printf("\n");
			for (uint32_t i = bmp_headerlength; i < size; i++)
			{

				printf("%3d,", file[i]);
				i++;
				printf("%3d,", file[i]);
				i++;
				printf("%3d\t", file[i]);
				/*if (ix == 4)
				{
					ix = 0;
					printf("\n");
				}*/
			}
		}
		void clear(uint32_t color = 0)
		{
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < width; x++)
				{
					memcpy(file + y * memwidth + x * (bitdepth / 8) + bmp_headerlength, &color, bitdepth / 8);
				}
			}
		}
		/*void blockread(uint8_t*** block, uint wid, uint hei)
		{
			uint32_t x0 = x;
			uint32_t y0 = y;
			for (uint i = 0; i < hei; i++)
			{
				x++;
				for (uint j = 0; j < wid; j++)
				{
					y++;
					pointread();
					memcpy(block[i][j], rgb, bitdepth / 8);
				}
			}
			x = x0;
			y = y0;
			return;
		}*/

		void draw_line(point p1, point p2)
		{
			int64_t dx = p1.x, dy = p1.y; // delta
			dx -= p2.x, dy -= p2.y;
			int64_t udx = abs(dx), udy = abs(dy);
			int64_t px = 0, py = 0; // pen
			int64_t
				hori = 0, // ˮƽ
				vert = 0, // ��ֱ
				diag = 0; // б��

			for (; px < udx && py < udy;)
			{
				pointwrite(p1.x + dx > 0 ? px : -px, p1.y + dy > 0 ? py : -py);

				hori += udy;
				vert -= udx;
				diag = vert + udy;

				if (abs(diag) <= abs(vert) && abs(diag) <= abs(hori)) // diag��С
				{
					hori = diag;
					vert = diag;
					dx++, dy++;
				}
				else if (abs(vert) <= abs(hori)) // hori��С
				{
					vert = hori;
					dx++;
				}
				else // vert��С
				{
					hori = vert;
					dy++;
				}
			}
		}
		void draw_line_testpic(point p1, point p2)
		{
			double dx = p1.x, dy = p1.y; // delta
			dx -= p2.x, dy -= p2.y;
			double dl = sqrt(dx * dx + dy * dy);
			dx /= dl, dy /= dl;
			uint8_t rgbx[4] = {0, 0, 0, 0};
			double vx = 0, vy = 0, vt = 0;
			for (int x = 0; x < this->width; x++)
			{
				vx = (double)p1.x - x;
				for (int y = 0; y < this->height; y++)
				{
					vy = (double)p1.y - y;
					vt = vy * dx + vx * dy;
					vt = abs(vt);
					const double w = 1.5;
					if (vt > w)
					{
						vt = 255;
					}
					else
					{
						vt *= 256 / w;
					}
					rgbx[0] = vt;
					rgbx[1] = rgbx[0];
					rgbx[2] = rgbx[0];
					rgbx[3] = rgbx[0];
					pointwrite(x, y, rgbx);
				}
			}
		}
		void draw_line_segment(double p1x, double p1y, double p2x, double p2y, double l_width = 1.5)
		{
			double dx = p1x, dy = p1y; // delta
			dx -= p2x, dy -= p2y;
			double dl = sqrt(dx * dx + dy * dy);
			dx /= dl, dy /= dl;
			uint8_t rgbx[4] = {0, 0, 0, 0};
			double vx = 0, vy = 0, vt = 0;
			double xmind = 0, xmaxd = this->width,
				   ymind = 0, ymaxd = this->height;
			if (p1x < p2x)
			{
				xmind = xmind > p1x - l_width ? xmind : p1x - l_width;
				xmaxd = xmaxd < p2x + l_width ? xmaxd : p2x + l_width;
			}
			else
			{
				xmind = xmind > p2x - l_width ? xmind : p2x - l_width;
				xmaxd = xmaxd < p1x + l_width ? xmaxd : p1x + l_width;
			}

			if (p1y < p2y)
			{
				ymind = ymind > p1y - l_width ? ymind : p1y - l_width;
				ymaxd = ymaxd < p2y + l_width ? ymaxd : p2y + l_width;
			}
			else
			{
				ymind = ymind > p2y - l_width ? ymind : p2y - l_width;
				ymaxd = ymaxd < p1y + l_width ? ymaxd : p1y + l_width;
			}
			xmind = xmind < 0 ? 0 : xmind > this->width ? this->width
														: xmind;
			xmaxd = xmaxd < 0 ? 0 : xmaxd > this->width ? this->width
														: xmaxd;
			ymind = ymind < 0 ? 0 : ymind > this->height ? this->height
														 : ymind;
			ymaxd = ymaxd < 0 ? 0 : ymaxd > this->height ? this->height
														 : ymaxd;

			uint32_t xmin = xmind, xmax = xmaxd, ymin = ymind, ymax = ymaxd;
			for (uint32_t y = ymin; y < ymax; y++)
			{
				vy = (double)p1y - y;
				for (uint32_t x = xmin; x < xmax; x++)
				{
					vx = (double)p1x - x;

					vt = vy * dx - vx * dy;
					vt = abs(vt);
					if (vt > l_width)
					{
						continue;
					}
					else
					{
						vt *= 256 / l_width;
					}
					rgbx[0] = vt;
					rgbx[1] = rgbx[0];
					rgbx[2] = rgbx[0];
					rgbx[3] = rgbx[0];
					pointwrite(x, y, rgbx);
				}
			}
		}
		void draw_line_segment_uncovered(double p1x, double p1y, double p2x, double p2y, double l_width = 1.5)
		{
			double dx = p1x, dy = p1y; // delta
			dx -= p2x, dy -= p2y;
			double dx2 = dx * dx, dy2 = dy * dy;
			double dl = sqrt(dx2 + dy2);
			dx /= dl, dy /= dl;
			uint8_t rgbx[4] = {0, 0, 0, 0};
			double vx = 0, vy = 0, vt = 0;
			double xmind = 0, xmaxd = this->width,
				   ymind = 0, ymaxd = this->height;
			if (p1x < p2x)
			{
				xmind = xmind > p1x - l_width ? xmind : p1x - l_width;
				xmaxd = xmaxd < p2x + l_width ? xmaxd : p2x + l_width;
			}
			else
			{
				xmind = xmind > p2x - l_width ? xmind : p2x - l_width;
				xmaxd = xmaxd < p1x + l_width ? xmaxd : p1x + l_width;
			}

			if (p1y < p2y)
			{
				ymind = ymind > p1y - l_width ? ymind : p1y - l_width;
				ymaxd = ymaxd < p2y + l_width ? ymaxd : p2y + l_width;
			}
			else
			{
				ymind = ymind > p2y - l_width ? ymind : p2y - l_width;
				ymaxd = ymaxd < p1y + l_width ? ymaxd : p1y + l_width;
			}
			xmind = xmind < 0 ? 0 : xmind > this->width ? this->width
														: xmind;
			xmaxd = xmaxd < 0 ? 0 : xmaxd > this->width ? this->width
														: xmaxd;
			ymind = ymind < 0 ? 0 : ymind > this->height ? this->height
														 : ymind;
			ymaxd = ymaxd < 0 ? 0 : ymaxd > this->height ? this->height
														 : ymaxd;

			uint32_t xmin = xmind, xmax = xmaxd, ymin = ymind, ymax = ymaxd;
			uint32_t dlim = xmax, ulim = xmin;
			for (uint32_t y = ymin; y < ymax; y++)
			{
				vy = (double)p1y - y;
				for (uint32_t x = xmin; x < xmax; x++)
				{
					vx = (double)p1x - x;

					vt = vy * dx - vx * dy;
					vt = abs(vt);
					if (vt > l_width)
					{
						continue;
					}

					vt /= l_width;
					pointread(x, y, rgbx);

					rgbx[0] *= vt;
					rgbx[1] = rgbx[0];
					rgbx[2] = rgbx[0];
					rgbx[3] = rgbx[0];
					pointwrite(x, y, rgbx);
				}
			};
		}
		void draw_line_segment_uncovered_o1(double p1x, double p1y, double p2x, double p2y, double l_width = 1.5, uint8_t grayscale = 0)
		{

			double dx = p1x, dy = p1y; // delta
			dx -= p2x, dy -= p2y;
			double dx2 = dx * dx, dy2 = dy * dy;
			double dl = sqrt(dx2 + dy2);
			dx /= dl, dy /= dl;
			uint8_t rgbx[4] = {0, 0, 0, 0};
			double vx = 0, vy = 0, vt = 0;
			double xmind = 0, xmaxd = this->width - 1,
				   ymind = 0, ymaxd = this->height - 1;
			if (p1x < p2x)
			{
				xmind = xmind > p1x - l_width ? xmind : p1x - l_width;
				xmaxd = xmaxd < p2x + l_width ? xmaxd : p2x + l_width;
			}
			else
			{
				xmind = xmind > p2x - l_width ? xmind : p2x - l_width;
				xmaxd = xmaxd < p1x + l_width ? xmaxd : p1x + l_width;
			}

			if (p1y < p2y)
			{
				ymind = ymind > p1y - l_width ? ymind : p1y - l_width;
				ymaxd = ymaxd < p2y + l_width ? ymaxd : p2y + l_width;
			}
			else
			{
				ymind = ymind > p2y - l_width ? ymind : p2y - l_width;
				ymaxd = ymaxd < p1y + l_width ? ymaxd : p1y + l_width;
			}
			xmind = xmind < 0 ? 0 : xmind > this->width - 1 ? this->width - 1
															: xmind;
			xmaxd = xmaxd < 0 ? 0 : xmaxd > this->width - 1 ? this->width - 1
															: xmaxd;
			ymind = ymind < 0 ? 0 : ymind > this->height - 1 ? this->height - 1
															 : ymind;
			ymaxd = ymaxd < 0 ? 0 : ymaxd > this->height - 1 ? this->height - 1
															 : ymaxd;

			int32_t xmin = xmind, xmax = xmaxd, ymin = ymind, ymax = ymaxd;
			bool is_rightup = dx * dy > 0;
			if (dx * dy > 0) // б�����ϣ�x��0��ʼ����y����
			{
				for (int32_t y = ymin; y <= ymax; y++)
				{
					vy = (double)p1y - y;
					uint8_t flag = 0;
					for (int32_t x = xmin; x <= xmax; x++)
					{
						vx = (double)p1x - x;

						vt = vy * dx - vx * dy;
						vt = abs(vt);

						if (vt > l_width) // �ڽ���
						{
							if (flag == 0)
								continue;
							else
								break;
						}
						else if (flag == 0)
						{
							flag = 1;
							xmin = x;
						}

						vt /= l_width;
						vt = (vt - 1) * 255 / (255 - grayscale) + 1;
						pointread(x, y, rgbx);
						rgbx[0] *= vt;
						rgbx[1] = rgbx[0];
						rgbx[2] = rgbx[0];
						rgbx[3] = rgbx[0];
						pointwrite(x, y, rgbx);
					}
				};
			}
			else // б������
			{
				for (int32_t y = ymin; y <= ymax; y++)
				{
					vy = (double)p1y - y;
					uint8_t flag = 0;
					for (int32_t x = xmax; x >= xmin; x--)
					{
						vx = (double)p1x - x;

						vt = vy * dx - vx * dy;
						vt = abs(vt);

						if (vt > l_width) // �ڽ���
						{
							if (flag == 0)
								continue;
							else
								break;
						}
						else if (flag == 0)
						{
							flag = 1;
							xmax = x;
						}

						vt /= l_width;
						pointread(x, y, rgbx);
						rgbx[0] *= vt;
						rgbx[1] = rgbx[0];
						rgbx[2] = rgbx[0];
						rgbx[3] = rgbx[0];
						pointwrite(x, y, rgbx);
					}
				};
			}
		}
		void draw_solid_circle(double px, double py, double r = 0, double edge = 1.5)
		{
			double lim = r + edge + 1;
			double xmind = px - lim, xmaxd = px + lim,
				   ymind = py - lim, ymaxd = py + lim;
			xmind = xmind < 0 ? 0 : xmind > this->width ? this->width
														: xmind;
			xmaxd = xmaxd < 0 ? 0 : xmaxd > this->width ? this->width
														: xmaxd;
			ymind = ymind < 0 ? 0 : ymind > this->height ? this->height
														 : ymind;
			ymaxd = ymaxd < 0 ? 0 : ymaxd > this->height ? this->height
														 : ymaxd;

			lim -= 1;
			uint32_t xmin = xmind, xmax = xmaxd;
			uint32_t ymin = ymind, ymax = ymaxd;
			uint8_t rgbx[4] = {};
			for (uint32_t x = xmin; x < xmax; x++)
			{
				for (uint32_t y = ymin; y < ymax; y++)
				{
					double dx = px - x, dy = py - y;
					double dl = sqrt(dx * dx + dy * dy);
					double color = 0;
					if (dl > lim)
					{
						continue;
					}
					else if (dl > r)
					{
						color = (dl - r) / edge * 256;
					}
					rgbx[0] = floor(color);
					rgbx[1] = rgbx[0];
					rgbx[2] = rgbx[0];
					rgbx[3] = rgbx[0];
					pointwrite(x, y, rgbx);
				}
			}
		}
		void draw_line_segment_pointed(double p1x, double p1y, double p2x, double p2y, double l_width = 1.5)
		{
			draw_line_segment(p1x, p1y, p2x, p2y, l_width);
			draw_solid_circle(p1x, p1y, l_width / 2, l_width);
			draw_solid_circle(p2x, p2y, l_width / 2, l_width);
		}
		void draw_line_segment_pointed_uncovered(double p1x, double p1y, double p2x, double p2y, double l_width = 1.5)
		{
			draw_line_segment_uncovered(p1x, p1y, p2x, p2y, l_width);
			draw_solid_circle(p1x, p1y, l_width / 2, l_width);
			draw_solid_circle(p2x, p2y, l_width / 2, l_width);
		}
	};
	inline void bmp_mixup_pixel(bmp &base, bmp &add, uint32_t x, uint32_t y, float base_percentage)
	{
		uint8_t rgbbase[4] = {0}, rgbadd[4] = {0};
		base.pointread(x, y, rgbbase);
		add.pointread(x, y, rgbadd);
		for (int i = 0; i < 4; i++)
		{
			rgbbase[i] = rgbbase[i] * base_percentage + rgbadd[i] * (1 - base_percentage);
		}
		base.pointwrite(x, y, rgbbase);
		return;
	}
	void bmp_mixup_block(bmp &base, bmp &add, uint32_t x, uint32_t y, uint32_t wid, uint32_t hei, float base_percentage)
	{
		uint32_t xw = x + wid, yh = y + hei;
		if (xw > base.width)
			xw = base.width;
		if (yh > base.height)
			yh = base.height;
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		for (int i = x; i < xw; i++)
		{
			for (int j = y; j < yh; j++)
			{
				bmp_mixup_pixel(base, add, i, j, base_percentage);
			}
		}
		return;
	}

	// color in rgb[3]
	bmp *bmp_mixup_color(bmp &base)
	{
		bmphd bhd;
		bhd.init(base.width, base.height, 24);
		bmp *ret = new bmp(bhd);
		uint8_t color[4] = {0};
		for (int i = 0; i < 3; i++)
		{
			color[i] = base.rgb[i];
		}
		for (int i = 0; i < base.width; i++)
		{
			for (int j = 0; j < base.height; j++)
			{
				base.pointread(i, j);
				for (int p = 0; p < 3; p++)
				{
					ret->rgb[p] = (float)base.rgb[p] * (255 - base.rgb[3]) / 255 + color[p] * base.rgb[3] / 255;
				}
				ret->pointwrite(i, j);
			}
		}
		return ret;
	}

	bmp *mix_funcal(bmp *base, bmp *another, void (*func)(bmp *b, bmp *a))
	{

		for (uint32_t y = 0; y < base->height; y++)
		{
			for (uint32_t x = 0; x < base->width; x++)
			{
				base->pointread(x, y);
				another->pointread(x, y);
				base->x = x;
				base->y = y;
				func(base, another);
				base->pointwrite(x, y);
			}
		}
		return base;
	}
#ifndef _WINGDI_
#include <Windows.h>
#include <wingdi.h>
#endif
#ifdef _WINGDI_
	// show bmp in window
	int show_bmp(bmp *mempic, HWND hwnd = NULL)
	{
		static HWND hwnds = NULL;
		if (hwnd == NULL)
		{
			if (hwnds == NULL)
			{
				// create a window
				hwnds = CreateWindowA("static", "bmp", WS_OVERLAPPEDWINDOW, 0, 0, mempic->width, mempic->height, NULL, NULL, NULL, NULL);
				hwnd = hwnds;
			}
			else
			{
				hwnd = hwnds;
				
			}
		}
		HDC hdc = GetDC(hwnd);
		HDC memdc = CreateCompatibleDC(hdc);
		HBITMAP hbitmap = CreateCompatibleBitmap(hdc, mempic->width, mempic->height);
		SelectObject(memdc, hbitmap);
		SetDIBits(memdc, hbitmap, 0, mempic->height, mempic->data, (BITMAPINFO *)(mempic + sizeof(BITMAPFILEHEADER)), DIB_RGB_COLORS);
		BitBlt(hdc, 0, 0, mempic->width, mempic->height, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);
		DeleteObject(hbitmap);
		ReleaseDC(hwnd, hdc);
		return 0;
	}
	bool show_bmp(uint8_t *mempic, HWND hwnd = NULL)
	{

		HDC hdc = GetDC(hwnd);
		RECT rect;
		if (hwnd == NULL)
		{
			GetClientRect(GetDesktopWindow(), &rect);
		}
		else
		{
			GetClientRect(hwnd, &rect);
		}
		HDC memdc = CreateCompatibleDC(hdc);
		HBITMAP hbitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		HGDIOBJ oldobj = SelectObject(memdc, hbitmap);
		BITMAPFILEHEADER *bfh = (BITMAPFILEHEADER *)mempic;
		BITMAPINFOHEADER *bih = (BITMAPINFOHEADER *)(mempic + sizeof(BITMAPFILEHEADER));
		RGBQUAD *palette = (RGBQUAD *)(mempic + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
		BYTE *bits = (BYTE *)(mempic + bfh->bfOffBits);
		SetDIBits(memdc, hbitmap, 0, bih->biHeight, bits, (BITMAPINFO *)bih, DIB_RGB_COLORS);

		BitBlt(hdc, rect.left, rect.top, rect.right - rect.left,
			   rect.bottom - rect.top,
			   memdc, 0, 0,
			   SRCCOPY);
		SelectObject(memdc, oldobj);
		DeleteObject(hbitmap);
		DeleteDC(memdc);
		ReleaseDC(NULL, hdc);
		return TRUE;
	};

#endif // _WINGDI_
#endif // !_bionukg_graphics_h
#ifdef namespace_bionukg
};
#endif // namespace_bionukg
