#include <iostream>
#include <vector>

using namespace std;

template <class T>
class Compare
{
public:
	virtual bool operator()(T&,T&)const=0;
};
template <class T>
class Less:public Compare<T>
{
public:
	bool operator()(T &a,T &b)const{
		if(b>=a)
			return true;
		else
			return false;
	}
};
template <class T>
class Greater:public Compare<T>
{
public:
	bool operator()(T &a,T &b)const{
                if(b<=a)
                        return true;
                else
                        return false;
	}
};
template <class T>
void mySwap(T &a,T &b)
{
	T tmp=a;
	a=b;
	b=tmp;
}
template <class T>
void selectionSort(vector<T>& array,const Compare<T>& compare)
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
	string s;
	double d;
	int i,num;
	vector<string> s_arr;
	vector<double> d_arr;
	cout<<"How many strings? ";
	cin>>num;
	for(i=0;i<num;i++)
	{
		cin>>s;
		s_arr.push_back(s);
	}
	cout<<"Before sort:\n";
	for(i=0;i<num;i++)
	{
		cout<<s_arr[i]<<" ";
	}
	cout<<"\nAscending sort:\n";
	selectionSort<string>(s_arr,Less<string>());
	for(i=0;i<num;i++)
        {
                cout<<s_arr[i]<<" ";
        }
	cout<<"\n\nHow many doubles? ";
	cin>>num;
	for(i=0;i<num;i++)
        {
                cin>>d;
                d_arr.push_back(d);
        }
	cout<<"Before sort:\n";
        for(i=0;i<num;i++)
        {
                cout<<d_arr[i]<<" ";
        }
	cout<<"\nDescending sort:\n";
	selectionSort<double>(d_arr,Greater<double>());
        for(i=0;i<num;i++)
        {
                cout<<d_arr[i]<<" ";
        }
	cout<<endl;
	return 0;
}
