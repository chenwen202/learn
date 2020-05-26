#include <iostream>
#include <thread>

class Aobing : public std::thread {
    private:
        bool flag = false;
    public:
        bool isFlag() {
            return flag;
        }
        void run() {
            
        }

}

int main() {
    std::cout << "Hello Easy C++ project!" << std::endl;
}