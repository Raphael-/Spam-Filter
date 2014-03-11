#include <cstdlib>
#include <fstream>
#include "instance.h"
#include "instancepool.h"
using namespace std;
int main()
{
	InstancePool ip;

        ifstream res("results.txt");
        if(res.is_open())
        {
        res>>ip;
        }
        else
        {
            cerr<<"error";
        }
        cout<<"Contents of pool\n";
        cout<<ip;
	//system("PAUSE");
	return 0;
}
