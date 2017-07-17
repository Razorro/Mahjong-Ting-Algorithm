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
typedef unordered_map<BYTE, hashSet> B_hashSet;




class Analyser
{
public:
	Analyser(int magicCard):analyseCount(0),_magicCardSize(0),_magicCard(magicCard) {}
	Analyser():analyseCount(0),_magicCardSize(0),_magicCard(0) {}

	// extend: 十三幺及七对牌型
	void AnalyserStart(BYTE userCard[], int n, int magicCard, int magicNum);//去掉万能牌 七对分析
	void analyseQiMagic(hashMap userCard, int magicNum);	//去掉万能牌 七对分析
	void analyseHuMagic(hashMap userCard, int magicNum);	//去掉万能牌 七对胡牌
	void AnalyserHuStart(BYTE userCard[], int n, int magicCard, int magicNum,BYTE SendCardData);//去掉万能牌 七对分析
	set<BYTE> IsTingWanNeng(hashMap userCard, int n, int magicNum, char magicCard);
	set<BYTE> IsTingWanNengstart(hashMap userCard, int n, int magicNum, char magicCard);

	// origin: 3N+2 module
	void start(BYTE userCard[],int n,int magicCard = 0);
	void setMagicCard(BYTE magicCard) { _magicCard = magicCard; }
	void printCard(hashMap& userCard) const;
	
	
	hashSet getTingCard() const { return _tingCard; };
	hashSet getHuCard() const { return _huCard; }
	int getRecursiveTime() const { return analyseCount; }
	bool isHu() const { return !_huCard.empty(); }
	bool isTing() const { return !_tingCard.empty(); }
	void resetState();


private:
	void analyse(hashMap userCard);
	void analyseWithMagic(hashMap userCard,int magicNum);
	void dealWithLastCard(hashMap& lastCard,int count);
	void dealWithLastCard2(hashMap& lastCard,int count,int magicNum);
	void getSequencialCombo(set<int>& CardValue,hashMap& lastCard);

	inline int getValue(int card) { return card & 0x0f; }
	inline int getColor(int card) { return (card & 0xf0) >> 4; }

	void Solution0(hashMap& lastCard);
	void Solution2(hashMap& lastCard);
	void Solution3(hashMap& lastCard);
	void Solution5(hashMap& lastCard);

	hashMap _userCard;
	hashSet	_tingCard;
	hashSet	_huCard;
	B_hashSet _tingHuCard;

	int analyseCount;
	int _magicCard;
	int _magicCardSize;
};
