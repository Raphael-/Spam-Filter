/*
*Michail Bogdanos p3100123
*Magkos Rafail-Georgios p3100098
*/
#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

class Classifier
{
	public:
	virtual void train(const InstancePool& trPool) = 0;
	virtual bool classify(const Instance& inst) const = 0;
	virtual ~Classifier() {}
};


#endif
