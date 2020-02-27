
#ifndef _TEST_IMGOUTPUT_H_
#define _TEST_IMGOUTPUT_H_

#include <afxstr.h>
#include <vector>

//! @brief save the image as bitmap
//! @param[in] data/width/height
//! @param[in] filename, full path name
//! @return none
//!	@remarks
//!		2016.12.08 remove pack, no use this field
void SaveAsBitmap(const unsigned char* data,int width,int height,CString filename);

bool  GetBitmapBytes(CString path, std::vector<unsigned char>& buf,int& width,int& height);

#endif //_TEST_IMGOUTPUT_H_

