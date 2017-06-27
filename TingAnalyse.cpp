#include "TingAnalyse.h"

void Analyser::start(unsigned char userCard[],int n)
{
	_isHu = false;

	for(int i=0;i<n;++i)
	{
		if(userCard[i]>0)
			_userCard[ChangeToValue(i)] = userCard[i];
	}

	if(_magicCard != 0)
	{
		_magicCardSize = _userCard[_magicCard];
		_userCard[_magicCard] = 0;
	}

	// 先扣除将牌的分析
	for(auto& elem:_userCard)
	{
		++analyseCount;
		hashMap TempUserCard(_userCard);
		if(elem.second == 2)
		{
			TempUserCard[elem.first] -= 2;
			analyse(TempUserCard);
		}
	}

	// 标准分析：将所有吃碰的牌都过滤
	analyse(_userCard);
}

void Analyser::analyse(hashMap userCard)
{
	bool isEnd = true;
	for(auto &elem:userCard)
	{

		if(elem.second >= 3)
		{
			analyseCount++;
			isEnd = false;
			hashMap TempUserCard(userCard);
			TempUserCard[elem.first] -= 3;
			analyse(TempUserCard);
		}
	}

	for(auto &elem:userCard)
	{

		if ((getValue(elem.first) < 8) && (getColor(elem.first) <= 2) && ((elem.second > 0) && 
			(userCard[elem.first + 1] > 0) && (userCard[elem.first + 2] > 0)))
		{
			analyseCount++;
			isEnd = false;
			hashMap TempUserCard(userCard);
			--TempUserCard[elem.first];
			--TempUserCard[elem.first + 1];
			--TempUserCard[elem.first + 2];
			analyse(TempUserCard);
		}
	}

	//对手牌进行分析
	if(isEnd){
		hashMap lastCard;
		int count = 0;
		for(auto& elem:userCard)
		{
			count += elem.second;
			if(elem.second)
				lastCard[elem.first] = elem.second;
		}
		if(count - 2*_magicCardSize <= 5)
		{
			//printCard(lastCard);
			dealWithLastCard(lastCard,count);
		}

	}

}

void Analyser::analyseWithMagic(hashMap userCard,int magicNum)
{
	bool isEnd = true;
	for(auto &elem:userCard)
	{

		if(elem.second == 2 && magicNum>0)
		{
			analyseCount++;
			isEnd = false;
			hashMap TempUserCard(userCard);
			TempUserCard[elem.first] -= 2;

			analyseWithMagic(TempUserCard,magicNum-1);
		}
	}

	for(auto &elem:userCard)
	{

		int value = getValue(elem.first),color = getColor(elem.first);
		if ((value <= 8) && (color <= 2) && (elem.second > 0) && (magicNum>0))
		{
			if(userCard.count(elem.first+1) && userCard[elem.first+1]>0)
			{
				analyseCount++;
				isEnd = false;
				hashMap TempUserCard(userCard);
				--TempUserCard[elem.first];
				--TempUserCard[elem.first + 1];
				analyseWithMagic(TempUserCard,magicNum-1);
			}
			else if(userCard.count(elem.first+2) && userCard[elem.first+2]>0)
			{
				analyseCount++;
				isEnd = false;
				hashMap TempUserCard(userCard);
				--TempUserCard[elem.first];
				--TempUserCard[elem.first + 2];
				analyseWithMagic(TempUserCard,magicNum-1);
			}
		}
	}

	for(auto &elem:userCard)
	{
		if(elem.second == 1 && magicNum>1)
		{
			analyseCount++;
			isEnd = false;
			hashMap TempUserCard(userCard);
			TempUserCard[elem.first] -= 1;

			analyseWithMagic(TempUserCard,magicNum-2);
		}
	}

	//对手牌进行分析
	if(isEnd){
		hashMap lastCard;
		int count = 0;
		for(auto& elem:userCard)
		{
			if(elem.second)
			{
				count += elem.second;
				lastCard[elem.first] = elem.second;
			}
		}

		cout<<"  magic card num:"<<magicNum<<endl;
		printCard(lastCard);
		dealWithLastCard2(lastCard,count,magicNum);
	}
}

