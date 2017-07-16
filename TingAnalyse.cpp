#include "TingAnalyse.h"

void Analyser::start(BYTE userCard[],int n)
{
	for(int i=0;i<n;++i)
		++_userCard[userCard[i]];
	
	if(_magicCard != 0)
	{
		_magicCardSize = _userCard[_magicCard];
		_userCard[_magicCard] = 0;
	}

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
		for (auto &elem : _userCard)
			_tingCard.insert(elem.first);
		
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
		if(color > 2 || value > 8) continue;
		int n1 = lastCard.count(elem+1),n2 = lastCard.count(elem+2);
		hashMap TemplastCard(lastCard);
		if(n1)
		{
			if(lastCard[elem+1] == 0) continue;
			--TemplastCard[elem],--TemplastCard[elem+1];
			auto itr = find_if(TemplastCard.begin(),TemplastCard.end(),[](hashMap::value_type itr){ return itr.second == 1; });
			if(value == 1)
			{
				_tingCard.insert(itr->first);
				_huCard.insert(elem+2);
			}
			else if (value == 8)
			{
				_tingCard.insert(itr->first);
				_huCard.insert(elem - 1);
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
			for (auto &elem : lastCard)
			_tingCard.insert(elem.first);	//获取所听的牌
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
	_magicCardSize = 0;

	_userCard.clear();
	_tingCard.clear();
	_huCard.clear();
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


void Analyser::AnalyserStart(BYTE userCard[], int n, int magicCard, int magicNum)//去掉万能牌 七对分析
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
