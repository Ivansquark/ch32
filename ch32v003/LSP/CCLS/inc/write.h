#ifndef WRITE_H
#define WRITE_H

#include "uart.h"
#include <charconv>
#include <string>

void print(const char* arr);

void printAddr(const char* text, uint32_t addr);
void printVal(const char* text, int32_t val);
void printStr(const char* text);
void printStr(std::string);

extern "C" int _write(int, char* ptr, int len);

#endif // WRITE_H
