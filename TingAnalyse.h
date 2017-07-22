// io
#include <iostream>

// container
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>

// algorithm
#include <algorithm>

// utility
#include <utility>

using namespace std;

typedef unsigned char BYTE;
typedef unordered_map<int,short> hashMap;
typedef unordered_set<int>	hashSet;
typedef unordered_map<BYTE, hashSet> hashByteMap;




class Analyser
{
public:
	enum { all_in = -1 };

	Analyser(int magicCard):analyseCount(0),_magicCardSize(0),_magicCard(magicCard) {}
	Analyser():analyseCount(0),_magicCardSize(0),_magicCard(0) {}

	// extend: 十三幺及七对牌型
	void AnalyserStart(BYTE userCard[], int n, int magicCard, int magicNum);
	void analyseQiMagic(hashMap userCard, int magicNum);	
	void analyseHuMagic(hashMap userCard, int magicNum);	
	void AnalyserHuStart(BYTE userCard[], int n, int magicCard, int magicNum,BYTE SendCardData);
	set<BYTE> IsTingWanNeng(hashMap userCard, int n, int magicNum, char magicCard);
	set<BYTE> IsTingWanNengstart(hashMap userCard, int n, int magicNum, char magicCard);

	// origin: 3N+2牌型
	void start(BYTE userCard[],int n,int magicCard = 0);
	void setMagicCard(BYTE magicCard) { _magicCard = magicCard; }
	int getMagicCard() const { return _magicCard; }
	void printCard(hashMap& userCard) const;
	
	
	hashSet getTingCard() const { return _tingCard; };
	hashSet getHuCard() const { return _huCard; }
	hashByteMap getTingHuCard() const { return _tingHuCard; }
	int getRecursiveTime() const { return analyseCount; }
	bool isHu() const { return _huFlag; }
	bool isTing() const { return !_tingCard.empty(); }
	void resetState();


private:
	void analyse(hashMap userCard);
	void analyseWithMagic(hashMap userCard,int magicNum);
	void dealWithLastCard(hashMap& lastCard,int count);
	void dealWithLastCard2(hashMap& lastCard,int count,int magicNum);
	void getSequencialCombo(hashSet& CardValue,hashMap& lastCard);
	void getSequenceWithMagic(int card1,int card2);
	

	inline int getValue(int card) { return card % 10; }
	inline int getColor(int card) { return card / 10; }

	void Solution0(hashMap& lastCard);
	void Solution2(hashMap& lastCard);
	void Solution3(hashMap& lastCard);
	void Solution5(hashMap& lastCard);

	hashMap _userCard;
	hashSet	_tingCard;
	hashSet	_huCard;
	hashByteMap _tingHuCard;

	int analyseCount;
	int _magicCard;
	int _magicCardSize;
	bool _huFlag;
};
