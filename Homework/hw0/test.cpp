#include <iostream> 
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
using namespace std; 

class Data {
	public:
		Data(size_t i){
			_cols=new int[i];
		}
		const int operator[] (size_t i) const{
			return _cols[i];
		}
		int& operator[] (size_t i){
	    	return *(_cols+i);
       	}		
	private:
		int *_cols;
};

vector<Data> table;
size_t col,row;

int main() { 
	string fname="",l;
	size_t i=0;
	char *tmp;
	cout << "Please enter the file name: ";
	cin >> fname;
	cout << "Please enter the number of rows and columns: ";
	cin >> row >> col;
	cout << "File “" << fname << "” was read in successfully.\n";
	ifstream fin(fname.c_str());
	while(fin >> l && i < row){
		cout << l << endl;
		tmp=strtok((char *)l.c_str(),",");
		Data d(col);
		size_t j=0;
		while(tmp!=NULL){
			cout<<atoi(tmp)<<' ';
		    d[j]=atoi(tmp);
			tmp=strtok(NULL,",");
			j++;
		}
		cout<<endl<<d[0]<<" "<<d[1];
		//table[i]=d;
		cout<<endl;
		i++;
	}
    while(true){
		string str;
		getline(cin,str);
		cout << str <<endl;
	}
	return 0; 
}

void print(){
	for(size_t i=0;i<col;i++){
		for(size_t j=0;j<row;j++){
			cout<<table[i][j]<<" ";
		}
		cout<<endl;
	}
}
