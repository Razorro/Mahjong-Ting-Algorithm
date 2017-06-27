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

typedef unordered_map<int,short> hashMap;
typedef unordered_set<int>	hashSet;




class Analyser
{
public:
	enum{ max_value = 0xffff };
	set<unsigned char> IsTingShisanyao(unsigned char userCard[], int n);
	Analyser(int magicCard):analyseCount(0),_magicCardSize(0),_magicCard(magicCard) {}
	Analyser():analyseCount(0),_magicCardSize(0),_magicCard(0) {}
	void AnalyserStart(unsigned char userCard[], int n, int magicCard, int magicNum);//去掉万能牌 七对分析
	void analyseQiMagic(hashMap userCard, int magicNum);//去掉万能牌 七对分析
	void analyseHuMagic(hashMap userCard, int magicNum);//去掉万能牌 七对胡牌
	void AnalyserHuStart(unsigned char userCard[], int n, int magicCard, int magicNum,WORD m_cbSendCardData);//去掉万能牌 七对分析
	void start(unsigned char userCard[],int n);
	void printCard(hashMap& userCard) const;
	inline int getValue(int card) { return card & 0x0f; }
	inline int getColor(int card){ return (card & 0xf0)>>4; }
	inline int ChangeToValue(int cbCardIndex) {
		if (cbCardIndex < 27)
			return ((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1);
		else return (0x30 | (cbCardIndex - 27 + 1));
	}
	bool TingPaiState() { return _tingCard.empty() ? false : true; }
	bool HuPaiState() { return _huCard.empty() ? false : true; }
	hashSet getTingCard() const { return _tingCard; };
	hashSet getHuCard() const { return _huCard; }
	bool IsHu() const { return _isHu; }
	void resetState();

	set<unsigned char> IsTingWanNeng(hashMap userCard, int n, int magicNum, char magicCard);
	set<unsigned char> IsTingWanNengstart(hashMap userCard, int n, int magicNum, char magicCard);

	bool IsQiXiaoDuiTing(const BYTE cbCardIndex[MAX_INDEX], const BYTE cbWeaveCount);
	

	int analyseCount;
	int _magicCard;
	int _magicCardSize;

private:
	void analyse(hashMap userCard);
	void analyseWithMagic(hashMap userCard,int magicNum);
	void dealWithLastCard(hashMap& lastCard,int count);
	void dealWithLastCard2(hashMap& lastCard,int count,int magicNum);
	void getSequencialCombo(set<int>& CardValue,hashMap& lastCard);
	void Solution2(int kindNum,hashMap& lastCard);
	void Solution3(int kindNum,hashMap& lastCard);
	void Solution5(int kindNum,hashMap& lastCard);


	hashMap _userCard;
	hashSet	_tingCard;
	hashSet	_huCard;
	bool _isHu;
};
