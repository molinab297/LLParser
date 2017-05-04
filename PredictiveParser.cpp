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
    loadGrammarDict(terminalsFileName, colDict);
    loadGrammarDict(nonterminalsFileName, rowDict);
}

PredictiveParser::~PredictiveParser() {
    for(int i = 0; i < rows; i++){
        delete []table[i];
    }
    delete []table;
}

bool PredictiveParser::validateCode(string filename) {
    ifstream inFile(filename);
    string line{""};
    string inputString{""};
    string temp{""};
    std::regex variableMatch("[^ribp][P-S]+[0-9]*[P-S]*"); // Expression to match any identifier
    std::unordered_set<string> identifierSet;

    // Convert PROGRAM, BEGIN, PRINT, INTEGER to r, b, p, i
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
            inputString += line;
    }

//    // Make sure that PROGRAM, BEGIN, and END. are present before tracing
//    try {
//        if (!std::regex_match(inputString, std::regex("(r)(.*)")))
//            throw "PROGRAM is expected";
//        if (!std::regex_match(inputString, std::regex("(.*)(b)(.*)")))
//            throw "BEGIN is expected";
//        if (!std::regex_match(inputString, std::regex("(.*)(e)")))
//            throw "END. is expected";
//
//        // Pre-parsing complete. Begin parsing using Predictive Table
        if(trace(inputString))
            return true;
//    }
//    catch(char const* ERROR){
//        cout << ERROR;
//    }
//    return false;
}

/* Loads dictionary with both terminal and non-terminal characters. The value to each terminal/non-terminal
 * character in the dictionary is the corresponding row or column of that character. */
void PredictiveParser::loadGrammarDict(string charFileName, std::unordered_map<char,int> &map){
    ifstream inFile(charFileName);
    int index = 0;
    char c;
    while(!inFile.eof()){
        inFile >> c;
        map.insert(map.begin(), std::make_pair(c, index));
        index++;
    }
}

void PredictiveParser::printStackContents(stack<char> stack){
    cout << "Stack: ";
    while(!stack.empty()){
        char curr = stack.top();
        stack.pop();
        cout << curr << " ";
    }
    cout << endl << endl;
}

/* Checks if an input string is a valid expression. */
bool PredictiveParser::trace(string inputString){
    stack<char> stack;
    stack.push('e');
    stack.push('A');

    int  indexInputString = 0;
    char currentChar      = inputString[indexInputString]; // Read initial character
    int  symbolIndex      = getColIndex(currentChar);

    while(!stack.empty()){

        printStackContents(stack);

        char top = stack.top();
        int topIndex  = getRowIndex(top);
        stack.pop();


        if(top == currentChar) {
            cout << "match " << currentChar << endl;

            if(currentChar == 'e') // if the match is a e, we know the input string is valid
                return true;
            currentChar = inputString[++indexInputString];
            symbolIndex = getColIndex(currentChar);
        }
        else{

                if(topIndex == -1 || symbolIndex == -1){
                    cout << " top key is " << stack.top() << endl;
                getErrorMessage(stack.top(), top);

                return false;
            }

            // Grab value from table
            string tableValue = table[topIndex][symbolIndex];

            cout << " [" << top << "," << currentChar << "] = " << tableValue << endl;
            cout << "top index : " << topIndex << " anddddd " <<  " symbolIndex  " << symbolIndex << endl;


            if(tableValue != "l") { // If lambda, don't push.
                if(tableValue == "n") { // n stands for 'no value'. If n is found, then the input string is invalid.
                    cout << "the key is " << stack.top() << endl;
                    getErrorMessage(stack.top(), top);
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

int PredictiveParser::getRowIndex(char key) {
    std::unordered_map<char,int>::const_iterator got = rowDict.find(key);
    if(got != rowDict.end())
        return rowDict.at(key);
    return -1;
}

int PredictiveParser::getColIndex(char key) {
    std::unordered_map<char,int>::const_iterator got = colDict.find(key);
    if(got != colDict.end()) {
        return colDict.at(key);
    }
        //check if symbol is a letter, if so set to last col of table
    else if(isalpha(key)) {
        return colDict.size();
    }
    else
        return -1;
}

void PredictiveParser::getErrorMessage(char key, char topStack) {

    switch(key){
        case '1':
            cout << "PROGRAM is expected";
            break;
        case '2':
            cout << "INTEGER is expected";
            break;
        case '3':
            cout << "Unacceptable variable name";
            break;
        case '4':
            cout << "Variable is expected";
            break;
        case '5':
            cout << "; is missing";
            break;
        case '6':
            cout << "illegal expression";
            break;
        case '7':
            cout << "invalid character";
            break;
        case 'e':
            cout << "END. is expected";
        break;
        default:
            if(topStack == 'b'){
                cout << "BEGIN is expected";
            }
            else{
                cout << " default case: ";
                cout << topStack << " is missing";
            }

    }

}
