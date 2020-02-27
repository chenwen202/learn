/******************************************************************************
	COPYRIGHTS (C), 2006-2008, PeopleSpot TECHNOLOGY.

	FILE NAME:	dib.h
	AUTHOR:		Xie Yonghui
	VERSION:	2.0
	DATE:		08-18-2006
	COMMENTS:	General Purpose DIB Class.
	REVISION:	
*******************************************************************************/
#include <math.h>
#include "Dib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDib::CDib()
{
	InitNull();
}

CDib::CDib(CFile* pFile)
{
	InitNull();

	Read(pFile);
}

CDib::CDib(CSize &DibSize, int nBt)//nBt default is 8
{
	Create(DibSize.cx,DibSize.cy,nBt);
}

CDib::CDib(unsigned int dwWidth, 
		   unsigned int dwHeight, 
		   int nBt)
{
	Create(dwWidth,dwHeight,nBt);
/*	CSize dibSize;
	dibSize.cx = dwWidth;
	dibSize.cy = dwHeight;
	CDib(dibSize,nBt);//这样是有问题的，想想这是为什么*/
}

CDib::CDib(char *strFileName)
{
	DWORD result;
	CFile rdFile;

	InitNull();
	
	result = rdFile.Open((LPCTSTR)strFileName, CFile::modeRead | CFile::typeBinary);
	if (!result) return ;
	
	result = static_cast<DWORD>(rdFile.GetLength());
	m_dwLength = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	
	//added by xieyhfirst 2004.05.28
	//加入这个判断是为了防止读入空的文件，从而导致异常
	//空的文件比如用右键新建一个BMP文件
	if (0 == result)
	{
		return;
	}

	if (result < m_dwLength || 
		!(AllocateMemory())) {
		return ;
	}
	result = rdFile.Read(m_lpBuf, m_dwLength);
	if (result != m_dwLength) {
		return ;
	}
	if (m_lpBMFH -> bfType != 0x4d42) {	// 'BM'
		return ;
	}
	
	int	nPaletteSize = CalPaletteSize(m_lpBMIH -> biBitCount);
	int nColorMasks = 0;
	if (m_lpBMIH -> biCompression == BI_BITFIELDS)
		nColorMasks = 3 * sizeof(DWORD);

/*	DWORD dwBytes = (m_lpBMIH -> biWidth * 
		m_lpBMIH -> biBitCount + 31) / 32 * 4;*/

	DWORD dwBytes = WIDTHBYTES(m_lpBMIH -> biWidth * m_lpBMIH -> biBitCount);
	
	int nOtherLength = nPaletteSize + nColorMasks +	dwBytes * m_lpBMIH -> biHeight;
	m_dwLength += nOtherLength;
	if (!(AllocateMemory(TRUE))) {
		return ;
	}
	
	switch (m_lpBMIH -> biCompression) {
	case BI_RLE4:
		rdFile.Read((char*)(m_lpColors), nPaletteSize);
		result = Decode_RLE4(rdFile, m_lpData, dwBytes, m_lpBMIH -> biHeight);
		m_lpBMIH -> biCompression = BI_RGB;
		break;
	case BI_RLE8:
		rdFile.Read((char*)(m_lpColors), nPaletteSize);
		result = Decode_RLE8(rdFile, m_lpData, dwBytes, m_lpBMIH -> biHeight);
		m_lpBMIH -> biCompression = BI_RGB;
		break;
	case BI_BITFIELDS:
	case BI_RGB:
		rdFile.Read((char*)(m_lpColors), nOtherLength);
		break;
	default:
		break;
	}
	
	m_nBits = m_lpBMIH -> biBitCount;
	m_bInit = TRUE;
}

CDib::CDib(CString strFileName)
{
	DWORD result;
	CFile rdFile;
	
	result = rdFile.Open(strFileName, CFile::modeRead | CFile::typeBinary);
	if (!result) return ;
	
	result = static_cast<DWORD>(rdFile.GetLength());
	m_dwLength = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	
	if (result < m_dwLength || 
		!(AllocateMemory())) {
		return ;
	}
	result = rdFile.Read(m_lpBuf, m_dwLength);
	if (result != m_dwLength) {
		return ;
	}
	if (m_lpBMFH -> bfType != 0x4d42) {	// 'BM'
		return ;
	}

	int	nPaletteSize = CalPaletteSize(m_lpBMIH -> biBitCount);
	int nColorMasks = 0;
	if (m_lpBMIH -> biCompression == BI_BITFIELDS)
		nColorMasks = 3 * sizeof(DWORD);
	
/*	DWORD dwBytes = (m_lpBMIH -> biWidth * 
			m_lpBMIH -> biBitCount + 31) / 32 * 4;*/
		
	DWORD dwBytes = WIDTHBYTES(m_lpBMIH -> biWidth * m_lpBMIH -> biBitCount);
	
	int nOtherLength = nPaletteSize +
		nColorMasks +
		dwBytes * m_lpBMIH -> biHeight;
	m_dwLength += nOtherLength;
	if (!(AllocateMemory(TRUE))) {
		return ;
	}
	
	switch (m_lpBMIH -> biCompression) {
	case BI_RLE4:
		rdFile.Read((char*)(m_lpColors), nPaletteSize);
		result = Decode_RLE4(rdFile, m_lpData, dwBytes, m_lpBMIH -> biHeight);
		m_lpBMIH -> biCompression = BI_RGB;
		break;
	case BI_RLE8:
		rdFile.Read((char*)(m_lpColors), nPaletteSize);
		result = Decode_RLE8(rdFile, m_lpData, dwBytes, m_lpBMIH -> biHeight);
		m_lpBMIH -> biCompression = BI_RGB;
		break;
	case BI_BITFIELDS:
	case BI_RGB:
		rdFile.Read((char*)(m_lpColors), nOtherLength);
		break;
	default:
		break;
	}
	
	m_nBits = m_lpBMIH -> biBitCount;
	m_bInit = TRUE;
}

