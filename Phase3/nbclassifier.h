/*
*Michail Bogdanos p3100123
*Magkos Rafail-Georgios p3100098
*/
#ifndef NBCLASSIFIER_H_
#define NBCLASSIFIER_H_

#include <map>
#include <set>
#include "math.h"

/*
* Naive Bayes Classifier for Instances' classification
* Trained by a training Instance Pool
* Classifier based on applying Bayes' theorem
*/
class NaiveBayesClassifier : public Classifier
{
	//percentage of spam and ham out of all training Instances (possibility that an instance is spam/ham)
	float Pspam, Pham;
	//number of spam and ham Instances
	unsigned Mspam,Mham;
	//maps where for each keyword, the conditional required for the Bernoulli variable calculation is stored
	//initially they contain the MTspam and MTham variables (at the training method)
	map<unsigned, double> PTspam;
	map<unsigned, double> PTham;
	//set containing the ID's found in all Instances
	set<unsigned> keywords;
	//indicates if the classifier has been trained
	bool trainCalled;

public:
	/*
	* Constructor
	*/
	NaiveBayesClassifier()
	{
		trainCalled = false;
		Mspam = 0;
		Mham = 0;
		Pspam = 0.0f;
		Pham = 0.0f;
	}
	
	/*
	* Training function of the Naive Bayes Classifier
	* Calculates the 
	*/
	void train(const InstancePool& training)
	{
		//get the category of each Instance in the trainer InstancePool
		for (unsigned i = 0; i < training.getNumberOfInstances(); i++)
		{
			//check if it's spam or ham, add to the Mspam(Mham) variable and for each keyword add to the MTspam(MTham) value
			//note: as mentioned above, the PTspam(PTham) map initially contains the MTspam(MTham) values
			if (training[i].getCategory()) //true == spam
			{
				Mspam++;
				for (unsigned j=0; j < training[i].getNumberOfFeatures(); j++)
				{
					PTspam[training[i].getFeatureID(j)]++;
				}
			}
			else

			{
				Mham++;
				for (unsigned j=0; j < training[i].getNumberOfFeatures(); j++)
				{
					PTham[training[i].getFeatureID(j)]++;
				}
			}
		}
		//calculate the possibility that an instance is spam/ham
		Pspam = (float)Mspam / training.getNumberOfInstances();
		Pham = 1-Pspam;
		
		//using an iterator, calculate the final PTspam and PTham values
		map<unsigned, double>::iterator p;		
		for(p = PTham.begin(); p != PTham.end(); p++)
		{
			p->second = (1 + p->second) / (2+Mham);
			keywords.insert(p->first);
		}
		for(p = PTspam.begin(); p != PTspam.end(); p++)
		{
			p->second=(1 + p->second) / (2+Mspam);
			keywords.insert(p->first);
		}
		trainCalled=true;
	} //end of train

	/*
	* Classifies a given Instance using the given classification inequality
	*/
	bool classify(const Instance& inst) const
	{
		if(!trainCalled) //If the train method hasn't been called, set inst's category to false (ham)
		{
			cerr <<"NaiveBayesClassifier WARNING:No previous calls of train method."<< endl;
			return false;
		}
		//inequality's "left" and "right" parts
		float left,right;
		//iterators used to read the map and the set
		map<unsigned, double>::const_iterator p;
		set<unsigned>::const_iterator setIterator;
		//initial calculations
		left=log(Pspam);
		right=log(Pham);
		//flag indicating that a keyword has been found
		bool found=false;
		
		//for each keyword in the set
		for(setIterator = keywords.begin(); setIterator != keywords.end();setIterator++)
		{
			//and each keyword in the given Instance
			for(unsigned i=0;i<inst.getNumberOfFeatures();i++)
			{
				//check which keywords of the set are contained in the Instance
				if(*setIterator==inst.getFeatureID(i))
				{
					found=true;
					//if a keyword is contained in the instance
					//calculate left
					if((p = PTspam.find(inst.getFeatureID(i))) != PTspam.end())
						//keyword found in spam instance(s) during training
						left += log(p->second);
					else
						left += log(1.0f/(2+Mspam));
					//calculate right
					if((p = PTham.find(inst.getFeatureID(i))) != PTham.end())
						//keyword found in ham instance(s) during training
						right+=log(p->second);
					else
						right+=log(1.0f/(2+Mham));
					break;
				}
			}
			if(!found)//keyword was not found in the Instance
			{
				//calculate left
				if((p=PTspam.find(*setIterator))!=PTspam.end())
					//keyword found in spam instance(s) during training
					left+=log(1.0f-p->second);
				else
					left+=log(1.0f-(1.0f/(2+Mspam)));
				//calculate right
				if((p=PTham.find(*setIterator))!=PTham.end())
					//keyword found in ham instance(s) during training
					right+=log(1.0f-p->second);
				else
					right+=log(1.0f-(1.0f/(2+Mham)));
			}
			//reset the flag
			found=false;
		}
		//return the result
		if(left>right)
			return true;
		else
			return false;
	}
};

#endif