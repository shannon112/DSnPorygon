#include <iostream>
#include "rnGen.h"
using namespace std;
int main(){
	RandomNumGen rnGen(0);
	while(1){	
		cout<<rnGen(3)<<endl;
	}
}

