#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <functional>  
using namespace std;

int totalManualErrors = 0;

void Print(string& output, int& numTotalSweeps, int& numThread);
int Paint(string& stripes, bool debug = false);
void PaintOptSubs(string& stripes, string& output, int startPosToAnalize, int endPosToAnazlize, int& numSweeps, int& threadId, bool debug);
bool ProcessFile(string filename);
void Tests();
int colorWidth[26];

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

	ProcessFile(filename);

	clock_t endClock = clock();
	time_t endTime = time(0);
	char* dtEnd = ctime(&endTime);
	cout << "Program ended at " << dtEnd;
	double elapsed_secs = double(endClock - beginClock) / CLOCKS_PER_SEC;
	cout << "Running time in seconds: \"" << elapsed_secs << "\"" << endl;
	cout << "=================================" << endl;


	Tests();
	system("pause");
}

void TestManualInput(string stripes, int expectedResult, bool debug)
{
	int result = Paint(stripes, debug);
	if (expectedResult != result)
	{
		if (debug)
			cout << "Expected: " << expectedResult << endl;
		totalManualErrors++;
	}

	if (debug)
		cout << "=================================" << endl;
}

void Tests()
{
	bool debug = true;
	totalManualErrors = 0;

	//example pdf:
	TestManualInput("YBYB", 3, debug);
	TestManualInput("BGRGB", 3, debug);
	TestManualInput("YBYCYRY", 4, debug);
	TestManualInput("ABCD", 4, debug);
	TestManualInput("AAAA", 1, debug);
	TestManualInput("A", 1, debug);
	TestManualInput("", 0, debug);
	TestManualInput("AABBBCCCDABABBBB", 6, debug);

	//TestManualInput("YBXCXBYYBBYM", 6, debug);
	//TestManualInput("ABCACB", 4, debug);
	//TestManualInput("JIZOZXJSOZXIJS", 10, debug);

	////input1:
	//TestManualInput("BECBBDDEEBABDCADEAAEABCACBDBEECDEDEACACCBEDABEDADD", 26, debug); //line 2, me=>28
	//TestManualInput("VYJICZOZZXJSTOZXGRPNIJSUH", 20, debug); //line 11, me=>21
	//TestManualInput("VLUGPXIOLYOGMVGYLXKYDOHLK", 20, debug); //OK if descending or widest, line 17, me=>21

	//														 //input2:
	//TestManualInput("CBAKRXDCDXTQ", 10, true); //BAD if descending, line 731, me=>11
	//TestManualInput("YGVVPYPPWGDDNNI", 9, debug); //OK if descending, line 1067, me=>10 

	if (totalManualErrors != 0)
	{
		cout << "Total manual errors: " << totalManualErrors << endl;
		cout << "=================================" << endl;
	}
}

void ChooseNextStripesGreedy(string& stripes, int startPosToAnalize, int endPosToAnazlize, int colorStartPos[26], int colorEndPos[26], int arrWidestColor[26])
{
	int	widestColor = -1;
	int widestColorWidth = -1;
	int widestColorStartPos = 99;
	int widestColorEndPos = -1;

	for (int i = startPosToAnalize; i <= endPosToAnazlize; i++)
	{
		int color = int(stripes[i]) - 65;
		if (i < colorStartPos[color])
			colorStartPos[color] = i;
		if (i > colorEndPos[color])
			colorEndPos[color] = i;

		if (colorEndPos[color] - colorStartPos[color] + 1 > widestColorWidth)
		{
			widestColor = color;
			widestColorStartPos = colorStartPos[color];
			widestColorEndPos = colorEndPos[color];
			widestColorWidth = colorEndPos[color] - colorStartPos[color] + 1;
		}
	}

	arrWidestColor[0] = widestColor;

	if (widestColorWidth <= 2)
	{
		return;
	}
	else
	{
		int arrIndex = 1;
		for (int i = 0; i < 26; i++)
		{
			if (colorEndPos[i] - colorStartPos[i] <= 1)
				continue;

			//Now removes any color that should not be in the returned array
			if (i == widestColor)
				continue;

			if ((colorStartPos[arrWidestColor[i]] > widestColorStartPos && colorEndPos[arrWidestColor[i]] < widestColorEndPos) ||
				(colorStartPos[arrWidestColor[i]] > widestColorEndPos) ||
				(colorEndPos[arrWidestColor[i]] < widestColorStartPos))
				continue;
			
			arrWidestColor[arrIndex++] = i;
		}
	}
}

