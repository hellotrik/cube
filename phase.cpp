//============================================================================
// Name        : ji.cpp
// Author      : my
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "stax.h"
using namespace std;
vector<int> answer;
typedef vector<int> vi;
vi applyMove(int move,vi state);
int inverse(int move);
vi id(vi state);
//int applicableMoves[] = { 0, 262143/*18个1*/, 259263/*111111010010111111*/, 74943/*10010010010111111*/, 74898/*10010010010010010*/ };
int applicableMoves[] = {0,260991,185535/*111111010010111111*/, 74925/*10010010010111111*/, 74898/*10010010010010010*/};
// TODO: Encode as strings, e.g. for U use "ABCDABCD"
int affectedCubies[][8] = {						//对每一个块编码，前四位棱块，后四位角块（顶层右下角开始0.1.2.3，底层右下角开始4.5.6.7），相同块数字相同。
	{1, 2,  3,  0, 12, 13, 14, 15},{7, 6,  5,  4, 17, 18, 19, 16},
		{3, 11, 7,  9, 15, 14, 18, 17},{1,  8, 5, 10, 13, 12, 16, 19},
		{2, 10, 6, 11, 14, 13, 19, 18},{0,  9, 4,  8, 12, 15, 17, 16}};

/********************************************************************************
*对state状态进行旋转(顺时针90°180°270°)，返回旋转后的状态
*
*move=0时，U面顺时针旋转90°move=1时，U面顺时针旋转180°move=2时，U面顺时针旋转270°
*move=3时，D面顺时针旋转90°move=4时，D面顺时针旋转180°move=5时，D面顺时针旋转270°
*move=6时，F面顺时针旋转90°move=7时，F面顺时针旋转180°move=8时，F面顺时针旋转270°
*move=9时，B面顺时针旋转90°move=10时，B面顺时针旋转180°move=11时，B面顺时针旋转270°
*move=12时，L面顺时针旋转90°move=13时，L面顺时针旋转180°move=14时，L面顺时针旋转270°
*move=15时，R面顺时针旋转90°move=16时，R面顺时针旋转180°move=17时，R面顺时针旋转270°
*
**********************************************************************************/
vi applyMove(int move, vi state) {
	int turns = move % 3 + 1;			//move对3求余+1  旋转90°的次数
	int face = move / 3;				//move除3取整    定义旋转哪一个面
	while (turns--) {					/*顺时针旋转turns个90°*/
		vi oldState = state;
		for (int i = 0; i<8; i++) {			/*在旋转过程中分别对8个楞块和8个角块的方向进行赋值*/
			int isCorner = i > 3;			//将i>3的逻辑判断结果（0，1）赋给isCorner i>3才能取到affectedCubies中的后四位，即角块
			int target = affectedCubies[face][i] ;
			int killer = affectedCubies[face][(i & 3) == 3 ? i - 3 : i + 1] ;		//将面按顺序的下一个楞块或者角块的值取出来（用于移位）
			int orientationDelta = (i<4) ? (face>1 && face<4) : (face<2) ? 0 : 2 - (i & 1);			//顺时针旋转后方向改变量（0.1.2）
			state[target] = oldState[killer];														//用后一个替换前一个，完成顺时针旋转
			state[target + 20] = oldState[killer + 20] + orientationDelta;							//记录旋转后方向的值
			if (!turns)			/*如果turns!=0即还要旋转，则不进入；若turns==0，则进入求余，防止方向值超过（0.1）或（0.1.2）*/
				state[target + 20] %= 2 + isCorner;					//楞块和2求余，角块和3求余，不改变方向的值
		}
	}
	return state;
}
/*用于返回move的逆动作*/
int inverse(int move) {
	return move + 2 - 2 * (move % 3);
}
//----------------------------------------------------------------------
int phase;				//整个西斯尔思韦特的步骤
//----------------------------------------------------------------------
				/*取出输入状态的方向的值id*/
