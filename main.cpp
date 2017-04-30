#include "myHeader.h"

int main() {

    const string INPUT_FILE = "finalv1.txt"; //original input file
    const string CLEAN_FILE = "finalv2.txt"; //cleaned input file ready for parsing
   // const regex REGULAR_EXPRESSION("END[.]|\/\/.*\n?|[0-9]+|[a-zA-Z]+[0-9]*[a-zA-Z]*|[*/+-=,()]"); //regular expression that cleans the input file according to rules of established language
    const regex REGULAR_EXPRESSION("END[.]|\/\/[^\/]*\/\/|-?[0-9]+|[a-zA-Z]+[0-9]*[a-zA-Z]*|[*/+-=,()]"); //regular expression that cleans the input file according to rules of established language

    //output clean version of original input file to new file
    cleanFile(INPUT_FILE, CLEAN_FILE, REGULAR_EXPRESSION);

    return 0;
}