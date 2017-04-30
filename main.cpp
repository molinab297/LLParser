#include "myHeader.h"

int main() {

    const int NUM_ROWS{22}, NUM_COLS{30};
    const string PARSING_TABLE_FILE = "resources/parsingtable.txt";
    const string TERMINALS_FILE = "resources/terminals.txt";
    const string NONTERMINALS_FILE = "resources/nonterminals.txt";
    const string INPUT_FILE = "resources/finalv1.txt"; //original input file
    const string CLEAN_FILE = "resources/finalv2.txt"; //cleaned input file ready for parsing
    const regex REGULAR_EXPRESSION("END[.]|[/][/].*\n?|[0-9]+|[a-zA-Z]+[0-9]*[a-zA-Z]*|[*/+-=,()]"); //regular expression that cleans the input file according to rules of established language

    //output clean version of original input file to new file
//    cleanFile(INPUT_FILE, CLEAN_FILE, REGULAR_EXPRESSION);


    PredictiveParser parser(NUM_ROWS, NUM_COLS, TERMINALS_FILE, NONTERMINALS_FILE, PARSING_TABLE_FILE);

    if(parser.validateCode(CLEAN_FILE))
        cout << "Ready to be translated to C++";

    return 0;
}