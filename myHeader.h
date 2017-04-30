#ifndef COMPILER_MYHEADER_H
#define COMPILER_MYHEADER_H

#include <regex>
#include <string>
#include "cleanFile.h"
using namespace std;

//Purpose: Accepts an input file then uses the passed in regular expression to iterate through regex matches and
//output those matches to the passed in output file
void cleanFile(string inputFile, string outputFile, regex regexExpression);

#endif //COMPILER_MYHEADER_H