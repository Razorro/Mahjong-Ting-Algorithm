# Mahjong-Ting-AnalyseAlgorithm
分析一组牌可打出哪些牌来进入听牌状态，以及进入听牌状态时可胡的牌

***
## 思路
通过深度优先遍历，将可组成吃碰的牌先进行剔除，然后在简单情况下的进行枚举分析  
常规胡牌时，牌的数量必定满足3N+2， N ∈ {0,1,2,3,4}  
在有万能牌的情况下，需要考虑一种情况就是：先把对牌挑出，然后进行分析


## 数据成员说明
hashMap   _userCard: 用户的手牌，用haspMap表示，key表示牌值，value表示数量    
hashMap   _tingHuCard: 用户打出哪些牌，可胡哪些牌,key表示打出后进入听状态，value为可胡的牌

  
int analyseCount：统计的进行递归的次数，用作性能分析  
boo _huFlag: 胡牌标志
int _magicCard：万能牌  
int _magicCardSize：万能牌的数量
