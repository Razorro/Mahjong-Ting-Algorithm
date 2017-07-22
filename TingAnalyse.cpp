#include "TingAnalyse.h"

void Analyser::start(BYTE userCard[],int n,int magicCard)
{
	resetState();

	if (magicCard != 0)
		_magicCard = magicCard;

	for(int i=0;i<n;++i)
		++_userCard[userCard[i]];
	
	if(_magicCard != 0)
	{
		_magicCardSize = _userCard[_magicCard];
		_userCard[_magicCard] = 0;

		if(_magicCardSize)
		{
			for (auto& elem : _userCard)
			{
				++analyseCount;
				hashMap TempUserCard(_userCard);
				if (elem.second == 2)
				{
					TempUserCard[elem.first] -= 2;
					analyse(TempUserCard);
				}

				if (elem.second == 1)
				{
					TempUserCard[elem.first] -= 1;
					_magicCardSize -= 1;
					analyse(TempUserCard);
					_magicCardSize += 1;
				}
			}
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
			++analyseCount;
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
			++analyseCount;
			isEnd = false;
			hashMap TempUserCard(userCard);
			--TempUserCard[elem.first];
			--TempUserCard[elem.first + 1];
			--TempUserCard[elem.first + 2];
			analyse(TempUserCard);
		}
	}

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
			dealWithLastCard(lastCard,count);
	}

}

void Analyser::analyseWithMagic(hashMap userCard,int magicNum)
{
	bool isEnd = true;
	for(auto &elem:userCard)
	{
		if(elem.second == 2 && magicNum>0)
		{
			++analyseCount;
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
				++analyseCount;
				isEnd = false;
				hashMap TempUserCard(userCard);
				--TempUserCard[elem.first];
				--TempUserCard[elem.first + 1];
				analyseWithMagic(TempUserCard,magicNum-1);
			}
			else if(userCard.count(elem.first+2) && userCard[elem.first+2]>0)
			{
				++analyseCount;
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
			++analyseCount;
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
	if (count == 0)
		Solution0(lastCard);
	else if(count == 2)
		Solution2(lastCard);
	else if(count == 3)
		Solution3(lastCard);
	else if(count == 5)
		Solution5(lastCard);

	return;
}

void Analyser::dealWithLastCard2(hashMap& lastCard,int count,int magicNum)
{
 	if(magicNum == 0)
	{
		int kindNum = lastCard.size();
		if (count == 0)
			Solution0(lastCard);
		else if(count == 2)
			Solution2(lastCard);
		else if(count == 3)
			Solution3(lastCard);
		else if(count == 5)
			Solution5(lastCard);

		return;
	}
	else if(count < 5)
	{
		if(count == 0)
		{
			_huFlag = true;
			return;
		}

		if(count == 1)
		{
			int existCard = lastCard.begin()->first;
			
			_tingHuCard[existCard].insert(all_in);
			_tingHuCard[_magicCard].insert(existCard);
		}
		else if(count == 2)
		{
			vector<int> key;
			for (auto &elem : lastCard)
				key.push_back(elem.first);

			int card1 = key[0], card2 = key[1];
			// 刻子听胡牌
			_tingHuCard[card1].insert(card2);
			_tingHuCard[card2].insert(card1);

			// 顺子听胡牌
			getSequenceWithMagic(card1, card2);
			getSequenceWithMagic(card2, card1);
		}
	}
}

void Analyser::getSequenceWithMagic(int card1, int card2)
{
	int v1 = getValue(card1), c1 = getColor(card1);
	int v2 = getValue(card2), c2 = getColor(card2);
	if (c1 < 2)
	{
		if (v1 == 1)
			_tingHuCard[card2].insert({ card1 + 1,card1 + 2 });
		else if (v1 == 2)
			_tingHuCard[card2].insert({ card1 - 1,card1 + 1,card1 + 2 });
		else if (v1 == 9)
			_tingHuCard[card2].insert({ card1 - 1,card1 - 2 });
		else if (v1 == 8)
			_tingHuCard[card2].insert({ card1 + 1,card1 - 1,card1 - 2 });
		else
			_tingHuCard[card2].insert({ card1 + 1,card1 + 2,card1 - 1,card1 - 2 });
	}
}

void Analyser::getSequencialCombo(hashSet& CardValue,hashMap& lastCard)
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
				_tingHuCard[itr->first].insert(elem + 2);
			
			else if (value == 8)
				_tingHuCard[itr->first].insert(elem - 1);
			
			else
				_tingHuCard[itr->first].insert({elem - 1, elem + 2});
			
		}
		else if(n2)
		{
			if(lastCard[elem+2] == 0) continue;
			--TemplastCard[elem],--TemplastCard[elem+2];
			auto itr = find_if(TemplastCard.begin(),TemplastCard.end(),[](hashMap::value_type itr){ return itr.second == 1; });
			_tingHuCard[itr->first].insert(elem + 1);
		}
	}
}

void Analyser::Solution0(hashMap& lastCard)
{
	_huFlag = true;
	return;
}

void Analyser::Solution2(hashMap& lastCard)
{
	int kindNum = lastCard.size();
	switch (kindNum)
	{
	case 1:
	{
		_huFlag = true;
		return;
	}

	case 2:
	{
		vector<int> key;
		for (auto &elem : lastCard)
			key.push_back(elem.first);

		_tingHuCard[key[0]].insert(key[1]);
		_tingHuCard[key[1]].insert(key[0]);

		break;
	}

	default:
		cerr<<"Solution2 failed"<<endl;
	}

	return;
}

void Analyser::Solution3(hashMap& lastCard)
{
	int kindNum = lastCard.size();
	switch (kindNum)
	{
	case 2:
		{
			int single, pair;
			for(auto& elem:lastCard)
			{
				if (elem.second == 1)
					single = elem.first;
				else
					pair = elem.first;
			}
			_tingHuCard[single].insert(pair);
			
			hashSet CardValue;
			for (auto& initElem : lastCard)
				CardValue.insert(initElem.first);

			getSequencialCombo(CardValue, lastCard);
			break;
		}
	case 3:
		{
			hashSet CardValue;
			for(auto& initElem:lastCard) 
				CardValue.insert(initElem.first);

			getSequencialCombo(CardValue,lastCard);
		}
	default:
		break;
	}
}
void Analyser::Solution5(hashMap& lastCard)
{
	int kindNum = lastCard.size();
	switch (kindNum)
	{
	case 2:
		{
			_huFlag = true;
			return;
		}
	case 3:
		{
			int single;
			vector<int> pair;
		    for (auto& elem : lastCard)
		    {
				if (elem.second == 1)
					single = elem.first;
				else
					pair.push_back(elem.first);
		    }
			_tingHuCard[single].insert({ pair[0],pair[1] });
			break;
		}

	case 4:
		{
			hashSet CardValue;
			for(auto& initElem:lastCard) 
			{
				if(initElem.second == 2)
				{
					lastCard[initElem.first] = 0;
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
	_magicCardSize = 0;
	_huFlag = false;

	_userCard.clear();
	_tingHuCard.clear();
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
