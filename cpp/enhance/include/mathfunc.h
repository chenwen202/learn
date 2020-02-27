#ifndef _IMG_PROCESS_FUNC_
#define _IMG_PROCESS_FUNC_

#include <vector>

namespace processing {
#define IN
#define OUT
#define IO
class ImgProcFunc {
public:
    ImgProcFunc() { }
    ~ImgProcFunc(){}
public:
    void equalization(IN const unsigned char* src,  IN int cols , IN int rows, OUT unsigned char* dst);
private:
    static const int   LEVEL = 256;
    std::vector<unsigned char> buf_;    
    std::vector<float> eq_;
    void cal_equalising(const unsigned char* src,  int cols , int rows);
};
}


#endif //_IMG_PROCESS_FUNC_