#include <iostream>
#include <cmath>
#include <cstring>
#include <sstream>
#include "mathfunc.h"

using namespace std;

class Solution {
public:
    /**
     * @param s: the given string
     * @return: whether this string is valid
     */
    bool checkValidString(string &s) {
        // Write your code here
        const char* p = s.c_str();
        int l =0;
        int cp =0;       
        while( *p != '\0')
        {
            char c = *p;
            if(c == '(') 
            {
                ++l;
                ++cp;
            }
            else if( c == '*')
            {
                if(l > 0) --l;
                ++cp;
            }
            else if( c == ')')
            {
                if( l > 0) --l;
                --cp;
                if( cp < 0) return false;
            }
            ++p;            
        }
        
        if( l == 0) return true;
        return false;
    }
};

int main() {
    struct bit {
        int a:3;
        int b:3;
        int c:2;
    } num;
        
    char *c = (char*)&num;    
    *c = 0xdb;
    
    std::cout<<"a: " << num.a << " b: "<< num.b << " c: " << num.c <<std::endl;

   

    // int d = 5;
    // int c = 3;
    // const int *p = &d;
    // p = &c;
    // std::cout<< *p <<std::endl;

    // const int* a = new int[10]{5};
    // int* const b = new int[10]{4};

    // auto f = [](const int* arr, int size) 
    // {
    //     std::string str;
    //     std::stringstream ss(str);
    //     for(int i=0; i< size; ++i)
    //         ss << arr[i];

    //     arr = new int[10]{6};
    //     return ss.str();
    // };
    // std::cout<<b<<std::endl;
    // std::cout<<f(b,10)<<std::endl;
    // std::cout<<b<<std::endl;
    // b[1] =5;
    // std::cout<<f(b, 10)<<std::endl;

    return 0;
}
