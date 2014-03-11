/*
*Michail Bogdanos p3100123
*Magkos Rafail-Georgios p3100098
*/
#ifndef KNNCLASSIFIER_H_
#define KNNCLASSIFIER_H_

#include "classifier.h"
#include "math.h"
#include <map>

/*
* KNN Classifier for Instances' classification
* Trained by a training Instance Pool
* Classifies judging by the given Instance's Euclidean distance from each training Pool Instance
*/
class KNNClassifier : public Classifier
{
	//memory is the InstancePool that stores the trainer InstancePool
	InstancePool memory;
	//number of neighbouring Instances (nearest to the given Instance) to pick
	unsigned short _kIn;
	//"flag" variable which indicates if the classifier has been trained
	bool trainCalled;
	
	/*
	* Sorts the parallel arrays num and inst, lowest to greatest number 
	* Is used in the "classify" method
	*/
	void sort(float *num, unsigned size, bool *inst) const
	{
		unsigned i, j, flag = 1;
		float temp;
		bool tempbool;
		
		//bubblesort
		for(i = 1; (i <= size) && flag; i++)
		{
			flag = 0;
			for (j=0; j < (size-1); j++)
			{
				if (num[j+1] < num[j])
				{
					temp = num[j];
					tempbool = inst[j];
					num[j] = num[j+1];
					inst[j] = inst[j+1];
					num[j+1] = temp;
					inst[j+1] = tempbool;
					flag = 1;
				}
			}
		}
		return;
	}

public:

	/*
	* Constructor
	*/
	KNNClassifier(unsigned short kIn = 5)
	{
		_kIn=kIn;
		trainCalled=false;
	}

	/*
	* Calculates the Euclidean distance of 2 Instances
	*/
	static float distance(const Instance& inst1,const Instance& inst2)
	{
		//sum variable
		float sum=0.0f;
		//map which finally stores the (xi-yi) subtractions
		map<unsigned,float> freq;
		//iterator for the map
		map<unsigned,float>::iterator it;
		
		//store the first Instance's keywords' IDs and frequencies
		for(unsigned i=0; i < inst1.getNumberOfFeatures(); i++)
		{
			freq[inst1.getFeatureID(i)] = inst1.getFrequency(i);
		}
		//while obtaining the keywords of the second Instance:
		//for each keyword, updates its frequency in the map using the ternary operator so as to avoid unsigned number errors at the subtraction (xi-yi)
		//if the keyword does not exist, the map automatically adds it
		for(unsigned i=0; i < inst2.getNumberOfFeatures(); i++)
		{
			freq[inst2.getFeatureID(i)] = inst2.getFrequency(i) > freq[inst2.getFeatureID(i)] ? inst2.getFrequency(i) - freq[inst2.getFeatureID(i)] : freq[inst2.getFeatureID(i)] - inst2.getFrequency(i);
		}
		//using the iterator, obtain the map's float type contents to calculate the required sum
		for(it=freq.begin(); it!=freq.end(); it++)
		{
			sum += pow(it->second, 2.0f); // exponentiation of each (xi-yi) using 2 as the exponent
		}
		//return the final value (square root of sum)
		return sqrt(sum);
	}

	/*
	* Training function of the KNN Classifier
	* Simply stores the training pool for use in the "classify" method
	*/
	void train(const InstancePool& trainingPool)
	{
		memory=trainingPool;
		trainCalled=true;
	}

	/*
	* Classifies a given Instance
	*/
	bool classify(const Instance& inst) const
	{
		if(!trainCalled) //If the train method hasn't been called, set inst's category to false (ham)
		{
			cerr << "KNNClassifier WARNING: No previous calls of train method." << endl;
			return false;
		}
		
		//counter indicating how many of the kIn nearest Instances are spam
		unsigned spam=0;
		//array where the distance of each instance is stored
		float *p = new float[memory.getNumberOfInstances()];
		//array parallel to the previous one, indicates if the instance is spam
		bool *ptr = new bool[memory.getNumberOfInstances()];
		
		//fill the arrays
		for(unsigned i=0; i<memory.getNumberOfInstances(); i++)
		{
			p[i] = distance(inst,memory[i]);
			ptr[i] = memory[i].getCategory();
		}
		
		//sort the arrays (lowest to greatest)
		sort(p, memory.getNumberOfInstances(), ptr);
		//obtain the kIn nearest Instances' categories, count the spam ones
		for(unsigned i=0; i<_kIn; i++)
		{
			if(ptr[i]) spam++;
		}
		//delete the dynamically allocated memory space
		delete []p;
		delete []ptr;
		//returns the category of the majority of the nearest Instances' category
		if(spam >= _kIn/2.0f)
			return true;
		else
			return false;
	}
};
#endif
