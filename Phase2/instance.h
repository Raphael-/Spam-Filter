/*
*Michail Bogdanos p3100123
*Magkos Rafail-Georgios p3100098
*/

#ifndef INSTANCE_H_
#define INSTANCE_H_
#include <string>
#include <iostream>

using namespace std;
class Instance
{
	
private:
	unsigned featuresNum;	
	bool cat; //true for spam, false for ham
	string filename;
	
	//Dynamically allocated Instance variables
	string* _features;
	unsigned* _featureID;
	unsigned* _frequency;

public:
	Instance(unsigned features = 0); //Default constructor in instance.cpp
	unsigned getNumberOfFeatures() const { return featuresNum; }
	Instance(const Instance& original);
	~Instance() { delete [] _features ; delete [] _featureID ; delete [] _frequency;} //Deconstructor
	
	//get and set methods
	void setCategory(bool category) { cat=category;}
	bool getCategory() const {return cat;}
	void setFileName(const string& fileName) { filename=fileName;}
	string getFileName() const { return filename; }
	void setFeature(unsigned i, const string& feature, unsigned featureID, unsigned frequency){ _features[i]=feature; _featureID[i]=featureID; _frequency[i]=frequency;}
	string getFeature(unsigned i) const { return _features[i]; }
	unsigned getFeatureID(unsigned i) const { return _featureID[i]; }
	unsigned getFrequency(unsigned i) const { return _frequency[i]; }
	
	//= operator overloading
	Instance& operator=(const Instance& right)
        {
          if (this == &right)
            return *this;
            
          //If the two instances are not the same, clear the instance "on the left"  
          delete [] _features ;
          delete [] _featureID ;
          delete [] _frequency;
          
          //Set the new info
          featuresNum=right.getNumberOfFeatures();
          filename=right.getFileName();
          cat=right.getCategory();
          _features=new string[featuresNum];
          _frequency=new unsigned[featuresNum];
          _featureID = new unsigned[featuresNum];
          
          //Set the tokens
          for(unsigned i=0;i<featuresNum;i++)
          {
              _features[i]=right.getFeature(i);
              _featureID[i]=right.getFeatureID(i);
              _frequency[i]=right.getFrequency(i);
          }
          return *this;
        }

	friend ostream& operator<<(ostream& out, const Instance& inst);
	friend istream& operator>>(istream& in, Instance& inst);
};



#endif