CDib::CDib(CDC* pDC, int nBt, BOOL bCompr)
//Constructs a DIB from the contents of a bitmap 
//pDC:		a memory DC ptr
//nBt:		color bits per pixel (default = 0)
//bCompr:	compression (default = TRUE)
{
	BITMAP	bm;
	int		nPaletteSize;

	InitNull();

	CBitmap* pEmptyBitmap = new CBitmap;
	pEmptyBitmap -> CreateCompatibleBitmap(pDC, 0, 0);
	CBitmap* pBitmap = (CBitmap*) (pDC -> SelectObject(pEmptyBitmap));
	pBitmap -> GetObject(sizeof(bm), &bm);

	if ((nBt == 1) || (nBt == 4) || (nBt == 8) || (nBt == 24)) {
		m_nBits = nBt;
	}
	else {	//nBt = 0
		m_nBits = bm.bmPlanes * bm.bmBitsPixel;	//color bits per pixel
	}

	nPaletteSize = CalPaletteSize(m_nBits);

	//fills out row to 4-byte boundary
	DWORD dwBytes = WIDTHBYTES(bm.bmWidth * m_nBits);
	
/*
	DWORD dwBytes = ((DWORD) bm.bmWidth * m_nBits) / 32;
	if (((DWORD) bm.bmWidth * m_nBits) % 32) 
		dwBytes ++;
	dwBytes *= 4;*/


	m_dwLength = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
				 /*sizeof(RGBQUAD) **/ nPaletteSize;
	if (!AllocateMemory()) return;

	m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
	m_lpBMIH->biWidth = bm.bmWidth;
	m_lpBMIH->biHeight = bm.bmHeight;
	m_lpBMIH->biPlanes = 1;
	m_lpBMIH->biBitCount = m_nBits;		//1, 4, 8, or 24

	if (bCompr && m_nBits == 4) {
		m_lpBMIH->biCompression = BI_RLE4;
	}
	else if (bCompr && m_nBits == 8) {
		m_lpBMIH->biCompression = BI_RLE8;
	}
	else 
		m_lpBMIH->biCompression = BI_RGB;

	m_lpBMIH->biSizeImage = 0;
	m_lpBMIH->biXPelsPerMeter = 0;
	m_lpBMIH->biYPelsPerMeter = 0;
	m_lpBMIH->biClrUsed = 0;
	m_lpBMIH->biClrImportant = 0;

	//calls GetDIBits with null data pointer to get size of DIB
	::GetDIBits(pDC->GetSafeHdc(), (HBITMAP)pBitmap->GetSafeHandle(),
				0, (WORD)bm.bmHeight, NULL, m_lpBMI, DIB_RGB_COLORS);

	if (m_lpBMIH->biSizeImage == 0) {
		m_dwLength += dwBytes * bm.bmHeight;
		m_lpBMIH->biCompression = BI_RGB;
	}
	else {
		m_dwLength += m_lpBMIH->biSizeImage;
	}

	if (!AllocateMemory(TRUE)) return;

	m_lpData = (BYTE*)m_lpBMIH + sizeof(BITMAPINFOHEADER) +
		/*sizeof(RGBQUAD) * */nPaletteSize;
	m_lpBMFH->bfType = 0x4d42;	//'BM'
	m_lpBMFH->bfSize = m_dwLength;
	m_lpBMFH->bfReserved1 = 0;
	m_lpBMFH->bfReserved2 = 0;
	m_lpBMFH->bfOffBits = (BYTE*)m_lpData - m_lpBuf;

	//second GetDIBits call to make DIB
	if (!::GetDIBits(pDC->GetSafeHdc(), (HBITMAP)
		pBitmap->GetSafeHandle(), 0, (WORD)bm.bmHeight, m_lpData,
		m_lpBMI, DIB_RGB_COLORS))
		m_dwLength = 0L;

	delete pDC->SelectObject(pBitmap);

	m_bInit = TRUE;
}

CDib::~CDib()
{
	if (m_lpBuf) 
	{
		free(m_lpBuf);		//free the DIB memory
		m_lpBuf = 0;
	}
}

void CDib::Copy(const CDib* pDibSrc)
{
//added 0622 for doc->m_pdibresult
	if (m_lpBuf)
	{
		free(m_lpBuf);		//free the DIB memory
		m_lpBuf = 0;		
	}

	m_dwLength = pDibSrc->m_dwLength;
	if (!AllocateMemory()) return;
	
	memcpy(m_lpBuf,pDibSrc->m_lpBuf,m_dwLength);
	//added by xieyhfirst 040916 
	if(m_lpBMFH->bfSize != m_dwLength)
	{	//for Gif format file when copy its source the filehead may fail.
		m_lpBMFH->bfType = 0x4d42;
		m_lpBMFH->bfSize = m_dwLength;
		m_lpBMFH->bfReserved1 = 0;
		m_lpBMFH->bfReserved1 = 0;
		m_lpBMFH->bfOffBits = 1078;
	}

	m_lpData = (BYTE*) m_lpBMFH + m_lpBMFH->bfOffBits;
	m_nBits = m_lpBMIH->biBitCount;
	
	m_bInit = TRUE;
}

/******************************************************************************
	NAME:		Create()
	PARAMS:		DWORD dwWidth,	Width of Dib to be created;
				DWORD dwHeight, Height of Dib to be created;
				int	nBt,		Bit count of dib.
	RETURNS:	NULL, NOT SUCCESS; 
				BYTE*, Pointer to the DIB block.
	COMMENTS:	Create a dib,whose data is inited 0.
	REFERENCE:	None.
*******************************************************************************/
BYTE* CDib::Create(DWORD dwWidth, DWORD dwHeight, int nBt)
{
	InitNull();
	m_nBits = nBt;
	BOOL bCompr = FALSE;
	m_dwLength = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	int	nPaletteSize = CalPaletteSize(nBt);

	DWORD dwBytes = WIDTHBYTES(dwWidth * m_nBits);
	
//	DWORD dwBytes = (dwWidth * m_nBits + 31) / 32 * 4;
	//切记调色板大小后要乘以sizeof(RGBQUAD)，这里害得我找了大半天才搞定这个bug
	//最好的办法用函数替代,避免犯这种错误.
	int nOtherLength = nPaletteSize/* * sizeof(RGBQUAD) + nColorMasks*/ + dwBytes * dwHeight;
	m_dwLength += nOtherLength;
	if (!(AllocateMemory(/*TRUE*/)))
	{
		return NULL;
	}

	m_lpBMFH->bfType = 0x4d42;	//'BM'
	m_lpBMFH->bfSize = m_dwLength;
	m_lpBMFH->bfReserved1 = 0;
	m_lpBMFH->bfReserved2 = 0;
	//Note:这里m_lpData在开辟内存时并没有赋值/或者赋值不正确，
	//因此这个值是不可信的，所以不能用来给其他的参数赋值。
	//可以用文件头、信息头和调色板来求得偏移量。
	//	m_lpBMFH->bfOffBits = (BYTE*)m_lpData - m_lpBuf;
	m_lpBMFH->bfOffBits = sizeof(BITMAPFILEHEADER) 
						  + sizeof(BITMAPINFOHEADER)
						  + nPaletteSize /** sizeof(RGBQUAD)*/;
	
	m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
	m_lpBMIH->biWidth = dwWidth;
	m_lpBMIH->biHeight = dwHeight;
	m_lpBMIH->biPlanes = 1;
	m_lpBMIH->biBitCount = m_nBits;		//1, 4, 8, or 24
	
	if (bCompr && m_nBits == 4) {
		m_lpBMIH->biCompression = BI_RLE4;
	}
	else if (bCompr && m_nBits == 8) {
		m_lpBMIH->biCompression = BI_RLE8;
	}
	else 
		m_lpBMIH->biCompression = BI_RGB;
	
	m_lpBMIH->biSizeImage = 0;
	m_lpBMIH->biXPelsPerMeter = 0;
	m_lpBMIH->biYPelsPerMeter = 0;
	m_lpBMIH->biClrUsed = 0;
	m_lpBMIH->biClrImportant = 0;

	if (!(AllocateMemory(TRUE)))
	{
		return NULL;
	}

	//clear the palette
	RGBQUAD* pal=GetPalette();
	if (pal) memset(pal,0,GetPaletteSize());

	memset(m_lpData,(BYTE)0,dwBytes * dwHeight);
	//如果没有赋值，则看不到图像，因为显示区域为0，0，详细请看GetWidth&GetHeight	
	//这个问题弄我好长时间哦，终于搞定是什么原因啦，呵呵
	m_bInit = TRUE;
	return m_lpBuf;
}

