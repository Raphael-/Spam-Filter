/*
*Michail Bogdanos p3100123
*Magkos Rafail-Georgios p3100098
*/

#ifndef INSTANCEPOOL_H_
#define INSTANCEPOOL_H_

#include <vector>
#include "instance.h"

class InstancePool
{
	
private:
    vector<Instance> instances; //vector containing the instances
    
public:
	//Default, empty constructor
	InstancePool(){ }
	
	//Deconstructor is not declared because vector class has its own
	InstancePool(const InstancePool& original);
	unsigned getNumberOfInstances() const
        {
            return instances.size();
        }
        
        //[] operator overloading
	const Instance& operator[](unsigned index) const
        {
            return instances[index];
        }
        
        //= operator overloading
	InstancePool& operator=(const InstancePool& right)
        {
            if (this == &right)
                return *this;
            this->instances=right.instances; // Assigns the "right" InstancePool (vector) to the "left" one using the [] overload of the vector class
            return *this;
        }
        
	friend istream& operator>>(istream& in, InstancePool& ip);
	friend ostream& operator<<(ostream& out, const InstancePool& ip);

};


#endif
