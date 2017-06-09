// io
#include <iostream>

// container
#include <vector>
#include <array>
#include <set>
#include <unordered_set>
#include <unordered_map>

// algorithm
#include <algorithm>

// utility
#include <utility>

using namespace std;

typedef unordered_map<int,short> hashMap;
typedef unordered_set<int>	hashSet;



class Analyser
{
public:
	Analyser(int userCard[],int n,int magicCard);
	void printCard(hashMap& userCard) const;
	inline int getValue(int card) { return card & 0x0f; }
	inline int getColor(int card){ return (card & 0xf0)>>4; }
	hashSet getTingCard() const { return _tingCard; };
	hashSet getHuCard() const { return _huCard; }
	bool IsHu() const { return _isHu; }

	int analyseCount;

private:
	void analyse(hashMap userCard);
	void dealWithLastCard(hashMap& lastCard,int count);
	void getSequencialCombo(set<int>& CardValue,hashMap& lastCard);
	void Solution2(int kindNum,hashMap& lastCard);
	void Solution3(int kindNum,hashMap& lastCard);
	void Solution5(int kindNum,hashMap& lastCard);

	hashMap _userCard;
	hashSet	_tingCard;
	hashSet	_huCard;
	bool _isHu;
	int _magicCard;
	int _magicCardSize;

};