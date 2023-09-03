

#ifdef _WINGDI_
namespace bmpstruct {
	// ����λͼ�ļ�ͷ��Ϣ�ṹ��
	typedef struct tagBITMAPFILEHEADER {
		WORD bfType; //�ļ����ͣ� ����ΪBM
		DWORD bfSize; //�ļ���С�����ֽ�Ϊ��λ
		WORD bfReserved1; //�����֣�����Ϊ0
		WORD bfReserved2; //  �����֣�����Ϊ0
		WORD bfOffBits; //���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���������ֽ�Ϊ��λ
	}BITMAPFILEHEADER;

	// ����λͼ��Ϣͷ�ṹ��
	typedef struct tagBITMAPINFOHEADER {
		DWORD biSize; //�ṹ���С�����ֽ�Ϊ��λ
		LONG biWidth; //λͼ���ȣ�������Ϊ��λ
		LONG biHeight; //λͼ�߶ȣ�������Ϊ��λ
		WORD biplanes; //Ŀ���豸�ļ��𣬱���Ϊ1
		WORD biBitCount;//ÿ�����������λ����������1��4�� 8�� 16��24��32֮ -
		DWORD biCompression;// λͼѹ�����ͣ�������0(��ѹ��)��1(BI_ RLE8ѹ������)��2(BI_ RLE4ѹ������)֮һ
		DWORD bisizeImage; //λͼ��С�����ֽ�Ϊ��λ
		LONG biXPelsPerMeter; //λͼˮƽ�ֱ��ʣ�ÿ��������
		LONG biYPelsPerMeter; // λͼ��ֱ�ֱ��ʣ�ÿ��������
		DWORD biClrUsed;// λͼʵ��ʹ�õ���ɫ���е���ɫ��
		DWORD biClrImportant;// λͼ��ʾ��������Ҫ����ɫ��
	}BITMAPINFOHEADER;
	//����RGB��ɫ�ṹ��
	typedef struct tagRGBQUAD {
		BYTE rgbBlue;// ��ɫ����
		BYTE rgbGreen;// ��ɫ����
		BYTE rgbRed;// ��ɫ����
		BYTE rgbReserved;// ����ֵ
	}RGBQUAD;
}
// ��ʾbmpͼƬ�ĺ���
			// ����: void* pic ���ڴ��е�bmpͼƬ���ݣ�
			// ����ֵBOOL��ʾ�Ƿ�ɹ���ʾͼƬ��
bool show_bmp(uint8_t* mempic,HWND hwnd=NULL)
{
	// ��ȡ��Ļ�豸��������Ϳͻ������δ�С��
	HDC hdc = GetDC(hwnd);
	RECT rect;
	if (hwnd == NULL) {
		GetClientRect(GetDesktopWindow(), &rect);
	}
	else
	{
		GetClientRect(hwnd, &rect);
	}
	// ����һ�����ݵ��豸��������ͼ��ݵ�λͼ����
	HDC memdc = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	//�����ݵ�λͼ����ѡ����ݵ��豸�����С�
	HGDIOBJ oldobj = SelectObject(memdc, hbitmap);
	//���ڴ��е�bmpͼƬ���ݸ��Ƶ����ݵ��豸�����С�
	BITMAPFILEHEADER* bfh = (BITMAPFILEHEADER*)mempic;
	BITMAPINFOHEADER* bih = (BITMAPINFOHEADER*)(mempic + sizeof(BITMAPFILEHEADER));
	RGBQUAD* palette = (RGBQUAD*)(mempic + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	BYTE* bits = (BYTE*)(mempic + bfh->bfOffBits);
	SetDIBits(memdc, hbitmap, 0, bih->biHeight, bits, (BITMAPINFO*)bih, DIB_RGB_COLORS);

	//�����ݵ��豸�����е�ͼƬ���Ƶ���Ļ�ϡ�
	BitBlt(hdc, rect.left, rect.top, rect.right - rect.left,
		rect.bottom - rect.top,
		memdc, 0, 0,
		SRCCOPY);
	//�ͷ���Դ��
	SelectObject(memdc, oldobj);
	DeleteObject(hbitmap);
	DeleteDC(memdc);
	ReleaseDC(NULL, hdc);
	return TRUE;
};

#endif // _WINGDI_

#ifdef namespace_bionukg
};
#endif //namespace_bionukg
