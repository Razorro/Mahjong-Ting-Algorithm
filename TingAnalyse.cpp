#include "standard.h"

Analyser::Analyser(int userCard[],int n,int magicCard):analyseCount(0)
{
	for(int i=0;i<n;++i)
		++_userCard[userCard[i]];

	if(magicCard != 0) 
		_magicCard = magicCard;

	_isHu = false;

	if(_userCard.find(magicCard) != _userCard.end())
		_magicCardSize = _userCard[magicCard];

	// 先扣除将牌的分析
	for(auto& elem:_userCard)
	{
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
		if(elem.second == 3)
		{
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
		if(count>5) return;
		printCard(lastCard);
		analyseCount++;
		dealWithLastCard(lastCard,count);
	}
	
}

void Analyser::printCard(hashMap& userCard) const
{
	for(auto &elem:userCard)
		printf("Card data:%x  ---  Card number:%d\n",elem.first,elem.second);
	cout<<endl;
}

// 枚举所有可能
void Analyser::dealWithLastCard(hashMap& lastCard,int count)
{
	int kindNum = lastCard.size();
	if(count == 0)
	{
		_isHu = true;
		return;
	}

	else if(count == 2)
		Solution2(kindNum,lastCard);
	else if(count == 3)
		Solution3(kindNum,lastCard);
	else if(count == 5)
		Solution5(kindNum,lastCard);
	
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
			break;
		}
	case 2:
		{
			for(auto& elem:lastCard)
			{
				_tingCard.insert(elem.first);
				_huCard.insert(elem.first);
			}
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
			break;
		}
	case 3:
		{
			for(auto& elem:lastCard)
				if(elem.second == 1)
					_tingCard.insert(elem.first);
				else
					_huCard.insert(elem.first);
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