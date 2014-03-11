/*
*Michail Bogdanos p3100123
*Magkos Rafail-Georgios p3100098
*/
#ifndef CLASSIFIEREVALUATOR_H_
#define CLASSIFIEREVALUATOR_H_

#include "classifier.h"

/*
* Classifier Evaluator
* Evaluates each classifier's performance, after training them
*/
class ClassifierEvaluator
{
	 //values useful for statistics
	float truePositives,trueNegatives,falsePositives,falseNegatives;
	//total Instances in the testPool(also useful for statistics)
	unsigned total; 
public:

	/*
	* Constructor
	* parameters: classifier to be evaluated, the trainer InstancePool and the evaluating test Pool
	*/
	ClassifierEvaluator(Classifier& classifier, const InstancePool& trainingPool, const InstancePool& testPool)
	{
		bool c,d;
		classifier.train(trainingPool);
		//Initialize
		truePositives=0;
		trueNegatives=0;
		falsePositives=0;
		falseNegatives=0;
		total=testPool.getNumberOfInstances();
		//for all the testPool Instances
		for(unsigned i = 0; i < total; i++)
		{
			//store the classifier's result
			c = classifier.classify(testPool[i]);
			//and the real category
			d = testPool[i].getCategory();
			//add to the corresponding counter
			if(c && d)
			{
				++truePositives;
			}
			else if(!c && !d)
			{
				++trueNegatives;
			}
			else if(c && !d)
			{
				++falsePositives;
			}
			else if(!c && d)
			{
				++falseNegatives;
			}
		}
		//Print the numbers
		//cout<<"falseN "<<falseNegatives<<" falseP "<<falsePositives<<" trueP "<<truePositives<<" trueN "<<trueNegatives<<endl;
	}
	
	/*
	* Calculates the evaluated classifier's accuracy
	*/
	float getAccuracy() const
	{
		return  (truePositives + trueNegatives) / total;
	}
	
	/*
	* Calculates the evaluated classifier's precision
	*/
	float getPrecision() const
	{
		if(truePositives + falsePositives) //If the sum is non-zero
			return truePositives / (truePositives + falsePositives);
		else
			return 0.0f;
	}
	
	/*
	* Calculates the evaluated classifier's recall
	*/
	float getRecall() const
	{
		if(truePositives + falseNegatives) //If the sum is non-zero
			return truePositives / (truePositives + falseNegatives);
		else
			return 0.0f;
	}
};


#endif
