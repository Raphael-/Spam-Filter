/*
*Michail Bogdanos p3100123
*Magkos Rafail-Georgios p3100098
*/
#ifndef BASELINECLASSIFIER_H_
#define BASELINECLASSIFIER_H_

/*
* Baseline Classifier for Instances' classification
* Trained by a training Instance Pool
* Classifies judging by the majority of spam/ham Intances in the trainer InstancePool
*/
class BaselineClassifier : public Classifier
{
	bool trainCalled;
	//counter variables
	unsigned ham,spam;
public:

	/*
	* Constructor
	*/
	BaselineClassifier()
	{
		trainCalled=false;
		ham=0;
		spam=0;
	}
	
	/*
	* Training function of the Baseline Classifier
	* Simply counts the number of ham and spam Instances of the trainer InstancePool
	*/ 
	void train(const InstancePool& training)
	{
		trainCalled = true;
		//overwrite previous values
		ham = 0;
		spam=0;
		//count spam and ham instances
		for(unsigned i=0; i<training.getNumberOfInstances(); i++)
		{
			if(training[i].getCategory())
				spam++;
			else
				ham++;
		}
	}
	
	/*
	* Classifies a given Instance
	*/
	bool classify(const Instance& inst) const
	{
		if(!trainCalled)
		{
			cerr <<"BaselineClassifier WARNING: No previous calls of train method."<< endl;
			return false;
		}
		if(ham >= spam)
			return false;
		else
			return true;

	}
};

#endif
