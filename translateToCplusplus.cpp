#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

// Translate project code to C++ and exports to a .cpp file. The .cpp file is then compiled and ran.
void translateToCPlusPlus(string inputFileName) {

    cout << "\nTranslating to C++ ... ";
    ifstream inFile(inputFileName);
    ofstream outFile("resources/output.cpp");
    outFile << "#include <iostream>" << "\nusing namespace std;" << "\nint main()" << "\n{\n";
    string str;
    getline(inFile, str);

    while(!inFile.eof()){
        inFile >> str;
        if(str == "INTEGER")
            outFile << "int ";
        else if(str == "PRINT") {
            outFile << "cout<<";
            inFile >> str;
            inFile >> str;
            outFile << str << "<<endl ";
            inFile >> str;
        }
        else if(str == ";")
            outFile << ";\n";
        else if(str == "BEGIN" || str == "END." || str == ":")
            continue;
        else
            outFile << str + " ";
    }
    outFile << "return 0;" << "\n}";
}
