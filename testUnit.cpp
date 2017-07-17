#include "TingAnalyse.h"

int main()
{
	BYTE arr[] = {1,1,2,2,3,4,7,8};
	BYTE arr1[] = {1,1,2,2,3,4,5,7,9,9,9};
	BYTE arr2[] = {1,2,4,4,5,6,9,9};
	BYTE arr3[] = {1,2,3,3,4,5,24,24,34,34,31};
	BYTE arr4[] = {1,1,1,2,2,2,3,3,3,5,7,8,9,9};

	Analyser a = Analyser();
	a.start(arr1, 11);
	a.start(arr1, 11,4);
	auto ting = a.getTingCard();
	auto hu = a.getHuCard();
}
