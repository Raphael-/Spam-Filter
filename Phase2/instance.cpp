/*
*Michail Bogdanos p3100123
*Magkos Rafail-Georgios p3100098
*/

#include "instance.h"
#include <stdlib.h>

using namespace std;

//Default Instance constructor
//Contains one mail's info
Instance::Instance(unsigned features) 
{
    featuresNum = features;			//Number of features
    _features = new string[featuresNum];	//Mail's features
    _frequency = new unsigned[featuresNum];	//Feature frequency
    _featureID = new unsigned[featuresNum];	//Feature ID
}

//Copy constructor
Instance::Instance(const Instance& original) 
{
    featuresNum = original.featuresNum;
    filename=original.getFileName();
    cat=original.getCategory();			//Category check	
    _features = new string[featuresNum];
    _frequency = new unsigned[featuresNum];
    _featureID = new unsigned[featuresNum];
    //Loop which copies the 3 arrays' contents to the new Instance
    for (unsigned i = 0; i < featuresNum; i++) 
    {
        _features[i] = original._features[i];
        _frequency[i] = original._frequency[i];
        _featureID[i] = original._featureID[i];
    }
}

//<< operator overloading
ostream & operator<<(ostream& out, const Instance& inst)
{
	// Output message, displays mail info
	// Calls the instance methods to get the info
    out << "< message file = \"" << inst.getFileName() << "\" category = \"" << (inst.getCategory() ? "spam" : "ham") << "\" features = \"" << inst.getNumberOfFeatures() << "\" >" << endl;
    for (unsigned i = 0; i < inst.getNumberOfFeatures(); i++)
    {
        out << "< feature token = \"" << inst.getFeature(i) << "\" id = \"" << inst.getFeatureID(i) << "\" freq = \"" << inst.getFrequency(i) << "\" />" << endl;
    }
    out << "</ message>" << endl;
    return out;		//return the stream

}

//>> operator overloading
istream& operator>>(istream& in, Instance& inst)
{
    	string line;		//string for storing the lines
    	getline(in,line);	//Reads from the stream
    	if(line.find("< messagecollection messages")!=line.npos) //Check the format
    	{
    		getline(in,line);	//Skip the useless line
    	}
    	if(line.compare("")==0) //Ignore empty lines that stream might read
    	{
    		getline(in,line);
    	}
    	if(line.find("< message file = ")!=line.npos) //If a mail is found, start reading
	{
		//Creation of a temp Instance with the number of features of the mail read
		//atoi() method used to convert string to integer
		//substr(), find_last_of(), length() and c_str() methods used to get the string containing the features number
	 	Instance temp(atoi(line.substr(line.find_last_of("=")+3,line.length()-line.find_last_of("\"")-1).c_str()));
	 	
	 	//Setting the temp Instance's variables
	 	temp.setFileName(line.substr(line.find("=")+3,line.find("\"",18)-line.find("\"")-1));
	 	temp.setCategory(line.substr(line.find("=",line.find(".txt\""))+3,line.find("features")-line.find("=",line.find(".txt\""))-5).compare("spam")==0?true:false);
	 	getline(in,line);	//Next line read
	 	
	 	//Loop which reads the tokens
	 	for(unsigned i=0;i<temp.getNumberOfFeatures();i++)
	 	{
	 		//One command which sets each feature
	 		//substr(), find(), atoi() and find_last_of() methods used to get the token info from the standard output.txt file
	 		temp.setFeature(i,
	 		line.substr(line.find("=")+3,line.find("id")-line.find("=")-5),
	 		atoi((line.substr(line.find("id")+6,line.find("freq")-line.find("id")-8).c_str())),
	 		atoi((line.substr(line.find_last_of("=")+3,line.find_last_of("\"")-line.find_last_of("=")-3)).c_str()));
	 		getline(in,line); //Next line read
	 	}
	 	
	 	//End of mail check
	 	//Checks the string until the end of the "message" word so as to function properly with both "</ message >" and "</ message>" formats
	 	if(line.find("</ message")!=line.npos)
	 	{
	 		inst=temp;
                 	getline(in,line); //empty line ignore
	 		return in; //return the stream
	 	}
	 	else //Bad file error
	 	{
	 		cerr<<"Message entity in file "<<temp.getFileName()<< "is not terminated by </message> tag"<<endl;
	 		return in;
	 	}
	 }
	 return in; //return the stream
}