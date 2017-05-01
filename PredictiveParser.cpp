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

PredictiveParser::~PredictiveParser() {
    for(int i = 0; i < rows; i++){
        delete []table[i];
    }
    delete []table;
}

bool PredictiveParser::validateCode(string filename) {
    ifstream inFile(filename);
    string line{""}, inputString{""};
    while(inFile >> line){
        if(line == "PROGRAM")
            inputString += "r";
        else if(line == "BEGIN")
            inputString += "b";
        else if(line == "PRINT")
            inputString += "p";
        else if(line == "INTEGER")
            inputString += "i";
        else if(line == "END.")
            inputString += "e";
        else
            inputString += (line + " ");
    }

    cout << inputString << endl;
    // Make sure that PROGRAM, BEGIN, and END. are present before tracing
    try {
        if (!std::regex_match(inputString, std::regex("(r)(.*)")))
            throw "PROGRAM is expected";
        if (!std::regex_match(inputString, std::regex("(.*)(b)(.*)")))
            throw "BEGIN is expected";
        if (!std::regex_match(inputString, std::regex("(.*)(e)")))
            throw "END. is expected";

        if(trace(inputString))
            return true;
    }
    catch(char const* ERROR){
        cout << ERROR;
    }
    return false;
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
    stack.push('e');
    stack.push('A');

    int  indexInputString = 0;
    char currentChar      = inputString[indexInputString]; // Read initial character
    int  symbolIndex      = grammarDict.at(currentChar);

    while(!stack.empty()){

        char top = stack.top();
        int topIndex  = grammarDict.at(top);
        stack.pop();

        if(top == currentChar) {
            if(currentChar == 'e') // if the match is a $, we know the input string is valid
                return true;
            currentChar = inputString[++indexInputString];
            if(currentChar == ' ')
                currentChar = inputString[++indexInputString];
            symbolIndex = grammarDict.at(currentChar);
        }
        else{
            // Grab value from table
            string tableValue = table[topIndex][symbolIndex];

            if(tableValue != "l") { // If lambda, don't push.
                if(tableValue == "n") { // n stands for 'no value'. If n is found, then the input string is invalid.
                    cout << currentChar << endl;
                    // INTEGER expected case
                    if(currentChar == ':')
                        cout << "\nINTEGER is expected";
                    else
                        cout << "\n; is expected";

                    return false;
                }
                else {
                    // push states to stack
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