/*
*Michail Bogdanos p3100123
*Magkos Rafail-Georgios p3100098
*/

#include "instancepool.h"
#include "instance.h"
#include <vector>
#include <cstdlib>

using namespace std;

//Copy constructor
InstancePool::InstancePool(const InstancePool& original)
{
    instances=original.instances;
}

//>> overloading for InstancePool
istream& operator>>(istream& in, InstancePool& ip)
{
   ip.instances.clear(); //clears any previous content of pool
   Instance temp;
   string line; //stores the line read
   getline(in,line); //read next line
   
   //Reads number of instances that the pool is going to contain
   //atoi(), substr(), length(), find_last_of() and c_str() methods used to locate, get and convert the string to the desired integer
   unsigned limit=atoi((line.substr(line.find_last_of("=")+3,line.length()-line.find_last_of("\""))).c_str());
   
   getline(in,line); //skip useless line
   //Loop which reads (through a stream) and saves instances to the pool 
   for(unsigned i=0; i<limit; i++)
   {
       in>>temp;    //instance << operator
       ip.instances.push_back(temp); //push_back() used for the InstancePool vector
   }
   return in; //return the stream
}

//<< overloading for InstacePool
ostream& operator<<(ostream& out, const InstancePool& ip)
{
    //Display the contents of the instancepool vector, the instances
    out<<"< messagecollection messages = \""<<ip.getNumberOfInstances()<<"\" >\n\n";
    for(unsigned i=0;i<ip.getNumberOfInstances();i++)
    {
        //Instance << operator and InstancePool [] operator overloaded
    	out<<ip[i]<<endl;
    }
    out<<"</ messagecollection >\n";
    return out; //return the stream

}
