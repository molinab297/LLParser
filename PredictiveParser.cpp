#include "PredictiveParser.h"

/* Class constructor */
PredictiveParser::PredictiveParser(int rows, int columns, string terminalsFileName, string nonterminalsFileName, string parsingTableName) {

    // initialize 2d string array
    this->rows    = rows;
    this->columns = columns;
    table = new string*[rows];
    for(int i = 0; i < rows; i++)
        table[i] = new string[columns];

    // load array with parsing table & load row/column dictionaries
    loadTable(parsingTableName);
    loadGrammarDicts(terminalsFileName, nonterminalsFileName);
}

PredictiveParser::~PredictiveParser() {
    for(int i = 0; i < rows; i++){
        delete []table[i];
    }
    delete []table;
}

/* Calls trace() to trace input string then performs some post-processing methods */
bool PredictiveParser::validateCode(string inputStringFileName) {
    std::ifstream inFile(inputStringFileName);
    std::string inputString, line;
    std::regex varLineMatch("(.*);i:(.*);b"), varMatch("[PQRS]+[0-9]*[PQRS]*[0-9]*");
    std::unordered_map<string,string> reservedWords{{"BEGIN","b"},{"END.","e"},{"PRINT","p"},{"INTEGER","i"},{"PROGRAM","r"}};

    // Converts reserved words to single letter terminals
    while(inFile >> line){
        if(line=="BEGIN" || line=="END." || line=="PRINT" || line=="PROGRAM")
            inputString += reservedWords.at(line);
        // Special case : handles missing commas
        else if(line == "INTEGER"){
            string tempLine;
            getline(inFile, tempLine);
            int rWordCount = std::distance(std::sregex_iterator(tempLine.begin(), tempLine.end(), varMatch), std::sregex_iterator());        // count number of reserved words
            int commaCount = std::distance(std::sregex_iterator(tempLine.begin(), tempLine.end(), std::regex(",")), std::sregex_iterator()); // count number of commas
            if (commaCount < rWordCount-1){ // if # of commas is less than # of reserved words, output error
                cout << "ERROR : Missing comma";
                return false;
            }
            // strip white space from line and append to input string
            tempLine.erase(remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
            inputString += (reservedWords.at("INTEGER") + tempLine);
        }
        else inputString+=line;
    }

    if(trace(inputString)) {

        // Special case : handles undeclared variables
        std::unordered_set<string> set{};
        std::smatch match;
        std::string line;
        if(regex_search(inputString,match,varLineMatch)){
            line = match.str();

            // Build set of identifiers
            while(regex_search(line,match,varMatch)){
                set.insert(match.str());
                line = match.suffix().str();
            }

            // Scan input string and look for undeclared identifiers
            while(regex_search(inputString, match, varMatch)){
                if(set.find(match.str()) == set.end()) {
                    cout << "ERROR: Unknown identifier";
                    return false;
                }
                inputString = match.suffix().str();
            }
        }
        return true;
    }
    return false;
}


/* Loads dictionary with both terminal and non-terminal characters. The value to each terminal/non-terminal
 * character in the dictionary is the corresponding row or column of that character. */
void PredictiveParser::loadGrammarDicts(string terminalsFileName, string nonterminalsFileName){
    ifstream inFile(terminalsFileName);
    int index = 0;
    char c;
    while(inFile >> c){
        colDict.insert(colDict.begin(), std::make_pair(c, index));
        index++;
    }
    inFile.close();
    index = 0;
    inFile.open(nonterminalsFileName);
    while(inFile >> c){
        rowDict.insert(rowDict.begin(), std::make_pair(c, index));
        index++;
    }
    inFile.close();
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
            cout << "top index : " << topIndex << " and " <<  " symbolIndex  " << symbolIndex << endl;


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
//        //check if symbol is a letter, if so set to last col of table
//    else{
//        return colDict.size();
//    }
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
        case 'b' :
            cout << "INTEGER is expected";
            break;
        default:
            if(topStack == 'b'){
                cout << "BEGIN is expected";
            }
            else if (topStack == 'C'){
                cout << "; is missing";
            }
            else{
                cout << " default case: ";
                cout << topStack << " is missing";
            }

    }

}
