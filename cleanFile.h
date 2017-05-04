/**
    cleanFile.cpp

    Purpose: Accepts an input file then uses the passed in regular expression to iterate through regex matches and
    output those matches to the passed in output file
*/

#ifndef COMPILER_CLEANFILE_H
#define COMPILER_CLEANFILE_H

#include <string>
#include <fstream>
#include <regex>
#include <iostream>
using namespace std;

void cleanFile(string inputFile, string outputFile, regex regexExpression){

    string fileString;
    string temp;
    ifstream inFile;
    ofstream oFile;

    inFile.open(inputFile);

    //loop through file and store contents in string
    while(!inFile.eof()){
        getline(inFile, temp);
        fileString.append(temp + '\n');
    }

    inFile.close();

    //initialize regex iterator to be used with the file string
    regex_iterator<std::string::iterator> rit (fileString.begin(),
                                               fileString.end(), regexExpression);
    regex_iterator<std::string::iterator> rend;

    oFile.open(outputFile);

    //output initial space to keep output format consistent
    oFile << ' ';

    //loop through regex iterator and output with 1 space before
    //and after each token and new lines after each semi colon
    while(rit != rend){

        std::cout << rit->str() << '\n';

        if(rit->str().substr(0,2) != "//")
            oFile << rit->str() << ' ';
        if(rit->str() == ";" || rit->str() == "BEGIN")
            oFile << endl << ' ';
        rit++;
    }

    oFile.close();
}

#endif //COMPILER_CLEANFILE_H