//////////////////////////////////////////////////////////////////////
BOOL CDib::Read(CFile* pfile)
{
	//file assumed to be open
	//modified by xieyhfirst 050306
	if (m_lpBuf) {
		free(m_lpBuf);
		InitNull();
	}

	m_dwLength = static_cast<DWORD>(pfile->GetLength());
//	if (!AllocateMemory(TRUE)) 
	if (!AllocateMemory()) 
		return FALSE;
	DWORD dwCount = pfile->Read(m_lpBuf, m_dwLength);
	if (dwCount != m_dwLength) {
		free(m_lpBuf);		//free the DIB memory
		m_lpBuf = NULL;
		return FALSE;
	}
	if (m_lpBMFH->bfType != 0x4d42) {
		free(m_lpBuf);		//free the DIB memory
		m_lpBuf = NULL;
		return FALSE;
	}

	if (m_lpBMIH->biBitCount != 1 && m_lpBMIH->biBitCount != 4 &&
		m_lpBMIH->biBitCount != 8 && m_lpBMIH->biBitCount != 24 ) {
		return FALSE;
	}

	m_lpData = (BYTE*) m_lpBMFH + m_lpBMFH->bfOffBits;
	m_nBits = m_lpBMIH->biBitCount;

	m_bInit = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
BOOL CDib::Write(CFile* pfile)
{
	TRY {
		pfile->Write(m_lpBuf, m_dwLength);
	}
	CATCH (CException, e) {
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
CBitmap* CDib::MakeBitmap(CDC* pDC, CSize& bmSize)
{
	//replaces the DC's existing bitmap with a new one from the DIB
	//returns the old one
	BITMAP bm;
	DWORD dwFore, dwBack;

	//checks to see whether DIB buffer is properly loaded
	if (m_dwLength == 0L) {
		bmSize.cx = bmSize.cy = 0;
		return NULL;
	}

	//this code conditions the bitmap for mono or color
	int nPlanes = pDC->GetDeviceCaps(PLANES);
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	CBitmap* pConfigBitmap = new CBitmap;
	char bits[100];
	if (m_lpBMIH->biBitCount == 1) {
		pConfigBitmap->CreateBitmap(1,1,1,1,bits);
	}
	else {
		pConfigBitmap->CreateBitmap(1,1,nPlanes, nBitsPixel, bits);
	}
	CBitmap* pOriginalBitmap = 
		(CBitmap*)pDC->SelectObject(pConfigBitmap);

	//CreateDIBitmap "switches bits" for mono bitmaps, depending on colors
	//So we'll fool it
	if (GetMonoColors(dwFore, dwBack)) {
		SetMonoColors(0L, 0xFFFFFFL);
	}

#ifdef _WIN32
	HBITMAP hBitmap = ::CreateDIBitmap(pDC->GetSafeHdc(), m_lpBMIH,
		CBM_INIT, (CONST BYTE*)(m_lpBuf + m_lpBMFH->bfOffBits),
		m_lpBMI, DIB_RGB_COLORS);
#else
	HBITMAP hBitmap = ::CreateDIBtmap(pDC->GetSafeHdc(), m_lpBMIH,
		CBM_INIT, (CONST BYTE*)(m_lpBuf + m_lpBMFH->bfOffBits),
		m_lpBMI, DIB_RGB_COLORS);
#endif
	if (hBitmap == NULL) {
		delete pDC->SelectObject(pOriginalBitmap);	//del config bitmap
		return NULL;		//untested error logic
	}

	SetMonoColors(dwFore, dwBack);

	//Can't use CBitmap::FromHandle here because we need to delete
	//the object later
	CBitmap* pBitmap = new CBitmap;
	pBitmap->Attach(hBitmap);
	pBitmap->GetObject(sizeof(bm), &bm);
	bmSize.cx = bm.bmWidth;
	bmSize.cy = bm.bmHeight;
	delete pDC->SelectObject(pBitmap);	//delete configuration bitmap

	return pOriginalBitmap;
}

//////////////////////////////////////////////////////////////////////
BOOL CDib::Display(CDC* pDC, CPoint origin)
{
	//direct to device--bypass the GDI bitmap
	if (!m_bInit) {
		return FALSE;	//nothing to display
	}
	if (!::SetDIBitsToDevice(pDC->GetSafeHdc(), origin.x, origin.y,
		(WORD)m_lpBMIH->biWidth, (WORD)m_lpBMIH->biHeight, 0, 0, 0,
		(WORD)m_lpBMIH->biHeight, m_lpData, m_lpBMI,
		DIB_RGB_COLORS)) {
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
BOOL CDib::Stretch(CDC *pDC, CPoint origin, CSize size)
{
	//direct to device--bypass the GDI bitmap
	if (!m_bInit) {
		return FALSE;	//nothinhg to display
	}
	::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
	if (!::StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y,
		size.cx, size.cy, 0, 0, (WORD)m_lpBMIH->biWidth,
		(WORD)m_lpBMIH->biHeight, m_lpData, m_lpBMI,
		DIB_RGB_COLORS, SRCCOPY)) {
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
int CDib::GetColorBits()
{
	return m_nBits;
}

//////////////////////////////////////////////////////////////////////
DWORD CDib::GetLength()
{
	return m_dwLength;
}

//////////////////////////////////////////////////////////////////////
void CDib::SetMonoColors(DWORD dwForeground, DWORD dwBackground)
{
	if (m_nBits != 1) {
		return;
	}
	DWORD* pPalette = (DWORD*)
		((BYTE*)m_lpBMIH + sizeof(BITMAPINFOHEADER));
	*pPalette = dwForeground;
	*(++pPalette) = dwBackground;
	return;
}

//////////////////////////////////////////////////////////////////////
BOOL CDib::GetMonoColors(DWORD& dwForeground, DWORD& dwBackground)
{
	if (m_nBits != 1) {
		return FALSE;
	}
	DWORD* pPalette = (DWORD*)
		((BYTE*)m_lpBMIH + sizeof(BITMAPINFOHEADER));
	dwForeground = *pPalette;
	dwBackground = *(++pPalette);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
BOOL CDib::AllocateMemory(BOOL bRealloc)	//bRealloc default = FALSE
{
	BYTE* ptr = NULL;

	if (!bRealloc) {
		ptr = (BYTE*)malloc(m_dwLength);
	}
	else {
		ptr = (BYTE*)realloc(m_lpBuf, m_dwLength);
	}

	if (!ptr) {
		if (bRealloc) free(m_lpBuf);
		InitNull();
		return FALSE;
	}
	m_lpBuf = ptr;
	
	m_lpBMFH = (LPBITMAPFILEHEADER)m_lpBuf;
	m_lpBMIH = (LPBITMAPINFOHEADER)(m_lpBuf + sizeof(BITMAPFILEHEADER));
	m_lpBMI = (LPBITMAPINFO)m_lpBMIH;
	m_lpColors = (RGBQUAD*)(m_lpBuf + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	//noted by xieyhfirst 040603: 这里在初次分配内存时，可能不妥，因为初次分配时m_lpBMFH是一个任意的初始值
	//因此m_lpData 指向的位置不对，从而可能导致问题。
	//个人认为在内部使用的m_lpBMFH，m_lpBMIH，m_lpBMI几个参数不用指针也许更合适
	if (bRealloc) 
	{//改为只有重分配内存时，才指定位置，因为此时m_lpBMFH已经赋值了。
		m_lpData = (BYTE*)m_lpBMFH + m_lpBMFH -> bfOffBits;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
BOOL CDib::MakeBMPFromRaw(BYTE* lpRawData, int Width, int Height, 
						  int Type)
{
	//modified by xieyhfirst 040607
	//加入对1位4位bmp的支持。
	int nPaletteSize;
	
	if (m_lpBuf) {
		free(m_lpBuf);
		InitNull();
	}	
	
	if (Type == 1 || Type == 4 || Type == 8 || Type == 24)
		m_nBits = Type;
	else
		return FALSE;
	
	nPaletteSize = CalPaletteSize(m_nBits)/4;
	
	DWORD dwBytes = (Width * m_nBits + 31) / 32 * 4;
	
	m_dwLength = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
		sizeof(RGBQUAD) * nPaletteSize + dwBytes * Height;
	if (!AllocateMemory())
		return FALSE;
	
	m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
	m_lpBMIH->biWidth = Width;
	m_lpBMIH->biHeight = Height;
	m_lpBMIH->biPlanes = 1;
	m_lpBMIH->biBitCount = m_nBits;		//1, 4, 8, or 24
	m_lpBMIH->biCompression = BI_RGB;
	m_lpBMIH->biSizeImage = 0;
	m_lpBMIH->biXPelsPerMeter = 0;
	m_lpBMIH->biYPelsPerMeter = 0;
	m_lpBMIH->biClrUsed = 0;
	m_lpBMIH->biClrImportant = 0;
	
	//	m_lpData = (BYTE*)m_lpBuf + sizeof(BITMAPFILEHEADER) 
	//						  + sizeof(BITMAPINFOHEADER)
	//						  + nPaletteSize * sizeof(RGBQUAD);
	m_lpData = (BYTE*)m_lpBMIH + sizeof(BITMAPINFOHEADER) +
		sizeof(RGBQUAD) * nPaletteSize;
	
	m_lpBMFH->bfType = 0x4d42;	//'BM'
	m_lpBMFH->bfSize = m_dwLength;
	m_lpBMFH->bfReserved1 = 0;
	m_lpBMFH->bfReserved2 = 0;
	m_lpBMFH->bfOffBits = (BYTE*)m_lpData - m_lpBuf;
	
	// Palette
	BYTE* lpPalette = (BYTE*)m_lpBMIH + sizeof(BITMAPINFOHEADER);
	if (m_nBits == 1) 
	{
		for (int i = 0; i < nPaletteSize; i ++) {
			lpPalette[4 * i] = (BYTE)i*255;
			lpPalette[4 * i + 1] = (BYTE)i*255;
			lpPalette[4 * i + 2] = (BYTE)i*255;
			lpPalette[4 * i + 3] = (BYTE)i*255;
		}
	}	
	/*	if (m_nBits == 4) 
	{
	for (int i = 0; i < nPaletteSize; i ++) {
	lpPalette[4 * i] = (BYTE)i;
	lpPalette[4 * i + 1] = (BYTE)i;
	lpPalette[4 * i + 2] = (BYTE)i;
	lpPalette[4 * i + 3] = (BYTE)i;
	}
}*/
	if (m_nBits == 8) 
	{
		for (int i = 0; i < nPaletteSize; i ++) {
			lpPalette[4 * i] = (BYTE)i;
			lpPalette[4 * i + 1] = (BYTE)i;
			lpPalette[4 * i + 2] = (BYTE)i;
			lpPalette[4 * i + 3] = (BYTE)i;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (dwBytes == Width)
	{	//for alignment
//		if (m_nBits == 8)
//		{
//			FlipData(lpRawData,dwBytes,Height);
//			memcpy(m_lpData,lpRawData,dwBytes * Height);
//			FlipData(lpRawData,dwBytes,Height);
//		}
//		else
//		{
//			memcpy(m_lpData,lpRawData,dwBytes * Height);
//		}
		int i = 0,j = 0,k = 0;
		memset(m_lpData,0,dwBytes*Height);
		for (i = Height-1, k = 0; i >= 0; i --, k ++)
		{
			for (j = 0; j < Width; j ++) 
			{
				m_lpData[k*dwBytes + j] = lpRawData[i*Width + j];
			}
		}
	}
	else
	{	//for not alignment
		memset(m_lpData,0,dwBytes*Height);
		if (m_nBits == 8)
		{
			int i = 0,j = 0,k = 0;
			for (i = Height-1, k = 0; i >= 0; i --, k ++)
			{
				for (j = 0; j < Width; j ++) 
				{
					m_lpData[k*dwBytes + j] = lpRawData[i*Width + j];
				}
			}
		}
		else
		{
			memcpy(m_lpData,lpRawData,Width * Height);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
	
//	memcpy(m_lpData,lpRawData,dwBytes * Height);
	//modified by xieyhfirst 040915
/*	if (dwBytes == Width)
	{	//for alignment
		if (m_nBits <= 8) 
		{//反转数据
			FlipData(lpRawData,dwBytes, Height);
		}
		memcpy(m_lpData,lpRawData,dwBytes * Height);
		//modified by xieyhfirst 040830 
		// mingxing liu find this bug
		if (m_nBits <= 8) 
		{//反转数据
			FlipData(lpRawData,dwBytes, Height);
		}
	}
	else
	{	//for not alignment
		if (m_nBits == 8)
		{
			int i = 0,j = 0,k = 0;
			memset(m_lpData,0,dwBytes*Height);
			for (i = Height-1, k = 0; i >= 0; i --, k ++)
			{
				for (j = 0; j < Width; j ++) 
				{
					m_lpData[k*dwBytes + j] = lpRawData[i*Width + j];
				}
			}
		}
		else
		{
			if (m_nBits <= 8) 
			{//反转数据
				FlipData(lpRawData,dwBytes, Height);
			}
			memcpy(m_lpData,lpRawData,dwBytes * Height);
			//modified by xieyhfirst 040830 
			// mingxing liu find this bug
			if (m_nBits <= 8) 
			{//反转数据
				FlipData(lpRawData,dwBytes, Height);
			}
		}
		
	}
*/
	m_bInit = TRUE;
	return TRUE;
}


BOOL CDib::OutputAsRaw(BYTE* lpRawData)
{
	if (!m_bInit) 
		return FALSE;

	DWORD dwBytes = WIDTHBYTES((DWORD) m_lpBMIH->biWidth * m_nBits);
	
/*	DWORD dwBytes = ((DWORD) m_lpBMIH->biWidth * m_nBits) / 32;
	if (((DWORD) m_lpBMIH->biWidth * m_nBits) % 32) 
		dwBytes ++;
	dwBytes *= 4;*/


	int i, j, k;
	int LineWidth = (m_lpBMIH->biWidth * m_nBits + 7) / 8;
	if (m_lpBMIH->biHeight > 0) {
		for (i = m_lpBMIH->biHeight-1, k = 0; i >= 0; i --, k ++) {
			for (j = 0; j < LineWidth; j ++) {
				lpRawData[k*LineWidth + j] = m_lpData[i*dwBytes + j];
			}
		}
	}
	else {
		for (i = 0; i < -(m_lpBMIH->biHeight); i ++) {
			for (j = 0; j < LineWidth; j ++) {
				lpRawData[i*LineWidth + j] = m_lpData[i*dwBytes + j];
			}
		}
	}

	return TRUE;
}

BOOL CDib::GetRectImage(CRect rcImage,BYTE *pRcImage)
{
	if (!m_bInit) 
		return FALSE;

	CRect rcDib(0,0,m_lpBMIH->biHeight,m_lpBMIH->biWidth);
	CPoint ptTopleft(rcImage.top,rcImage.left);
	CPoint ptBottomRight(rcImage.bottom,rcImage.right);

	if (!rcDib.PtInRect(ptTopleft)||!rcDib.PtInRect(ptBottomRight))
		return FALSE;

	DWORD dwBytes = WIDTHBYTES((DWORD) m_lpBMIH->biWidth * m_nBits);
	
/*	DWORD dwBytes = ((DWORD) m_lpBMIH->biWidth * m_nBits) / 32;
	if (((DWORD) m_lpBMIH->biWidth * m_nBits) % 32) 
		dwBytes ++;
	dwBytes *= 4;*/

	int i, j, k;
	int LineWidth = (rcImage.Width() * m_nBits + 7) / 8;
	if (m_lpBMIH->biHeight > 0) {
		for (i = m_lpBMIH->biHeight - rcImage.top - 1, k = 0;
		     i >= m_lpBMIH->biHeight - rcImage.bottom; i --, k ++) {
			for (j = 0; j < LineWidth; j ++) {
				pRcImage[k*LineWidth + j] = m_lpData[i*dwBytes + j + rcImage.left];
			}
		}
	}
	else {
		for (i = rcImage.top; i < rcImage.bottom; i ++) {
			for (j = 0; j < LineWidth; j ++) {
				pRcImage[i*LineWidth + j] = m_lpData[i*dwBytes + j + rcImage.left];
			}
		}
	}

	return TRUE;
}

int CDib::GetWidth()
{
	if (!m_bInit) return 0;
	else
		return m_lpBMIH->biWidth;
}

int CDib::GetHeight()
{
	if (!m_bInit) return 0;
	else
		return m_lpBMIH->biHeight;
}

void CDib::InitNull()
{
	m_dwLength	= 0L;
	m_nBits		= 0;
	m_lpBuf		= NULL;
	m_bInit		= FALSE;

	m_lpColors	= NULL; // FOR Palette Manage
	m_lpBMFH = NULL;
	m_lpBMIH = NULL;
	m_lpBMI	 = NULL;
	m_lpData = NULL;
	m_bBottomUp = true;
}

BOOL CDib::MakeBMPFromGif(BYTE *lpBitmap)
{
	int	i, nBytes;

	if(lpBitmap == NULL)
		return FALSE;

	m_nBits = 8;
	BITMAPINFOHEADER *tBMIH;
	tBMIH = (BITMAPINFOHEADER *)lpBitmap;
	nBytes = fmod(tBMIH->biWidth, 4.0) ? ((tBMIH->biWidth / 4 + 1) * 4) : tBMIH->biWidth; 
	m_dwLength = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ 256 * sizeof(RGBQUAD) + nBytes * tBMIH->biHeight;

	if(m_lpBuf != NULL)
		AllocateMemory(TRUE);
	else
		AllocateMemory();

	m_lpBMFH->bfType = 0x4d42;
	memcpy(m_lpBMI, lpBitmap, sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
	m_lpBMIH = (BITMAPINFOHEADER *)m_lpBMI;
	m_lpData = (BYTE *)m_lpBMIH + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	BYTE *pRow = new BYTE[m_lpBMIH->biWidth];
	for(i = 0; i < m_lpBMIH->biHeight; i ++)
		memcpy(m_lpData + (m_lpBMIH->biHeight - i - 1) * nBytes, lpBitmap
		+ sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + i * nBytes, m_lpBMIH->biWidth);

	delete pRow;

//	convert to gray

	return TRUE;
}


BOOL CDib::Decode_RLE4(CFile &rdFile, BYTE * pBuf, int nWidth, int nHeight)
{
	int i, j, k, flag;
	BYTE *ptr, code, codelength;

	for (i = 0; i < nWidth * nHeight; i ++)
		pBuf[i] = 0;
	for (i = 0; i < nHeight; i ++) {
		ptr = pBuf + i * nWidth;

		flag = 1;
		j = 0;
		while (flag) {
			rdFile.Read(&codelength, 1);
			if (codelength) {	// repeated pixels
				rdFile.Read(&code, 1);
				for (k = 0; k < codelength; k ++) {
					if (k & 1)
						ptr[j++] = code & 0x0F;
					else
						ptr[j++] = code >> 4;
				}
			}
			else {
				rdFile.Read(&codelength, 1);
				switch (codelength) {
				case 0:		// end of line
					flag = 0;
					break;
				case 1:		// end of total image
					flag = 0;
					i = nHeight;
					break;
				case 2:		// move of index
					rdFile.Read(&code, 1);
					ptr += code * nWidth;
					rdFile.Read(&code, 1);
					j += code;
					break;
				default:
					for (k = 0; k < codelength; k ++) {
						if (k & 1)
							ptr[j++] = code & 0x0F;
						else {
							rdFile.Read(&code, 1);
							ptr[j++] = code >> 4;
						}
					}
					if (((codelength + 1) >> 1) & 1)
						rdFile.Read(&code, 1);
				}	// end of switch
			}	// end of else
		}	// end of while
	}	// end of for (j = 0..nHeighr)

	return TRUE;
}

BOOL 
CDib::Decode_RLE8(CFile &rdFile, 
				  BYTE * pBuf, 
				  int nWidth, 
				  int nHeight)
{
	int i, j, k, flag;
	BYTE *ptr, code, codelength;

	for (i = 0; i < nWidth * nHeight; i ++)
		pBuf[i] = 0;
	for (i = 0; i < nHeight; i ++) {
		ptr = pBuf + i * nWidth;

		flag = 1;
		j = 0;
		while (flag) {
			rdFile.Read(&codelength, 1);
			if (codelength) {	// repeated pixels
				rdFile.Read(&code, 1);
				while (codelength --)
					ptr[j++] = code;
			}
			else {
				rdFile.Read(&codelength, 1);
				switch (codelength) {
				case 0:		// end of line
					flag = 0;
					break;
				case 1:		// end of total image
					flag = 0;
					i = nHeight;
					break;
				case 2:		// move of index
					rdFile.Read(&code, 1);
					ptr += code * nWidth;
					rdFile.Read(&code, 1);
					j += code;
					break;
				default:
					for (k = 0; k < codelength; k ++) {
						rdFile.Read(&code, 1);
						ptr[j++] = code;
					}
					if (codelength & 1)
						rdFile.Read(&code, 1);
				}	// end of switch
			}	// end of else
		}	// end of while
	}	// end of for (j = 0..nHeighr)

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// returns the pointer to the first palette index 
BOOL 
CDib::IsEmpty()
{
	return (m_lpBuf == NULL);
}

////////////////////////////////////////////////////////////////////////////////
// returns the pointer to the first palette index 
RGBQUAD* 
CDib::GetPalette() const
{
	return m_lpColors;
	//or the following routine
	if ((m_lpBuf)&&(m_lpBMIH->biBitCount <= 8/*m_lpBMIH->biClrUsed*/))
	{
		return (RGBQUAD*)((BYTE*)m_lpBuf + sizeof(BITMAPINFOHEADER));
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
// returns the palette dimension in byte
DWORD CDib::GetPaletteSize()
{
	if (m_lpBMIH->biBitCount <= 8 )
	{
		return (DWORD)((pow(2.0,(m_lpBMIH->biBitCount))) * sizeof(RGBQUAD));
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
RGBQUAD CDib::GetPaletteColor(BYTE idx)
{
	RGBQUAD rgb = {0,0,0,0};
	if ((!IsEmpty())&&(m_lpBMIH->biBitCount <= 8))
	{
		BYTE* iDst = (BYTE*)m_lpBuf + sizeof(BITMAPINFOHEADER);
		if (idx < (pow(2.0, m_lpBMIH->biBitCount)))
		{
			long ldx=idx*sizeof(RGBQUAD);
			rgb.rgbBlue = iDst[ldx++];
			rgb.rgbGreen=iDst[ldx++];
			rgb.rgbRed =iDst[ldx++];
			rgb.rgbReserved = iDst[ldx];
		}
	}
	return rgb;
}

////////////////////////////////////////////////////////////////////////////////
//	set the Palette of the DIB
void CDib::SetPalette(RGBQUAD* pPal,DWORD nColors)
{
	if ((m_lpBuf==NULL)||(m_lpColors==NULL)||(m_lpBMIH->biBitCount > 8))
		return;
	memcpy(GetPalette(),pPal,min(GetPaletteSize(),nColors*sizeof(RGBQUAD)));
}


////////////////////////////////////////////////////////////////////////////////
void CDib::SetPalette(DWORD n, BYTE *r, BYTE *g, BYTE *b)
{
	if ((!r)||(m_lpColors == NULL)||(m_lpBMIH->biBitCount > 8)) 
	{
		return;
	}
	if (!g) g = r;
	if (!b) b = g;
	RGBQUAD* ppal=GetPalette();
	DWORD m=min(n,((DWORD)pow(2.0, (m_lpBMIH->biBitCount))));
	for (DWORD i=0; i<m;i++)
	{
		ppal[i].rgbRed=r[i];
		ppal[i].rgbGreen=g[i];
		ppal[i].rgbBlue=b[i];
	}
}

////////////////////////////////////////////////////////////////////////////////
void CDib::SetPaletteColor(BYTE idx, BYTE r, BYTE g, BYTE b, BYTE alpha)
{//not test
	if ((m_lpBuf)&&(m_lpBMIH)&&(m_lpBMIH->biBitCount <= 8))
	{
		BYTE* iDst = (BYTE*)(m_lpBuf) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		if (idx < (pow(2.0, (m_lpBMIH->biBitCount))))
		{
			long ldx=idx*sizeof(RGBQUAD);
			iDst[ldx++] = (BYTE) b;
			iDst[ldx++] = (BYTE) g;
			iDst[ldx++] = (BYTE) r;
			iDst[ldx] = (BYTE) alpha;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void CDib::SetPaletteColor(BYTE idx, RGBQUAD c)
{
	if ((m_lpBuf)&&(m_lpBMIH)&&(m_lpBMIH->biBitCount <= 8))
	{
		if (idx < (pow(2.0, (m_lpBMIH->biBitCount))))
		{
			m_lpColors[idx] = c;
		}
		return;//note here;
		//or
		BYTE* iDst = (BYTE*)(m_lpBuf) + sizeof(BITMAPINFOHEADER);
		if (idx < (pow(2.0, (m_lpBMIH->biBitCount)))/*m_lpBMIH->biClrUsed*/)
		{
			long ldx=idx*sizeof(RGBQUAD);
			iDst[ldx++] = (BYTE) c.rgbBlue;
			iDst[ldx++] = (BYTE) c.rgbGreen;
			iDst[ldx++] = (BYTE) c.rgbRed;
			iDst[ldx] = (BYTE) c.rgbReserved;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CDib::SetGrayPalette()
{
	if ((m_lpBuf == NULL)||(m_lpBMIH == NULL)||(m_lpBMIH->biBitCount > 8))
		return;

	RGBQUAD* pal=GetPalette();
	DWORD nColor = (DWORD)pow(2.0, (m_lpBMIH->biBitCount))/*m_lpBMIH->biClrUsed*/;
	int nTmp = 0;
	for (DWORD ni = 0; ni < nColor; ni++)
	{
		pal[ni].rgbBlue=pal[ni].rgbGreen = pal[ni].rgbRed = (BYTE)(ni*(255/(nColor - 1)));
		nTmp = (int)pal[ni].rgbBlue;
//		TRACE("%3d: %5d\n",ni,pal[ni].rgbBlue);
	}
}

//////////////////////////////////////////////////////////////////////////
void CDib::SetStdPalette()
{
	if (!m_lpBuf) return;
	if (NULL== m_lpBMIH) return;

	switch (m_lpBMIH->biBitCount/*m_lpBMIH->biClrUsed*/){
	case 8:
		{
			const BYTE pal256[1024] = {
				  0,  0,  0,  0,  0,   0, 128,  0,  0,  128,  0,  0,  0,128,128,
				  0,128,  0,  0,  0, 128,   0,128,  0,  128,128,  0,  0,192,192,
				  192,0,
				192,220,192,0,240,202,166,0,212,240,255,0,177,226,255,0,142,212,255,0,107,198,255,0,
				72,184,255,0,37,170,255,0,0,170,255,0,0,146,220,0,0,122,185,0,0,98,150,0,0,74,115,0,0,
				50,80,0,212,227,255,0,177,199,255,0,142,171,255,0,107,143,255,0,72,115,255,0,37,87,255,0,0,
				85,255,0,0,73,220,0,0,61,185,0,0,49,150,0,0,37,115,0,0,25,80,0,212,212,255,0,177,177,255,0,
				142,142,255,0,107,107,255,0,72,72,255,0,37,37,255,0,0,0,254,0,0,0,220,0,0,0,185,0,0,0,150,0,
				0,0,115,0,0,0,80,0,227,212,255,0,199,177,255,0,171,142,255,0,143,107,255,0,115,72,255,0,
				87,37,255,0,85,0,255,0,73,0,220,0,61,0,185,0,49,0,150,0,37,0,115,0,25,0,80,0,240,212,255,0,
				226,177,255,0,212,142,255,0,198,107,255,0,184,72,255,0,170,37,255,0,170,0,255,0,146,0,220,0,
				122,0,185,0,98,0,150,0,74,0,115,0,50,0,80,0,255,212,255,0,255,177,255,0,255,142,255,0,255,107,255,0,
				255,72,255,0,255,37,255,0,254,0,254,0,220,0,220,0,185,0,185,0,150,0,150,0,115,0,115,0,80,0,80,0,
				255,212,240,0,255,177,226,0,255,142,212,0,255,107,198,0,255,72,184,0,255,37,170,0,255,0,170,0,
				220,0,146,0,185,0,122,0,150,0,98,0,115,0,74,0,80,0,50,0,255,212,227,0,255,177,199,0,255,142,171,0,
				255,107,143,0,255,72,115,0,255,37,87,0,255,0,85,0,220,0,73,0,185,0,61,0,150,0,49,0,115,0,37,0,
				80,0,25,0,255,212,212,0,255,177,177,0,255,142,142,0,255,107,107,0,255,72,72,0,255,37,37,0,254,0,
				0,0,220,0,0,0,185,0,0,0,150,0,0,0,115,0,0,0,80,0,0,0,255,227,212,0,255,199,177,0,255,171,142,0,
				255,143,107,0,255,115,72,0,255,87,37,0,255,85,0,0,220,73,0,0,185,61,0,0,150,49,0,0,115,37,0,
				0,80,25,0,0,255,240,212,0,255,226,177,0,255,212,142,0,255,198,107,0,255,184,72,0,255,170,37,0,
				255,170,0,0,220,146,0,0,185,122,0,0,150,98,0,0,115,74,0,0,80,50,0,0,255,255,212,0,255,255,177,0,
				255,255,142,0,255,255,107,0,255,255,72,0,255,255,37,0,254,254,0,0,220,220,0,0,185,185,0,0,150,150,0,
				0,115,115,0,0,80,80,0,0,240,255,212,0,226,255,177,0,212,255,142,0,198,255,107,0,184,255,72,0,
				170,255,37,0,170,255,0,0,146,220,0,0,122,185,0,0,98,150,0,0,74,115,0,0,50,80,0,0,227,255,212,0,
				199,255,177,0,171,255,142,0,143,255,107,0,115,255,72,0,87,255,37,0,85,255,0,0,73,220,0,0,61,185,0,
				0,49,150,0,0,37,115,0,0,25,80,0,0,212,255,212,0,177,255,177,0,142,255,142,0,107,255,107,0,72,255,72,0,
				37,255,37,0,0,254,0,0,0,220,0,0,0,185,0,0,0,150,0,0,0,115,0,0,0,80,0,0,212,255,227,0,177,255,199,0,
				142,255,171,0,107,255,143,0,72,255,115,0,37,255,87,0,0,255,85,0,0,220,73,0,0,185,61,0,0,150,49,0,0,
				115,37,0,0,80,25,0,212,255,240,0,177,255,226,0,142,255,212,0,107,255,198,0,72,255,184,0,37,255,170,0,
				0,255,170,0,0,220,146,0,0,185,122,0,0,150,98,0,0,115,74,0,0,80,50,0,212,255,255,0,177,255,255,0,
				142,255,255,0,107,255,255,0,72,255,255,0,37,255,255,0,0,254,254,0,0,220,220,0,0,185,185,0,0,
				150,150,0,0,115,115,0,0,80,80,0,242,242,242,0,230,230,230,0,218,218,218,0,206,206,206,0,194,194,194,0,
				182,182,182,0,170,170,170,0,158,158,158,0,146,146,146,0,134,134,134,0,122,122,122,0,110,110,110,0,
				98,98,98,0,86,86,86,0,74,74,74,0,62,62,62,0,50,50,50,0,38,38,38,0,26,26,26,0,14,14,14,0,240,251,255,0,
				164,160,160,0,128,128,128,0,0,0,255,0,0,255,0,0,0,255,255,0,255,0,0,0,255,0,255,0,255,255,0,0,255,255,255,0};
			memcpy(GetPalette(),pal256,1024);
			break;
		}
	case 4:
		{
			const BYTE pal16[64]={0,0,0,0,0,0,128,0,0,128,0,0,0,128,128,0,128,0,0,0,128,0,128,0,128,128,0,0,192,192,192,0,
				128,128,128,0,0,0,255,0,0,255,0,0,0,255,255,0,255,0,0,0,255,0,255,0,255,255,0,0,255,255,255,0};
			memcpy(GetPalette(),pal16,64);
			break;
		}
	}
	return;
}

int CDib::CalPaletteSize(const unsigned short nBts)
{
	if (nBts<0 ||nBts >64)
	{
		AfxMessageBox(_T("Input param is error!"));
		return -1;
	}

	int nPaletteSize = -1;
	switch (nBts) 
	{
	case 1:
		nPaletteSize = 2;
		break;
	case 4:
		nPaletteSize = 16;
		break;
	case 8:
		nPaletteSize = 256;
		break;
	case 24:
		nPaletteSize = 0;
		break;
	default:	// 16, 32
		nPaletteSize = 0;
		break;
	}
	nPaletteSize *= sizeof(RGBQUAD);
	return nPaletteSize;
}

bool CDib::FlipData(BYTE *&pData,int nWidth,int nHeight)
{
	DWORD dwBytes = WIDTHBYTES((DWORD) nWidth * m_nBits);
	
	BYTE *pchSrc,*pchDst,*pchTmp;
	pchSrc=pData + (nHeight-1)*nWidth;
	pchDst = new BYTE[nWidth* nHeight];
	pchTmp = pchDst;
	if (NULL == pchDst) {
		return false;
	}
	for(long y=0; y < nHeight; y++)
	{
		memcpy(pchTmp,pchSrc,nWidth);
		pchSrc -= nWidth;
		pchTmp += nWidth;
	}
	
	memcpy(pData,pchDst,nWidth*nHeight);
	delete [] pchDst;
	return true;
}
/*
{
	DWORD dwBytes = WIDTHBYTES((DWORD) nWidth * m_nBits);

	BYTE *pchSrc,*pchDst,*pchTmp;
	pchSrc=pData + (nHeight-1)*nWidth;
	pchDst = new BYTE[nWidth* nHeight];
	pchTmp = pchDst;
	if (NULL == pchDst) {
		return false;
	}
	for(long y=0; y < nHeight; y++){
		memcpy(pchTmp,pchSrc,nWidth);
		pchSrc -= nWidth;
		pchTmp += nWidth;
	}
	
	memcpy(pData,pchDst,nWidth*nHeight);
	delete [] pchDst;
	return true;
}*/
//////////////////////////////////////////////////////////////////////////
BYTE CDib::GetPixelIndex(long x,long y)
{
//	return 0l;
	if ((IsEmpty())||(m_lpBMIH->biBitCount > 8)) return 0;

	//modified 040907
	if ((x<0)||(y<0)||(x>=m_lpBMIH->biWidth)||(y>=m_lpBMIH->biHeight)) {
		return 0;
	}
	
	DWORD dwEffWidth = WIDTHBYTES(m_lpBMIH -> biWidth * m_lpBMIH -> biBitCount);
	if (m_lpBMIH->biBitCount==8){
		// modified by xieyhfirst 040916
		return m_lpData[(m_lpBMIH->biHeight - y - 1)*dwEffWidth + x];
		//return m_lpData[(m_lpBMIH->biHeight - y )*dwEffWidth + x];
	} else {
		BYTE pos;
		BYTE iDst= m_lpData[y * dwEffWidth + (x*m_lpBMIH->biBitCount >> 3)];
		if (m_lpBMIH->biBitCount==4){
			pos = (BYTE)(4*(1-x%2));
			iDst &= (0x0F<<pos);
			return (BYTE)(iDst >> pos);
		} else if (m_lpBMIH->biBitCount==1){
			pos = (BYTE)(7-x%8);
			iDst &= (0x01<<pos);
			return (BYTE)(iDst >> pos);
		}
	}
	return 0;
}

RGBQUAD CDib::GetPixelColor(long x, long y, bool bGetAlpha)
{
	RGBQUAD rgb={0,0,0,0};
	if (IsEmpty())
		return rgb;
	//modified 040907
	if((x <= 0)||(y <= 0)||
		(x >= m_lpBMIH->biWidth)||(y >= m_lpBMIH->biHeight))
	{
		return rgb;
	}
	
	if (m_lpBMIH->biBitCount <= 8){
		rgb = GetPaletteColor(GetPixelIndex(x,y));
	} else {
		//modified by xieyhfirst 040915
//		BYTE* iDst  = m_lpData + (m_lpBMIH ->biHeight - y +1)*WIDTHBYTES(m_lpBMIH -> biWidth * m_lpBMIH -> biBitCount) + x*3;
		BYTE* iDst  = m_lpData + (m_lpBMIH ->biHeight - y)*WIDTHBYTES(m_lpBMIH -> biWidth * m_lpBMIH -> biBitCount) + x*3;
		rgb.rgbBlue = *iDst++;
		rgb.rgbGreen= *iDst++;
		rgb.rgbRed  = *iDst;
	}
	
	return rgb;
}

int CDib::AddPixels(CDib *pDib)
{
	if (pDib->GetHeight() != m_lpBMIH->biHeight ||
		pDib->GetWidth() != m_lpBMIH->biWidth)
		return FALSE;

	int dwBytes = ((DWORD) m_lpBMIH->biWidth * m_nBits) / 32;
	if ((m_lpBMIH->biWidth * m_nBits) % 32) 
		dwBytes ++;
	dwBytes *= 4;

	for (int i = 0; i < m_lpBMIH->biHeight; i ++) {
		for (int j = 0; j < dwBytes; j ++) {
			int tmp = m_lpData[i * dwBytes + j] +
				pDib->m_lpData[i * dwBytes + j] * 3 / 4;
			if (tmp > 255)
				tmp = 255;
			m_lpData[i * dwBytes + j] = (BYTE)tmp;
		}
	}

	return TRUE;
}
