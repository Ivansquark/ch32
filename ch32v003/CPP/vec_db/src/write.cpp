#include "write.h"

extern "C" int _write(int, char* ptr, int len) {
    Uart::sendStr(ptr, len);
    return len;
}

void print(const char* arr) { Uart::sendStr(arr, strlen(arr)); }

void printAddr(const char* text, uint32_t addr) {
    char a[20] = {0};
    std::to_chars(a, a + sizeof(a), addr, (int)16);
    print(text);
    print(a);
    print((char*)("\r\n"));
}

void printVal(const char* text, int32_t val) {
    char a[20] = {0};
    std::to_chars(a, a + sizeof(a), val);
    print(text);
    print(a);
    print((char*)("\r\n"));
}
void printStr(const char* text) {
    print(text);
    print((char*)("\r\n"));
}
void printStr(std::string text) {
    print(text.c_str());
    print((char*)("\r\n"));
}
