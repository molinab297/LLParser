
#ifndef FINAL_PROJECT_COMPILERS_TRANSLATETOCPP_H
#define FINAL_PROJECT_COMPILERS_TRANSLATETOCPP_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

void translateToCPP(string inFileName, string outFileName){

    cout << "Translating to C++ ... ";
    ifstream inFile(inFileName);
    ofstream outFile(outFileName);
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
        else {
            outFile << str + " ";

        }
    }
    outFile << "\nreturn 0;" << "\n}";
}


#endif //FINAL_PROJECT_COMPILERS_TRANSLATETOCPP_H
