#ifndef SYSTEM_G_PRINT_TEST_HEADER_HH
#define SYSTEM_G_PRINT_TEST_HEADER_HH

#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>

void print_test_header(const char* aTestName, int aTot = 80);
void print_test_header(std::ostream& os, const char* aTestName, int aTot = 80);

#endif
