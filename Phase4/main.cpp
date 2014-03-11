#include <ctype.h>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include "exception.h"
#include "featureselector.h"

using namespace std;

bool isNumeric(const char* str);
bool fexists(const char *filename);
void checkArgsNum(int &length) throw (IllegalNumberOfArgumentsException);
void checkValidityOfArgs(int &num, char *args[], int startpos);
void reEnterFile(char* s);
unsigned defineThreshold(int len, bool scores, char *arg[]);

int main(int argc, char* argv[])
{
	//mode check (compare the second input argument with "-scores")
	bool scores_enabled;
	scores_enabled = strcmp(argv[1], "-scores") == 0 ? true : false;

	//check for valid number of arguments
	try
	{
		checkArgsNum(argc);
		if (scores_enabled)
			checkValidityOfArgs(argc, argv, 4);
		else
			checkValidityOfArgs(argc, argv, 3);
	} catch (IllegalNumberOfArgumentsException& e)
	{
		cerr << e.slot << e.message << endl;
		exit(1); //exit the program and return 1 to OS (abnormal termination)
	}

	unsigned threshold = defineThreshold(argc, scores_enabled, argv);
	unsigned hamKeywords, spamKeywords;

	char *spam = new char[50];
	char *ham = new char[50];

	//parse the ham and spam index numbers
	hamKeywords = ((scores_enabled) ? atoi(argv[2]) : atoi(argv[1]));
	spamKeywords = ((scores_enabled) ? atoi(argv[3]) : atoi(argv[2]));

	//parse the names of the files containing the ham and spam file names (strcpy used to copy
	strcpy(ham, ((scores_enabled) ? argv[4] : argv[3]));
	strcpy(spam, ((scores_enabled) ? argv[5] : argv[4]));

	cout << "scores " << scores_enabled << " hamKeywords " << hamKeywords
			<< " spamKeywords " << spamKeywords << " ";

	//FeatureSelector initialized, desired data sent to cout
	FeatureSelector selector(ham, spam, threshold);
	FeatureSelector::const_iterator iter = selector.hamBegin();
	unsigned featuresPrinted = 0;
	while (featuresPrinted < hamKeywords && iter != selector.hamEnd())
	{
		iter.print(cout, scores_enabled);
		iter++;
		featuresPrinted++;
	}
	iter = selector.spamBegin();
	featuresPrinted = 0;
	while (featuresPrinted < spamKeywords && iter != selector.spamEnd())
	{
		iter.print(cout, scores_enabled);
		iter++;
		featuresPrinted++;
	}

	delete[] spam;
	delete[] ham;
	return 0;
} //main

/**
 * Error handling in case of illegal number of arguments.
 */
void checkArgsNum(int &length) throw (IllegalNumberOfArgumentsException)
{
	if (length < 5 || length > 7)
	{
		throw IllegalNumberOfArgumentsException(" is not a valid number of arguments. Arguments' number must be 6 (or 7 if -scores is activated)", length);
	}
}

/**
 * Error handling in case of illegal type of argument(s).
 * @param &num is argc
 * @param *args[] is argv
 * @param startpos depends on the mode (-score)
 */
void checkValidityOfArgs(int &num, char *args[], int startpos)
{
	for (int i = startpos; i < startpos + 2; i++)
	{
		if (!fexists(args[i]))
		{
			cerr << "File " << i << args[i]
					<< " doesn't exist. Type a new file name: " << endl;
			do
			{
				reEnterFile(args[i]);
				if (fexists(args[i]))
					break;
				else
					cerr << args[i] << " doesn't exist as well. Try again: "
							<< endl;
			} while (1);
		}
	}
}

/**
 * Checks the corresponding argument to define the threshold number.
 */
unsigned defineThreshold(int len, bool scores, char *arg[])
{
	//7 arguments indicate that -scores is enabled
	if (scores && len == 7)
	{
		if (isNumeric(arg[6]))
			return atoi(arg[6]);
		else
			cerr
					<< "Invalid value for argument threshold. Restoring default value (10)."
					<< endl;
	}
	else if (!scores && len == 6) //scores disabled
	{
		if (isNumeric(arg[5]))
			return atoi(arg[5]);
		else
			cerr
					<< "Invalid value for argument threshold. Restoring default value (10)."
					<< endl;
	}

	return 10;
}

/**
 * Checks if the file exists.
 * If the file exists, ifstream returns 1
 */
bool fexists(const char *filename)
{
	ifstream file(filename);
	return file;
}

/**
 * Used in cases of illegal type of argument(s) for re-parsing a string (through cin).
 */
void reEnterFile(char* s)
{
	gets(s);
}

/**
 * Checks if a given char sequence is a number
 */
bool isNumeric(const char* str)
{
	for (; *str; str++)
	{
		if (!isdigit(*str))
			return false;
	}
	return true;
}
