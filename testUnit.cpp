#include "standard.h"

#define TEST	arr5

int main()
{
	int arr[] = {1,1,2,2,3,4,7,8};
	int arr1[] = {1,1,2,2,3,4,5,7,9,9,9};
	int arr2[] = {1,2,4,4,5,6,9,9};
	int arr3[] = {1,2,3,3,4,5,24,24,34,34,31};
	int arr4[] = {1,1,1,2,2,2,3,3,3,5,7,8,9,9};
	int arr5[] = { 0x7,0x8,0x9,0x11,0x12,0x13,0x21,0x22,0x23,0x27,0x29,0x29,0x31,0x31 };

 	int s = distance(begin(TEST),end(TEST));
	auto a = Analyser(TEST,s,0);
	auto ting = a.getTingCard();
	auto hu = a.getHuCard();

	cout<<endl;
	cout<<"analyse count:"<<a.analyseCount<<endl;
}
