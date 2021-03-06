#include <iostream>
#include "p1a.h"

using namespace std;

int i;
void printSize()
{
	cout<<"The size of class P1a is "<<sizeof(P1a);
}
void printStaticArraySize()
{ 
	P1a arr1b_1[5];
	cout<<"\n============================\nAddresses of arr1b_1[5] are:\n";
	for(i=0;i<5;i++)
	{
		cout<<"&arr1b_1["<<i<<"]: "<<&arr1b_1[i]<<endl;
	}
}
void printDynamicArraySize()
{
	P1a *arr1b_2=new P1a [5];
	cout<<"============================\nAddresses of arr1b_2[5] are:\n";
	for(i=0;i<5;i++)
        {
                cout<<"&arr1b_2["<<i<<"]: "<<&arr1b_2[i]<<endl;
        }
	cout<<"&arr1b_2   : "<<&arr1b_2;
}
void printPointerArraySize()
{
	P1a **arr1b_3 = new P1a *[5];
	for (size_t j = 0; j < 5; ++j)
		arr1b_3[j] = new P1a;
	cout<<"\n============================\nAddresses of arr1b_3[5] are:\n";
        for(i=0;i<5;i++)
        {
                cout<<"&arr1b_3["<<i<<"]: "<<&arr1b_3[i]<<endl;
        }
	cout<<"Contents of arr1b_3[5] are:\n";
	for(i=0;i<5;i++)
        {
                cout<<"arr1b_3["<<i<<"]: "<<arr1b_3[i]<<endl;
        }
	cout<<"&arr1b_3  : "<<&arr1b_3<<endl;
}	
