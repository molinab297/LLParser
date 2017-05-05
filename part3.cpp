#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <iomanip>

using namespace std;

int main()
{
	ofstream fout("output.cpp");
	ifstream fin("testrun.txt");
	string temp;
	string fileString;
	regex expression("[A-Za-z]+[0-9]*[A-Za-z]*|-?[0-9]+;|[*/+-=,()]");
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
			cout << "#include <iostream>\nusing namespace std;\nint main()\n{";
			fout << "#include <iostream>\nusing namespace std;\nint main()\n{";
			while (rit->str() != ";")
				rit++;
			if (rit->str() == ";")
			{
				cout << endl;
				fout << endl;
			}
				
		}
		else if (rit->str() == "END")
		{
			cout << "return 0;\n}" << endl;
			fout << "return 0;\n}" << endl;
		}
		else if (rit->str() == "INTEGER")
		{
			cout << "    int ";
			fout << "    int ";
		}
		else if (rit->str() == ";")
		{
			cout << "; " << endl;
			cout << "    ";
			fout << "; " << endl;
		}
		else if (rit->str() == "PRINT")
		{
			cout << "cout << ";
			fout << "cout << ";
			rit++;
			while (rit->str() != ";")
			{
				if (rit->str() != "(" && rit->str() != ")")
				{
					cout << rit->str() << " << ";
					fout << rit->str() << " << ";
				}
				rit++;
			}
			if (rit->str() == ";")
			{
				cout << "endl;" << endl;
				cout << "    ";
				fout << "endl;" << endl;
				fout << "endl;" << endl;
			}
		}
		else if (rit->str() != ":" && rit->str() != "BEGIN")
		{
				cout << rit->str() << " ";
				fout << rit->str() << " ";
		}
		rit++;
	
	}

	return 0;
}