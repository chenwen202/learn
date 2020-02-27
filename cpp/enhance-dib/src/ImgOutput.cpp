
#include "ImgOutput.h"
#include "Dib.h"
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类
#include <Strsafe.h>


void SaveAsBitmap(const unsigned char* data,int width,int height,CString filename) {
		
	//directly return
	if( 0 == data ) return;

	CDib dib;
	if (dib.MakeBMPFromRaw((BYTE*)data, width, height))
	{		
		CFile file;
		if (file.Open(filename, CFile::modeCreate | CFile::modeReadWrite))
		{
			dib.Write(&file);
			file.Close();
		}		
	}
}

bool GetBitmapBytes(CString path, std::vector<unsigned char>& buf,int& width,int& height) {
	
	if( !PathFileExists(path)) return false;

	CDib dib;
	CFile file;
	if (!file.Open(path, CFile::modeRead))
	{		
		return false;
	}

	if (!dib.Read(&file))
	{
		file.Close();		
	}
	
	width = dib.GetWidth();
	height = abs(dib.GetHeight());
	if (buf.size() != width * height)
			buf.resize(width * height);
	dib.OutputAsRaw(&buf[0]);

	file.Close();

	return true;
}
