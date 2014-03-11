#ifndef FEATURESELECTOR_H_
#define FEATURESELECTOR_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include "wordscore.h"

class FeatureSelector
{
	const char* hamFileNames;
	const char* spamFileNames;
	const unsigned threshold;
protected:
	//vectors with the words fulfilling the threshold criteria
	vector<WordScore> spam;
	vector<WordScore> ham;

	/**
	 * spamFmeasure calculation
	 */
	double calcSpamFm(unsigned sw, unsigned hw)
	{
		if (sw == 0)
			return 0;
		double spamprec = double(sw) / (sw + hw);
		double spamrec = double(sw) / spamsize;
		return (2 * spamprec * spamrec) / (spamprec + spamrec);
	}

	/**
	 * hamFmeasure calculation
	 */
	double calcHamFm(unsigned sw, unsigned hw)
	{
		if (hw == 0)
			return 0;
		double hamprec = double(hw) / (sw + hw);
		double hamrec = double(hw) / hamsize;
		return (2 * hamprec * hamrec) / (hamprec + hamrec);
	}

	/**
	 * FeatureSelector's main function.
	 * opens the mail files, stores the keywords and their counts at the corresponding vectors
	 * checks for common words in the 2 vectors
	 * calculates the desired measuers and updates the vectors
	 * sorts the 2 vectors in descending order
	 */
	void start()
	{
		map<string, unsigned> hamwords; //the unsigned variable is the H(w) value
		map<string, unsigned> spamwords; ////the unsigned variable is the S(w) value
		string word;

		ifstream file(hamFileNames);
		if (file.is_open())
		{
			//parse the ham filenames
			while (file >> word)
			{
				ifstream msgs(word.c_str());
				if (msgs.is_open())
				{
					//update the ham words' vector
					//word variable also used to parse the actual words apart from the filenames
					while (msgs >> word) hamwords[word]++;
				}
				else
				{
					cerr << "Cannot open file " << word << endl;
				}
				msgs.close();
				hamsize++; //Nham
			}
		}

		file.close();

		file.open(spamFileNames);
		if (file.is_open())
		{
			//parse the spam filenames
			while (file >> word)
			{
				ifstream msgs(word.c_str());
				if (msgs.is_open())
				{
					//update the spam words' vector
					while (msgs >> word) spamwords[word]++;
				}
				else
				{
					cerr << "Cannot open file " << word << endl;
				}
				msgs.close();
				spamsize++; //Nspam
			}
		}

		file.close();

		map<string, unsigned>::const_iterator mapIterator;
		for (mapIterator = spamwords.begin(); mapIterator != spamwords.end(); mapIterator++)
		{
			//check for the common words in spam and ham mails
			if (hamwords.count(mapIterator->first))
			{
				//add the word to the WordScore vectors (if needed) and erase it from the map
				if ((mapIterator->second + hamwords[mapIterator->first]) >= threshold)
				{
					//add the word with its calculated measures
					spam.push_back(
							WordScore(
									mapIterator->first,
									calcSpamFm(mapIterator->second,
											hamwords[mapIterator->first])));
					ham.push_back(
							WordScore(
									mapIterator->first,
									calcHamFm(mapIterator->second,
											hamwords[mapIterator->first])));
				}
				hamwords.erase(mapIterator->first); //erasing the entry means it has been calculated

			}
			else
			{
				//word only found in spam mails: check if it must be added to the vector
				if (mapIterator->second >= threshold)
					spam.push_back(
							WordScore(mapIterator->first,
									calcSpamFm(mapIterator->second, 0)));
			}
		}

		//iterate hamwords map. After the previous erase operations, this map will contain
		//words that only occured in ham messages
		for (mapIterator = hamwords.begin(); mapIterator != hamwords.end();	mapIterator++)
		{
			//word only found in ham mails: check if it must be added to the vector
			if (mapIterator->second >= threshold)
				ham.push_back(
						WordScore(mapIterator->first,
								calcHamFm(0, mapIterator->second)));
		}

		sort(ham.begin(), ham.end());
		sort(spam.begin(), spam.end());
	}//end of start

public:
	static unsigned spamsize; //Nspam
	static unsigned hamsize; //Nham

	//FeatureSelector constructor. Calls the start function
	FeatureSelector(char* hfn, char* sfn, unsigned th) :
			hamFileNames(hfn), spamFileNames(sfn), threshold(th) {	start(); }

	//FeatureSelector's iterator
	class const_iterator
	{
		vector<WordScore> *current; //used to access the spam and ham vectors
		vector<WordScore> *other;
		unsigned index; //used to access the vector elements
		bool vspam; //used to indicate if current vector is spam or ham (true for spam)

		/**
		 * finds the other Fmeasure(ham/spam) of the word w
		 * returns 0 if not found
		 */
		double findScore(string w)
		{
			for (unsigned i = 0; i < (*other).size(); i++)
				if (w.compare(other->at(i).getWord()) == 0)
					return other->at(i).getScore();
			return 0;
		}

	public:
		//iterator constructor
		const_iterator(vector<WordScore> *pt, vector<WordScore> *ot, unsigned i, bool b):current(pt), other(ot), index(i), vspam(b){};

		//operator overloads
		bool operator!=(const const_iterator& right)
		{
			return (index != right.index);
		}

		const WordScore operator++(int)
		{
			index++;
			return (*current)[index];
		}

		/**
		 * sends tou the output stream the desired format
		 * (word, if -scores is disabled)
		 * (word spamFmeasure hamFmeasure, is -scores is enabled
		 */
		void print(ostream &out, bool b)
		{
			WordScore p = current->at(index);
			if (b)
			{
				double sc2 = findScore(current->at(index).getWord());
				//check which is the current vector and send to the out stream the right format
				if(vspam)
					out<<p.getWord()<<" "<<p.getScore()<<" "<<sc2<<endl;
				else
					out<<p.getWord()<<" "<<sc2<<" "<<p.getScore()<<endl;
			}
			else
				out<<p.getWord()<<endl;
	}

};
	//FeatureSelector functions for the iterator initialization in main
const const_iterator hamBegin()
{
	return const_iterator(&ham,&spam,0,false);
}

const const_iterator hamEnd()
{
	return const_iterator(&ham,&spam,ham.size()-1,false);
}

const const_iterator spamBegin()
{
	return const_iterator(&spam,&ham,0,true);
}

const const_iterator spamEnd()
{
	return const_iterator(&spam,&ham,spam.size()-1,true);
}

};
unsigned FeatureSelector::spamsize = 0;
unsigned FeatureSelector::hamsize = 0;

#endif /* FEATURESELECTOR_H_ */
