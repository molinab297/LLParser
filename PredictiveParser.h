#ifndef FINAL_PROJECT_COMPILERS_PREDICTIVEPARSER_H
#define FINAL_PROJECT_COMPILERS_PREDICTIVEPARSER_H
#include <iostream>
#include <string>
#include <stack>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <regex>
using namespace std;


class PredictiveParser {
public:
    PredictiveParser(int rows, int columns, string terminalsFileName, string nonterminalsFileName, string parsingTableName);
    ~PredictiveParser();
    bool validateCode(string inputStringFileName);

private:
    void loadGrammarDicts(string terminalsFileName, string nonterminalsFileName);
    void loadTable(string parsingTableFileName);
    void printStackContents(stack<char> stack);
    int  getRowIndex(char key);
    int  getColIndex(char key);
    bool trace(string inputString);
    void getErrorMessage(char key, char topStack);

    string **table;
    int rows;
    int columns;
    std::unordered_map<char,int> rowDict;
    std::unordered_map<char,int> colDict;
    std::unordered_map<int, string> errorCodesDict;
};


#endif //FINAL_PROJECT_COMPILERS_PREDICTIVEPARSER_H