void Analyser::printCard(hashMap& userCard) const
{
	for(auto &elem:userCard)
		printf("Card data:%d  ---  Card number:%d\n\n",elem.first,elem.second);
	cout<<endl;
}

// 枚举所有可能
void Analyser::dealWithLastCard(hashMap& lastCard,int count)
{
	if(_magicCardSize>0)
	{
		analyseWithMagic(lastCard,_magicCardSize);
		return;
	}

	int kindNum = lastCard.size();
	if(count == 0)
	{
		_isHu = true;
		for (auto &elem : _userCard)
		{
			_tingCard.insert(elem.first);//获取所听的牌
		}
		return;
	}

	else if(count == 2)
		Solution2(kindNum,lastCard);
	else if(count == 3)
		Solution3(kindNum,lastCard);
	else if(count == 5)
		Solution5(kindNum,lastCard);

	return;
}

void Analyser::dealWithLastCard2(hashMap& lastCard,int count,int magicNum)
{
 	if(magicNum == 0)
	{
		int kindNum = lastCard.size();
		if(count == 0)
		{
			_isHu = true;
			for (auto &elem : _userCard)
				_tingCard.insert(elem.first);//获取所听的牌
			
			return;
		}

		else if(count == 2)
			Solution2(kindNum,lastCard);
		else if(count == 3)
			Solution3(kindNum,lastCard);
		else if(count == 5)
			Solution5(kindNum,lastCard);

		return;
	}
	else
	{
		if(count == 0)
		{
			for(auto &elem:_userCard)
				_tingCard.insert(elem.first);
			_tingCard.insert(_magicCard);
			_isHu = true;
			return;
		}

		// 因为只要有两张以上万能牌且count不为0，就会以2张万能牌凑吃碰组合，所以此时magicNum必为1
		if(count == 1)
		{
			_tingCard.insert(lastCard.begin()->first);
			_tingCard.insert(_magicCard);
		}
		else if(count == 2)
		{
			for(auto &elem:lastCard)
				_tingCard.insert(elem.first);
		}
	}
}

void Analyser::getSequencialCombo(set<int>& CardValue,hashMap& lastCard)
{
	for(auto& elem:CardValue)
	{
		int color = getColor(elem),value = getValue(elem);
		if(color > 2 || value > 7) continue;
		int n1 = lastCard.count(elem+1),n2 = lastCard.count(elem+2);
		hashMap TemplastCard(lastCard);
		if(n1)
		{
			if(lastCard[elem+1] == 0) continue;
			--TemplastCard[elem],--TemplastCard[elem+1];
			//printCard(lastCard);
			auto itr = find_if(TemplastCard.begin(),TemplastCard.end(),[](hashMap::value_type itr){ return itr.second == 1; });
			if(value == 1 || value == 7)
			{
				_tingCard.insert(itr->first);
				_huCard.insert(elem+2);
			}
			else
			{
				_tingCard.insert(itr->first);
				_huCard.insert(elem-1),_huCard.insert(elem+2);
			}
		}
		else if(n2)
		{
			if(lastCard[elem+2] == 0) continue;
			--TemplastCard[elem],--TemplastCard[elem+2];
			//printCard(lastCard);
			auto itr = find_if(TemplastCard.begin(),TemplastCard.end(),[](hashMap::value_type itr){ return itr.second == 1; });
			_tingCard.insert(itr->first);
			_huCard.insert(elem+1);
		}
	}
}


