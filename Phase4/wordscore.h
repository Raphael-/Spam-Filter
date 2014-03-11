#ifndef WORDSCORE_H_
#define WORDSCORE_H_

#include <string>

using namespace std;

/**
 * WordScore objects contain a word and a score
 * Contains constructor, 2 getters, 1 operator overload
 */
class WordScore
{
	string word;
	double score;
public:
	WordScore(const string& w, const double sc) : word(w), score(sc){}
	string getWord() const
	{
		return word;
	}

	double getScore() const
	{
		return score;
	}

	bool operator<(const WordScore& right) const
	{
		return (score < right.score);
	}
};

#endif /* WORDSCORE_H_ */
