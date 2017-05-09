/**
    translateToCPP.h

    Purpose: Accepts an input file then rewrites it to passed in outfile translated to C++
*/

#ifndef FINAL_PROJECT_COMPILERS_TRANSLATETOCPP_H
#define FINAL_PROJECT_COMPILERS_TRANSLATETOCPP_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

void translateToCPP(string inFileName, string outFileName){

    ofstream fout(outFileName);
    ifstream fin(inFileName);
    string temp;
    string fileString;
    regex expression("[A-Za-z]+[.]?[0-9]*[A-Za-z]*|-?[0-9]+;?|[*/+-=,()]");
    while (!fin.eof())
    {
        getline(fin, temp);
        fileString.append(temp + '\n');
    }
    fin.close();
    regex_iterator<string::iterator> rit(fileString.begin(), fileString.end(), expression);
    regex_iterator<string::iterator> rend;

    while (rit != rend)
    {
        if (rit->str() == "PROGRAM")
        {
            fout << "#include <iostream>\nusing namespace std;\nint main()\n{";
            while (rit->str() != ";")
                rit++;
            if (rit->str() == ";")
            {
                fout << endl;
            }
        }
        else if (rit->str() == "END.")
        {
            fout << "return 0;\n}" << endl;
        }
        else if (rit->str() == "INTEGER")
        {
            fout << "    int ";
        }
        else if (rit->str() == ";")
        {
            fout << "; " << endl;
        }
        else if (rit->str() == "PRINT")
        {
            fout << "cout << ";
            rit++;
            while (rit->str() != ";")
            {
                if (rit->str() != "(" && rit->str() != ")")
                {
                    fout << rit->str() << " << ";
                }
                rit++;
            }
            if (rit->str() == ";")
            {
                fout << "endl;" << endl;
            }
        }
        else if (rit->str() != ":" && rit->str() != "BEGIN")
        {
            fout << rit->str() << " ";
        }
        rit++;

    }
}

#endif //FINAL_PROJECT_COMPILERS_TRANSLATETOCPP_H