void Analyser::Solution2(int kindNum,hashMap& lastCard)
{
	switch (kindNum)
	{
	case 1:
		{
			_isHu = true;
			for (auto &elem : lastCard)
			_tingCard.insert(elem.first);//获取所听的牌
			break;
		}
	case 2:
		{
			hashSet temp;
			BYTE GangCard = 0;
			for(auto &elem:lastCard)
			{
				if(_userCard[elem.first] == 4) GangCard = elem.first;		// 防止在手里已经有四张牌的时候但是达成听这张牌的条件
				temp.insert(elem.first);
			}
			if(GangCard)
				_tingCard.insert(GangCard);
			else
				_tingCard.insert(temp.begin(),temp.end());
			break;
		}
	default:
		break;
	}
}
void Analyser::Solution3(int kindNum,hashMap& lastCard)
{
	switch (kindNum)
	{
	case 2:
		{
			for(auto& elem:lastCard)
			{
				if(elem.second == 1)
					_tingCard.insert(elem.first);
				else
					_huCard.insert(elem.first);
			}
			set<int> CardValue;
			for (auto& initElem : lastCard)
				CardValue.insert(initElem.first);

			getSequencialCombo(CardValue, lastCard);
			break;
		}
	case 3:
		{
			set<int> CardValue;
			for(auto& initElem:lastCard) 
				CardValue.insert(initElem.first);

			getSequencialCombo(CardValue,lastCard);
		}
	default:
		break;
	}
}
void Analyser::Solution5(int kindNum,hashMap& lastCard)
{
	switch (kindNum)
	{
	case 2:
		{
			_isHu = true;
			for (auto &elem : lastCard)
			{
				_tingCard.insert(elem.first);
				_huCard.insert(elem.first);
			}
			break;
		}
	case 3:
		{
		    for (auto& elem : lastCard)
		    {
			    if (elem.second == 1)
				_tingCard.insert(elem.first);
			    else
				_huCard.insert(elem.first);
		    }
			break;
		}

	case 4:
		{
			set<int> CardValue;
			for(auto& initElem:lastCard) 
			{
				if(initElem.second == 2)
				{
					lastCard[initElem.first] = 0;
					//lastCard.erase(initElem.first);
					continue;
				}
				CardValue.insert(initElem.first);
			}

			getSequencialCombo(CardValue,lastCard);
			break;
		}
	default:
		break;
	}
}

void Analyser::resetState()
{
	_magicCard = 0;
	analyseCount = 0;
	// _magicCard = 0;
	_magicCardSize = 0;

	_userCard.clear();
	_tingCard.clear();
	_huCard.clear();
	_isHu = false;
}


set<unsigned char> Analyser::IsTingShisanyao(unsigned char userCard[], int n)
{
	map<unsigned char, int> cardMap;
	vector<unsigned char> coll;
	set<unsigned char> ret;
	unsigned char HuaPaiCard = 0;
	WORD btHuaPaiCount = 0;
	WORD  btHuaPaiValue = 0;
	for (int i = 0; i < n; ++i)
	{
		if (userCard[i] > 0)
			_userCard[ChangeToValue(i)] = userCard[i];
	}
	if (_userCard.size() < 13) return ret;

	for (int i = 27; i <= 33; ++i)
	{
		if (userCard[i] > 2)
			return  ret;
		if (userCard[i] != 1)
		{
			btHuaPaiValue = userCard[i];
			HuaPaiCard = ChangeToValue(i);
		}//剩下花牌的数量

	}

	for (int i = 27; i <=33; ++i)//减掉所有花牌的数量
	{
		if (userCard[i] > 0)
		_userCard[ChangeToValue(i)] = 0;
	}

	cardMap.insert(_userCard.begin(), _userCard.end());

	int numCount = 0;

	int continualCount = 0;

	for (auto &elem : cardMap)
	{
		auto pos = cardMap.upper_bound(elem.first);
		if (pos != cardMap.end()
			&& getValue(elem.first) != 9 
			&& pos->first - elem.first < 3 
			&& elem.second>0
			&& pos->second>0)
		{
			if (pos->first - elem.first == 2)		// 上边界是离它2的一张牌
			{
				++continualCount;	//不连在一起的
				coll.push_back(elem.first), coll.push_back(pos->first);
			}
			else if (pos->first - elem.first == 1)				// 上边界是紧挨着的一张牌
			{
				++continualCount;	//不连在一起的
				coll.push_back(elem.first), coll.push_back(pos->first);
				if (cardMap.count(elem.first + 2))					// 再继续查找相隔2的牌是否存在
				{
					++continualCount;	//不连在一起的
					coll.push_back(elem.first), coll.push_back(elem.first + 2);
				}
			}
		}
	}

	switch (continualCount)
	{
	case 0:
		if (cardMap.size() == 13)
		{
			for (auto &elem : _userCard)
				if (elem.second == 2)
					ret.insert(elem.first);
		}
		else
		{ 
			for (auto &elem : _userCard)
				ret.insert(elem.first);
		}
		break;
	case 1:
		ret.insert(coll.begin(), coll.end());
		break;
	case 2:
	{
		int color = getColor(*coll.begin());
		bool flag = true;
		if (coll[1] != coll[2]) return ret;

		ret.insert(coll[1]);
		break;
	}
	default:
		break;
	}
	if (ret.size() >= 6 )
	{
		if (btHuaPaiValue == 2)
		{
			ret.clear();
			ret.insert(HuaPaiCard);
		}
	}
	if(!ret.empty())
	{
		for(auto &elem:ret)
			_tingCard.insert(elem);//把听的牌发送给客户
	}
	return ret;
}

