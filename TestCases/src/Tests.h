#pragma once

#define LOGStr(VAR) PrintStr(VAR, __FUNCTION__, __LINE__)

#include "Iterables/Dynamic/String.h"

void PrintStr(const CTL::Dynamic::String& str, const char* funcName, int line);

void ConstructorsTest();
void Test2();
void AppendTest();
void Test4();
void Test5();
void Test6();
void Test7();
void Test8();
void Test9();
void Test10();
void Test11();
void Test12();
void Test13();