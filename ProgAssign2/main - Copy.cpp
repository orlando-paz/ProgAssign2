#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

bool ProcessFile(string filename, string ouputfile);
int Paint(string stripes, bool debug = false);
int PaintOld(string stripes, bool debug = false);
char paletteUsedColors[26];
int usedColors[26]{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
int colorStart[26]{ 99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99 };
int colorEnd[26]{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };


void main()
{
	string filename;
	/*cout << "Please specify a file name: ";
	cin >> filename;*/
	filename = "ProgrammingAssignment2SampleInput2.txt";

	clock_t beginClock = clock();
	time_t beginTime = time(0);
	char* dtBegin = ctime(&beginTime);
	cout << "Program started at " << dtBegin;

	string ouputfile = filename + ".output.txt";
	if (ProcessFile(filename, ouputfile))
		cout << "Output file saved as \"" << ouputfile << "\"" << endl;

	clock_t endClock = clock();
	time_t endTime = time(0);
	char* dtEnd = ctime(&endTime);
	cout << "Program ended at " << dtEnd;
	double elapsed_secs = double(endClock - beginClock) / CLOCKS_PER_SEC;
	cout << "Running time in seconds: \"" << elapsed_secs << "\"" << endl;

	//Paint("YBYB", true); //OK
	//Paint("BGRGB", true); //OK
	//Paint("YBYCYRY", true); //OK
	//Paint("AABBBCCCDABABBBB", true); //OK
	//Paint("YBXCXBYYBBYM", true); //OK, me=>6
	//Paint("VYJICZOZZXJSTOZXGRPNIJSUH", true); //file1-line11=>20, me=>21
	//Paint("CBAKRXDCDXTQ", true); //file2-line731=>10, me=>11
	//Paint("YGVVPYPPWGDDNNI", true); //file2-line1067=>9, me=>10
	//Paint("YGPYPG", true); 
	//PaintOld("YGPYPG", true);
	Paint("ABCABC", true);
	PaintOld("ABCABC", true);

	system("pause");
}

bool ProcessFile(string filename, string ouputfile)
{
	string line;
	ifstream inputFile(filename);
	ofstream outputFile(ouputfile);
	if (inputFile.is_open() && outputFile.is_open())
	{
		while (getline(inputFile, line))
		{
			int numChanges = Paint(line);
			outputFile << numChanges << endl;
		}
		inputFile.close();
		outputFile.close();
	}
	else
	{
		cout << "ERROR!!!: Unable to open file" << endl;
		return false;
	}

	return true;
}

int Paint(string stripes, bool debug)
{
	int prevNumChanges = 0;
	int numChanges = 1;
	int paletteSize = 0;
	string output(stripes.length(), stripes[0]);
	if (debug)
	{
		cout << "00: " << stripes << endl;
		cout << "01: " << output << endl;
	}

	for (int i = 0; i < 26; i++)
	{
		usedColors[i] = 0;
		colorStart[i] = 99;
		colorEnd[i] = -1;
	}

	for (size_t i = 0; i < stripes.length(); i++)
	{
		bool colorExists = false;
		int color = int(stripes[i]) - 65;
		usedColors[color] = 1;
		if (i < colorStart[color])
			colorStart[color] = i;
		//if (i > colorEnd[color])
		//	colorEnd[color] = i;
	}


	for (size_t i = 1; i < stripes.length(); i++)
	{
		if (output[i] == stripes[i])
		{
			if(output[i] == stripes[0])
			paletteSize = 0;
			continue;
		}

		char newColor = stripes[i];
		bool colorWasUsed = false;
		for (int arrIndex = paletteSize - 1; arrIndex >= 0; arrIndex--)
			if (newColor == paletteUsedColors[arrIndex])
			{
				colorWasUsed = true;
				paletteSize = arrIndex + 1;
				break;
			}

		if (!colorWasUsed)
		{
			numChanges++;
			output[i] = newColor;
			paletteUsedColors[paletteSize] = newColor;
			paletteSize++;
			if (debug && prevNumChanges != numChanges)
			{
				prevNumChanges = numChanges;
				if (numChanges < 10)
					cout << "0" << numChanges << ": " << output << endl;
				else
					cout << numChanges << ": " << output << endl;
			}
		}
	}

	if (debug)
	{
		if (stripes != output)
			cout << "ERROR!!!: " << stripes << " ==> " << output << " ==> " << numChanges << endl;
		else
			cout << stripes << " ==> " << numChanges << endl;
	}

	return numChanges;
}

//int Paint(string stripes, bool debug)
//{
//	int prevNumChanges = 0;
//	int numChanges = 1;
//	string output(stripes.length(), stripes[0]);
//	if (debug)
//	{
//		cout << "00: " << stripes << endl;
//		cout << "01: " << output << endl;
//	}
//	
//	int paletteSize = 0;
//	char* paletteUsedColors = new char[stripes.length()];
//
//	for (size_t i = 1; i < stripes.length(); i++)
//	{
//		if (output[i] == stripes[i])
//		{
//			paletteSize = 0;
//			continue;
//		}
//
//		char newColor = stripes[i];
//		bool colorWasUsed = false;
//		for (int arrIndex=paletteSize-1; arrIndex >= 0; arrIndex--)
//			if (newColor == paletteUsedColors[arrIndex])
//			{
//				colorWasUsed = true;
//				paletteSize = arrIndex + 1;
//				break;
//			}
//
//		if (!colorWasUsed)
//		{
//			numChanges++;
//			output[i] = newColor;
//			paletteUsedColors[paletteSize] = newColor;
//			paletteSize++;
//			if (debug && prevNumChanges != numChanges)
//			{
//				prevNumChanges = numChanges;
//				if (numChanges < 10)
//					cout << "0" << numChanges << ": " << output << endl;
//				else
//					cout << numChanges << ": " << output << endl;
//			}
//		}
//	}
//
//	if (debug)
//	{
//		if (stripes != output)
//			cout << "ERROR!!!: " << stripes << " ==> " << output << " ==> " << numChanges << endl;
//		else
//			cout << stripes << " ==> " << numChanges << endl;
//	}
//
//	delete[] paletteUsedColors;
//	return numChanges;
//}

int PaintOld(string stripes, bool debug)
{
	int numSweeps = 1;
	string output(stripes.length(), stripes[0]);
	int prevNumChanges = 0;
	if (debug)
	{
		cout << "0: " << stripes << endl;
		cout << "1: " << output << endl;
	}

	for (size_t i = 1; i < stripes.length(); i++)
	{	
		char newColor = stripes[i];
		for (size_t j = i; j < stripes.length(); j++)
		{
			if (output[j] == stripes[j])
				break;

			if (i == j)
				numSweeps++;

			output[j] = newColor;

		}

		if (debug)
		{	
			if (prevNumChanges != numSweeps)
			{
				prevNumChanges = numSweeps;
				cout << numSweeps << ": " << output << endl;
			}
		}
	}

	if (debug)
	{
		if (stripes != output)
			cout << stripes << " ==> " << output << " ==> " << numSweeps << endl;
		else
			cout << stripes << " ==> " << numSweeps << endl;
	}

	return numSweeps;
}