set<unsigned char> Analyser::IsTingWanNengstart(hashMap userCard, int n, int magicNum, char magicCard)
{
	set<unsigned char> ret;
	_userCard = userCard;
 	ret=IsTingWanNeng(userCard, n, magicNum, magicCard);
	if(!ret.empty())
	{
		for(auto &elem:ret)
        _tingCard.insert(elem);//把听的牌发送给客户
	}

	return  ret;
}

set<unsigned char> Analyser::IsTingWanNeng(hashMap userCard, int n, int magicNum, char magicCard)
{
	bool isEnd = true;
	 char magicCount = magicNum;
	 char clearCount = 0;
	set<unsigned char> ret;
	hashMap TempUserCard(userCard);
	for (auto &elem : userCard)//减掉拱子
	{
	    if ((elem.first == magicCard)  && (elem.second > 0))
		{
			TempUserCard[elem.first] = 0;;
		}	
	}
    	clearCount = 0;//这种牌的判断结束，所以要清0
	map<unsigned char, int> cardMap;
	vector<unsigned char> coll;
	unsigned char HuaPaiCard = 0;
	WORD btHuaPaiCount = 0;
	WORD  btHuaPaiValue = 0;

	WORD countTemp1 = _userCard.size() + magicCount - 1;

	if (countTemp1 < 13)
		return ret;
    for (auto &elem : TempUserCard)//减掉花牌
	{
		if ((elem.first & 0xf0) == 0x30)//表示花牌
		{
			if (elem.second >0)
			{
				TempUserCard[elem.first] = 0;
			}
		}
	}

	cardMap.insert(TempUserCard.begin(), TempUserCard.end());

	int numCount = 0;


	int continualCount = 0;
	for (auto &elem : cardMap)
	{
		auto pos = cardMap.upper_bound(elem.first);
		if (pos != cardMap.end() && getValue(elem.first) != 9 
		&& pos->first - elem.first < 3 
		&& elem.second>0 && pos->second>0)
		{
			if (pos->first - elem.first == 2)		// 上边界是离它2的一张牌
			{
				++continualCount;	//不连在一起的
				coll.push_back(elem.first), coll.push_back(pos->first);
			}
			else if (pos->first - elem.first == 1)				// 上边界是紧挨着的一张牌
			{
				++continualCount;	//不连在一起的
				coll.push_back(elem.first), coll.push_back(pos->first);
				if (cardMap.count(elem.first + 2))					// 再继续查找相隔2的牌是否存在
				{
					++continualCount;	//不连在一起的
					coll.push_back(elem.first), coll.push_back(elem.first + 2);
				}
			}
		}
	}
	if(countTemp1 == 13)
	{
		if (continualCount == 0)
		{
			for (auto &elem : _userCard)
			{
				if (elem.second == 2 && elem.first != magicCard)
					ret.insert(elem.first);
			}
		}
	}
	else
	{
		switch (continualCount)
		{
		case 0:
			for (auto &elem : _userCard)
				ret.insert(elem.first);
			
			break;
		case 1:
			ret.insert(coll.begin(), coll.end());
			break;
		case 2:
		{
			int color = getColor(*coll.begin());
			bool flag = true;
			if (coll[1] != coll[2]) return ret;

			ret.insert(coll[1]);
			break;
		}
		default:
			break;
		}
	}

		return ret;

}




