#ifndef TEST_CPP_
#define TEST_CPP_

#include <sstream>
#include <cstring>

namespace test {

    template<typename T>
    const char* stringifyArr(T arr[], size_t size) {
        std::stringstream  buffer;
        for(size_t i = 0; i < size; ++i) {
            buffer << arr[i] << " ";
        }// for
        const std::string tmp = buffer.str();
        size_t len = tmp.length() + 1;
        char* out = new char[len];
        strncpy(out, tmp.c_str(), len);
        return out;
    }// stringifyArr

    template<typename T>
    void printArr(T arr[], size_t size) {
        for(size_t i = 0; i < size; ++i) {
            std::cout << arr[i] << " ";
        }// for
        std::cout << std::endl;
        return;
    }// printArr

}// ns

#endif // TEST_CPP_