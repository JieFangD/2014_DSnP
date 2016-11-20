#include <iostream>
#include <vector>

using namespace std;

bool compare(int a,int b)
{
        if(b>=a)
                return true;
        else
                return false;
}
void mySwap(int &a,int &b)
{
	int tmp=a;
	a=b;
	b=tmp;
}
void selectionSort(vector<int>& array)
{
	for (size_t i = 0, n = array.size(); i < n - 1; ++i) {
		size_t pivot = i;
		for (size_t j = i+1; j < n; ++j) {
			if (!compare(array[pivot], array[j]))
			pivot = j;
		}
		if (pivot != i)
			mySwap(array[pivot], array[i]);
	}
}
int main()
{
	int num,i,x;
	vector<int> arr;
	cout<<"How many numbers? ";
	cin>>num;
	for(i=0;i<num;i++)
	{
		cin>>x;
		arr.push_back(x);
	}
	cout<<"Before sort:\n";
	for(i=0;i<num;i++)
	{
		cout<<arr[i]<<" ";
	}
	cout<<"\nAfter sort:\n";
	selectionSort(arr);
	for(i=0;i<num;i++)
        {
                cout<<arr[i]<<" ";
        }
	cout<<endl;
	return 0;
}	
