// looper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int sub1(int a){
	int i=0;
	return a*1;
}

int sub2(int a){
	int i=0;
	return a*2;
}

int main(int argc, char* argv[])
{
	for(int i=0;i<10;i++){
		if(i%2==0){
			printf("sub1(%d)=%d\n", i, sub1(i));
		}
		else{
			printf("sub2(%d)=%d\n", i, sub2(i));
		}
	}

	return 0;
}



