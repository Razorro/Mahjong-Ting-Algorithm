#include "TingAnalyse.h"

int main()
{
	BYTE arr[] = {1,1,2,2,3,4,7,8};
	BYTE arr1[] = {1,1,1,2,2,2,5,5,4,9,15};
	BYTE arr2[] = {1,2,4,4,5,6,9,9};
	BYTE arr3[] = {1,2,3,3,4,5,24,24,34,34,31};
	BYTE arr4[] = {1,1,1,2,2,2,3,3,3,5,7,8,9,9};
	BYTE arr5[] = { 1,2,3,6,7,8,11,12,16,18,19 };
	BYTE arr6[] = { 1,3,7,7,13,15,15,16,19,19,22,23,24,28 };

	Analyser a = Analyser();
	a.start(arr6, 14,19);
	auto ret = a.getTingHuCard();
}
