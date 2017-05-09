#include "myHeader.h"

int main() {

    const int NUM_ROWS{22}, NUM_COLS{30};
    const string PARSING_TABLE_FILE = "resources/parsingtable.txt";
    const string TERMINALS_FILE = "resources/terminals.txt";
    const string NONTERMINALS_FILE = "resources/nonterminals.txt";
    const string INPUT_FILE = "resources/finalv1.txt"; //original input file
    const string CLEAN_FILE = "resources/finalv2.txt"; //cleaned input file ready for parsing
    const string FINAL_FILE = "resources/finalv3.cpp"; //final file in C++
    const regex REGULAR_EXPRESSION("END[.]|//[^/]*//|-?[0-9]+|[a-zA-Z]+[0-9]*[a-zA-Z]*|[*/+-=,()]");

    //output clean version of original input file to new file
    cleanFile(INPUT_FILE, CLEAN_FILE, REGULAR_EXPRESSION);

    //initialize the parsing table
    PredictiveParser parser(NUM_ROWS, NUM_COLS, TERMINALS_FILE, NONTERMINALS_FILE, PARSING_TABLE_FILE);

    //run the contents of the clean file through the parsing table, and if the code is validated
    //translate it to C++, then compile and run it
    if(parser.validateCode(CLEAN_FILE)) {
        cout << "\nNo error.\n\nTranslating to C++ ...";
        translateToCPP(CLEAN_FILE, FINAL_FILE);
        cout << "\n\nPress Enter to run C++ File!";
        cin.get();
        cout << "Output:\n\n";
        system("g++ resources/finalv3.cpp -o resources/finalv3");
        system("./resources/finalv3");
    }

    return 0;
}