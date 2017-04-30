#include "PredictiveParser.h"

/* Class constructor */
PredictiveParser::PredictiveParser(int rows, int columns, string terminalsFileName, string nonterminalsFileName, string parsingTableName) {

    // initialize 2d string array
    this->rows    = rows;
    this->columns = columns;
    table = new string*[rows];
    for(int i = 0; i < rows; i++)
        table[i] = new string[columns];

    // load array with parsing table & grammar dictionary
    loadTable(parsingTableName);
    loadGrammarDict(terminalsFileName);
    loadGrammarDict(nonterminalsFileName);
}

/* Class destructor: Frees memory from 2d string array */
PredictiveParser::~PredictiveParser() {
    for(int i = 0; i < rows; i++){
        delete []table[i];
    }
    delete []table;
}

/*  */
bool PredictiveParser::validateCode(string codeFileName) {
    ifstream inFile(codeFileName);
    string line{""};
    while(!inFile.eof()){
        getline(inFile, line);
        if(!trace(line)){
            return false;
        }
    }
    return true;
}

/* Loads dictionary with both terminal and non-terminal characters. The value to each terminal/non-terminal
 * character in the dictionary is the corresponding row or column of that character. */
void PredictiveParser::loadGrammarDict(string charFileName){
    ifstream inFile(charFileName);
    int index = 0;
    char c;
    while(!inFile.eof()){
        inFile >> c;
        grammarDict.insert(grammarDict.begin(), std::make_pair(c, index));
        index++;
    }
}

/* Checks if an input string is a valid expression. */
bool PredictiveParser::trace(string inputString){
    stack<char> stack;
    stack.push('$');
    stack.push('E');

    int  indexInputString = 0;
    char currentChar      = inputString[indexInputString]; // Read initial character
    int  symbolIndex      = getStateIndex(currentChar);

    while(!stack.empty()){

        char top = stack.top();
        int topIndex  = getStateIndex(stack.top());
        stack.pop();

        if(top == currentChar) {
            if(currentChar == '$') // if the match is a $, we know the input string is valid
                return true;
            currentChar = inputString[++indexInputString];
            symbolIndex = getStateIndex(currentChar);
        }
        else{
            // Grab value from table
            string tableValue = table[topIndex][symbolIndex];

            if(tableValue != "l") { // If lambda, don't push.
                if(tableValue == "n") // n stands for 'no value'. If n is found, then the input string is invalid.
                    return false;
                else {
                    for (int i = tableValue.length() - 1; i >= 0; i--)
                        stack.push(tableValue[i]);
                }
            }
        }
    }
    return false;
}

/* Loads a 2d array of strings from an input file. */
void PredictiveParser::loadTable(string parsingTableFileName){

    ifstream inFile;
    inFile.open(parsingTableFileName);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++)
            inFile >> table[i][j];
    }

    inFile.close();
}

/* Returns table index for either a row or column character. */
int PredictiveParser::getStateIndex(char state){
    return grammarDict.at(state);
}