int Paint(string& stripes, bool debug)
{
	int colorStartPos[26] = { 99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99 };
	int colorEndPos[26] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
	int widestColor[26]{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

	int minNumSweps = 100000;
	//Greedy choice:
	ChooseNextStripesGreedy(stripes, 0, stripes.length() - 1, colorStartPos, colorEndPos, widestColor);

	int threadId = 1;
	for (int i = 0; i < 26 && widestColor[i] != -1; i++, threadId++)
	{
		if (widestColor[i] == -1)
			continue;

		string output(stripes.length(), widestColor[i] + 65);
		int numSweeps = 1;
		if (debug)
		{
			cout << "Th: " << threadId << ", Sw: 00, " << stripes << endl;
			Print(output, numSweeps, threadId);
		}

		int startPosToAnalyze = colorStartPos[widestColor[i]];
		int endPosToAnalyze = colorEndPos[widestColor[i]];

		PaintOptSubs(stripes, output, 0, startPosToAnalyze - 1, numSweeps, threadId, debug);
		PaintOptSubs(stripes, output, startPosToAnalyze + 1, endPosToAnalyze - 1, numSweeps, threadId, debug);
		PaintOptSubs(stripes, output, endPosToAnalyze + 1, stripes.length() - 1, numSweeps, threadId, debug);

		if (numSweeps < minNumSweps)
			minNumSweps = numSweeps;
	}

	return minNumSweps == 100000 ? 0 : minNumSweps;
}

void ChangeSmallestBlock(string& stripes, string& output, int startPosToAnalize, int endPosToAnazlize, int& numSweeps, int& threadId, bool debug)
{
	char newColor = '*';
	char prevColor = '*';

	for (int i = startPosToAnalize; i <= endPosToAnazlize; i++)
	{
		if (output[i] != stripes[i])
		{
			newColor = stripes[i];
			if (prevColor != newColor)
			{
				prevColor = newColor;
				output[i] = newColor;
				numSweeps++;

				if (debug)
					Print(output, numSweeps, threadId);
			}
		}
	}
}

void PaintOptSubs(string& stripes, string& output, int startPosToAnalyze, int endPosToAnalyze, int& numSweeps, int& threadId, bool debug)
{
	if (endPosToAnalyze < startPosToAnalyze)
		return;

	if (endPosToAnalyze - startPosToAnalyze <= 1)
	{
		ChangeSmallestBlock(stripes, output, startPosToAnalyze, endPosToAnalyze, numSweeps, threadId, debug);
		return;
	}

	int prevStartPosToAnalyze = startPosToAnalyze;
	int prevEndPosToAnazlyze = endPosToAnalyze;

	int colorStartPos[26] = { 99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99 };
	int colorEndPos[26] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
	//Greedy choice:
	int widestColor[26]{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
	ChooseNextStripesGreedy(stripes, startPosToAnalyze, endPosToAnalyze, colorStartPos, colorEndPos, widestColor);

	for (int i = 0; i < 26 && widestColor[i] != -1; i++)
	{
		int mustBePainted = 0;
		for (int j = startPosToAnalyze; j <= endPosToAnalyze; j++)
		{
			if (output[j] != widestColor[i] + 65)
			{
				mustBePainted = 1;
				output[j] = widestColor[i] + 65;
			}
		}

		//if (i > 0)
		//	cout << "WARNING!!" << endl;	

		if (mustBePainted)
		{
			numSweeps++;
			if (debug)
				Print(output, numSweeps, threadId);
		}

		int startPosToAnalyze = colorStartPos[widestColor[i]];
		int endPosToAnalyze = colorEndPos[widestColor[i]];

		PaintOptSubs(stripes, output, prevStartPosToAnalyze, startPosToAnalyze - 1, numSweeps, threadId, debug);
		PaintOptSubs(stripes, output, startPosToAnalyze + 1, endPosToAnalyze - 1, numSweeps, threadId, debug);
		PaintOptSubs(stripes, output, endPosToAnalyze + 1, prevEndPosToAnazlyze, numSweeps, threadId, debug);
	}
}

bool ProcessFile(string filename)
{
	string ouputfile = filename + ".output.txt";
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
		cout << "Output file saved as \"" << ouputfile << "\"" << endl;
	}
	else
	{
		cout << "ERROR!!!: Unable to open file" << endl;
		return false;
	}

	return true;
}

void Print(string& output, int& numSweeps, int& threadId)
{
	if (numSweeps < 10)
		cout << "Th: " << threadId << ", Sw: 0" << numSweeps << ", " << output << endl;
	else
		cout << "Th: " << threadId << ", Sw: " << numSweeps << ", " << output << endl;
}