//七对牌
bool Analyser::IsQiXiaoDuiTing(const BYTE cbCardIndex[MAX_INDEX], const BYTE cbWeaveCount)
{
	//组合判断
	if (cbWeaveCount > 0)
		return 0;

	DWORD dwResult =0;

	BYTE btDui = 0; //多少对
					//	BYTE btGang = 0; //多少杠
					//构造扑克
//	unsigned char btCardIndex = SwitchToCardIndex(btHuCard);
	BYTE cbCardIndexTemp[MAX_INDEX] = { 0 };
	CopyMemory(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	//   ++cbCardIndexTemp[btCardIndex];
	for (BYTE i = 0; i<MAX_VALID_INDEX; i++)
	{
		if (2 == cbCardIndexTemp[i])
		{
			++btDui;
		}
	}
	if (6 == btDui)
	{
		for (BYTE i = 0; i < MAX_VALID_INDEX; i++)
		{
			if (1 == cbCardIndexTemp[i])
			{
				_tingCard.insert(ChangeToValue(i));

			}
		}
		return true;
	}

return false;
}



void Analyser::analyseQiMagic(hashMap userCard, int magicNum)//去掉万能牌 七对分析
{
	bool isEnd = true;
	for (auto &elem : userCard)
	{
		if ((elem.second == 2)|| (elem.second == 4))
		{
			isEnd = false;
			hashMap TempUserCard(userCard);
			if (elem.second == 2)
				TempUserCard[elem.first] -= 2;
			else if (elem.second == 4)
				TempUserCard[elem.first] -= 4;
			analyseQiMagic(TempUserCard, magicNum);
		}
	}
	for (auto &elem : userCard)
	{
		if ((elem.second == 1) && (magicNum > 0))
		{
			isEnd = false;
			hashMap TempUserCard(userCard);
			TempUserCard[elem.first] -= 1;
			analyseQiMagic(TempUserCard, magicNum - 1);
		}
	}
	if (isEnd)
	{
		hashMap lastCard;
		int count = 0;
		for (auto &elem : userCard)
		{
			count += elem.second;
			if (elem.second)
				lastCard[elem.first] = elem.second;
		}
		switch (count)
		{
		case 0:
			if ((magicNum == 0) || (magicNum == 1) || (magicNum == 2) || (magicNum == 3))
			{
				for (auto &elem : _userCard)
					_tingCard.insert(elem.first);
			}
			break;
		case 1:
			for (auto &elem : lastCard)
				_tingCard.insert(elem.first);
			break;
		case 2:
			if (magicNum == 0)
			{
				for (auto &elem : lastCard)
					_tingCard.insert(elem.first);
				break;
			}
		default:
			break;
		}
	}
}


void Analyser::AnalyserStart(unsigned char userCard[], int n, int magicCard, int magicNum)//去掉万能牌 七对分析
{
	for (int i = 0; i < n; ++i)
		_userCard[userCard[i]];
	hashMap TempUserCard(_userCard);
	if (magicCard != 0)
	{
		_magicCard = magicCard;

		for (auto &elem : TempUserCard)
		{
			if (elem.first == _magicCard)//减掉魔法牌
				TempUserCard[elem.first] = 0;
		}
	}
	analyseQiMagic(TempUserCard, magicNum);
}
