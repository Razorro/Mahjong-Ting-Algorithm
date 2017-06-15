#include "TingAnalyse.h"

void Analyser::start(unsigned char userCard[],int n)
{
	_isHu = false;

	for(int i=0;i<n;++i)
		_userCard[userCard[i]];

	if(_magicCard != 0)
	{
		_magicCardSize = _userCard[_magicCard];
		_userCard[_magicCard] = 0;
	}

	// 分析一：先扣除将牌，再过滤所有吃碰下的情况
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

	// 分析二：枚举所有在过滤完吃碰下的情况
	analyse(_userCard);
}

void Analyser::analyse(hashMap userCard)
{
	bool isEnd = true;
	for(auto &elem:userCard)
	{
		if(elem.second == 3)
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
		if ((value < 8) && (color <= 2) && (elem.second > 0) && (magicNum>0))
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
			count += elem.second;
			if(elem.second)
				lastCard[elem.first] = elem.second;
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

// 到底之后枚举所有可能
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

// 在处理完万能牌之后的情景枚举
void Analyser::dealWithLastCard2(hashMap& lastCard,int count,int magicNum)
{
	if(magicNum == 0)
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

		return;
	}
	else
		_isHu = true;
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
				if (elem.second == 1)
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

void Analyser::resetState()
{
	analyseCount = 0;
	// _magicCard = 0;
	_magicCardSize = 0;

	_userCard.clear();
	_tingCard.clear();
	_huCard.clear();
	_isHu = false;
}

