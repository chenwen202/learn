/******************************************************************************
	COPYRIGHTS (C), 2006-2008, PeopleSpot TECHNOLOGY.

	FILE NAME:	dib.h
	AUTHOR:		Xie Yonghui
	VERSION:	2.0
	DATE:		08-18-2008
	COMMENTS:	General Purpose DIB Class.
	REVISION:	
*******************************************************************************/

#ifndef __DIB_H__
#define __DIB_H__
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类
#include <Strsafe.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef WIDTHBYTES
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 
#endif
//WIDTHBYTES(bits)           ((unsigned)((bits+31)&(~31))/8)

//#ifndef BYTE
//#define BYTE unsigned char
//#endif
//
//#ifndef DWORD
//#define DWORD unsigned int
//#endif

class CDib
{
friend class CImageLoader;

//	Attributes
private:
	BYTE				*m_lpBuf;	// DIB data buffer
	DWORD				m_dwLength;	// Total buffer length, including file header
	int					m_nBits;	// Number of color bits per pixel

	//	Pointers for internal use
	LPBITMAPFILEHEADER	m_lpBMFH;	// DIB文件头指针
	LPBITMAPINFOHEADER	m_lpBMIH;	// DIB信息头指针
	LPBITMAPINFO		m_lpBMI;
	RGBQUAD				*m_lpColors;// 调色板的入口
	BYTE				*m_lpData;
public:
	BYTE				m_bInit;
	//added by xieyhfirst 040916
	bool				m_bBottomUp;
	bool				IsBottomUp(){return m_bBottomUp;};
	void				SetDirectory(bool bBottomUp = true){m_bBottomUp = false;}
// Operations
private:
	void InitNull();
	BOOL AllocateMemory(BOOL bRealloc = FALSE);
protected:
	// 游程长度编码解码算法，Decode_RLE4针对4位位图，Decode_RLE8针对8位位图
	BOOL Decode_RLE4(CFile &pFile, BYTE *pBuf, int nWidth, int nHeight);
	BOOL Decode_RLE8(CFile &pFile, BYTE *pBuf, int nWidth, int nHeight);	
	
// Constructor and destructor
public:
	CDib();
	CDib(char* strFileName);
	CDib(CFile* pFile);
	CDib(CDC* pDC, int nBt = 0, BOOL bCompr = TRUE);
	CDib(CSize &DibSize, int nBt = 8);
	CDib(unsigned int dwWidth, unsigned int dwHeight, int nBt);
	CDib(CString strFileName);
	virtual ~CDib();

	BOOL Read(CFile *pFile);
	BOOL Write(CFile *pFile);
	BYTE	*Create(DWORD dwWidth, DWORD dwHeight, int nBt = 8);
	void	Copy(const CDib* pDibSrc);

	// Get and Set
	BYTE	*GetDIBData() {return m_lpData;};
	int		GetHeight();
	int		GetWidth();
	int		GetColorBits();
	DWORD	GetLength();
	void	SetMonoColors(DWORD dwForeground, DWORD dwBackground);
	BOOL	GetMonoColors(DWORD& dwForegroung, DWORD& dwBackground);
	BOOL	GetRectImage(CRect rcImage,BYTE *pRcImage);
	RGBQUAD GetPixelColor(long x,long y, bool bGetAlpha = false);
	BYTE	GetPixelIndex(long x,long y);
		

	RGBQUAD	*GetPalette() const;
	DWORD	GetPaletteSize();
	RGBQUAD GetPaletteColor(BYTE idx);	
	void	SetStdPalette();
	void	SetGrayPalette();
	void	SetPalette(DWORD n, BYTE *r, BYTE *g, BYTE *b);
	void	SetPalette(RGBQUAD* pPal,DWORD nColors=256);
	void	SetPaletteColor(BYTE idx, BYTE r, BYTE g, BYTE b, BYTE alpha);
	void	SetPaletteColor(BYTE idx, RGBQUAD c);
	int		AddPixels(CDib *pDib);

	// Format conversion
	BOOL	OutputAsRaw(BYTE* lpRawData);
	BOOL	MakeBMPFromRaw(BYTE* lpRawData, int Width, int Heigth, int Type = 8);
	CBitmap *MakeBitmap(CDC* pDC, CSize& bmSize);
	BOOL	MakeBMPFromGif(BYTE *lpBitmap);

	// Display
	BOOL Display(CDC*, CPoint origin);
	BOOL Stretch(CDC*, CPoint origin, CSize size);

	BOOL IsEmpty();
	int	 CalPaletteSize(const unsigned short nBts);	
	bool FlipData(BYTE *&pData,int nWidth,int nHeight);	
};

#endif // #ifndef __DIB_H__