vi id(vi state) {

	//--- Phase 1: Edge orientations.（//第一步：棱块取向）
	if (phase < 2)
		return vi(state.begin() + 20, state.begin() + 32);			//返回输入state状态的棱块的取向，共12位，0表示方向正确，1表示方向错误（即翻转了180°）

	//-- Phase 2: Corner orientations, E slice edges.（//第二步：角块方向，E层（即中间层）棱块）
	if (phase < 3) {
		vi result(state.begin() + 31, state.begin() + 40);			//取角块的方向值给result
		for (int e = 0; e<12; e++)
			result[0] |= (state[e]>>3) << e;						// result[0]用于存E层（中间层）楞块的位置（用二进制表示）
		return result;												//返回角块的方向（0.1.2）和E层楞块的位置（result[0]）
	}

	//--- Phase 3: Edge slices M and S, corner tetrads, overall parity.（//第三步：M层S层的楞块，对应角块呈现正四面体型）
	if (phase < 4) {
		vi result(3);
		for (int e = 0; e<12; e++)
			result[0] |= ((state[e] > 7) ? 2 : (state[e] & 1)) << (2 * e);	//result[0]用24位存12个楞块位置正确，
		for (int c = 0; c<8; c++)
			result[1] |= ((state[c + 12] - 12) & 5) << (3 * c);				//result[1]用24位存放8个角块的位置
		for (int i = 12; i<20; i++)
			for (int j = i + 1; j<20; j++)
				result[2] ^= state[i] > state[j];					//result[2]=0表示角块方向正确，result[2]=1表示角块方向错误
		return result;
	}

	//--- Phase 4: The rest.
	return state;
}

int hello(){
	char  * argv[] = { "DB","UR", "UB", "UL", "DF","DR", "UF", "DL", "BR", "FL",  "FR", "BL",
		"UBL","URB", "UFR", "ULF", "DLB", "DFL","DRF",  "DBR" };
	string goal[] = { "UF", "UR", "UB", "UL", "DF", "DR", "DB", "DL", "FR", "FL", "BR", "BL",
		"UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR" };
	vi currentState(40), goalState(40);
	for (int i = 0; i<20; i++) {						/*该for循环的作用: 将当前魔方状态输入到数表currentState里，
														由字母顺序转化到数字顺序，字母顺序包涵了位置和方向，
														数字顺序用一位表示位置，一位表示方向。
														规则为：0-11/12-19存按goal里编号楞块/角块的数字位置，
														20-39存楞块和角块的方向
														楞块如需翻转，则20-31对应位置为1；
														角块如需旋转，则顺时针旋转90°记为1，顺时针旋转180°记为2*/
														//--- Goal state.
		goalState[i] = i;			//初始化目标魔方各个楞块和角块的位置

		//--- Current (start) state.（//输入魔方各个楞块和角块的位置）
		string cubie = argv[i];						//遍历输入的argv中的20个块
		/*和目标魔方块的位置比较，块需要顺时针旋转90°或者楞块翻转180°*/
		while ((currentState[i] = find(goal, goal + 20, cubie) - goal) == 20) {
			cubie = cubie.substr(1) + cubie[0];				//旋转后块的字母顺序
			currentState[i + 20]++;							//记录到方向，回到正确位置需要顺时针旋转180°为2，顺时针旋转90°为1
		}
	}
	for(int i=0;i<20;i++){
		currentState[i]=i;
		currentState[i+20]=(i<12?1:0);
	}
	//--- Dance the funky Thistlethwaite...（//开始牛逼的西斯尔思韦特操作）
	while (++phase < 5) {
		vi currentId = id(currentState), goalId = id(goalState);

		if (currentId == goalId) continue;
		queue<vi> q;q.push(currentState);	q.push(goalState);
		map<vi, vi> predecessor;map<vi, int> direction,lastMove;
		direction[currentId] = 1;	direction[goalId] = 2;
		while (1) {
			vi oldState = q.front();q.pop();    vi oldId = id(oldState); int& oldDir = direction[oldId];
			for (int move = 0; move<18; move++) {
				if (applicableMoves[phase] & (1 << move)) {
					vi newState = applyMove(move, oldState);
					vi newId = id(newState);
					int& newDir = direction[newId];
					if (newDir  &&  newDir != oldDir) {
						if (oldDir > 1) { swap(newId, oldId);	move = inverse(move); }
						vi algorithm(1, move);

						while (newId != goalId) {
              algorithm.push_back(inverse(lastMove[newId]));
							newId = predecessor[newId];}
						while (oldId != currentId) {
              algorithm.insert(algorithm.begin(), lastMove[oldId]);
							oldId = predecessor[oldId];}
						for (int i = 0; i<(int)algorithm.size(); i++) {
							cout << "UDFBLR"[algorithm[i] / 3] << algorithm[i] % 3 + 1;
							answer.push_back(algorithm[i]);
							currentState = applyMove(algorithm[i], currentState);
						}
						cout <<endl;
						goto nextPhasePlease;
					}
					if (!newDir) {q.push(newState);newDir = oldDir;
						lastMove[newId] = move;predecessor[newId] = oldId;}
				}
			}
		}
	nextPhasePlease:
		;
	}
	return 0;
}
