#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <string.h>
#include <queue>
#include <math.h>
#include <time.h>
#include <stdlib.h> 
#include <algorithm>

using namespace std;

#define cities 10
#define groupNum 50
#define mutatePro 0.005 
#define iterationNum 500 
#define crossNum 20

int chooseStrategy;
int mutateStrategy;

int distances[cities][cities];
/*
0 1 2 3 4 5 6 7 8 9
1 0 1 2 3 4 5 6 7 8
2 1 0 1 2 3 4 5 6 7
3 2 1 0 1 2 3 4 5 6
4 3 2 1 0 1 2 3 4 5
5 4 3 2 1 0 1 2 3 4
6 5 4 3 2 1 0 1 2 3
7 6 5 4 3 2 1 0 1 2
8 7 6 5 4 3 2 1 0 1
9 8 7 6 5 4 3 2 1 0
*/

struct Group {
	int path[cities];//路径
	int length;//适应度
	bool operator <(const Group &g)const {
		return length < g.length;
	}
}group[groupNum];

//随机产生一个 0-(cities-1) 的数
int produce() { return (rand() % (cities)); }

void getLength(int x) {
	group[x].length = 0;
	for (int j = 0; j < cities - 1; j++) {
		group[x].length += distances[group[x].path[j]][group[x].path[j + 1]];
	}
	group[x].length += distances[group[x].path[cities - 1]][group[x].path[cities]];
}

void init() {
	cout << "请输入十个城市间的距离:" << endl;
	for (int i = 0; i < cities; i++) {
		for (int j = 0; j < cities; j++) {
			cin >> distances[i][j];
		}
	}
	cout << "请输入选择策略:" << endl;
	cout << "1:以适应性的倒数为参数进行选择" << endl;
	cout << "2:以适应性的倒数的平方为参数进行选择" << endl;
	cin >> chooseStrategy;
	cout << "请输入变异策略:" << endl;
	cout << "1:互换变异" << endl;
	cout << "2:逆转变异" << endl;
	cin >> mutateStrategy;
	for (int i = 0; i < groupNum; i++) {
		int visit[cities], gnum = 0, tempstep;
		memset(visit, 0, sizeof(visit));
		for (int j = 0; j < cities; j++) {
			while (1) {
				tempstep = produce();
				if (visit[tempstep] == 0) {
					visit[tempstep] = 1;
					break;
				}
			}
			group[i].path[j] = tempstep;
		}
		getLength(i);
;	}
}

int crossStart = 3, crossEnd = 6;//染色体交换的始末点

int findp(int x, int a) {
	for (int i = 0; i < cities; i++) {
		if (i == crossStart) {
			i = crossEnd;
			continue;
		}
		if (group[a].path[i] == x)
			return i;
	}
	return -1;
}

//第x和第y个个体进行染色体交叉,交换部位:0 1 2 |3 4 5 6| 7 8 9
void cross(int x,int y) {	
	int temp = 0;
	int gx[cities], gy[cities];
	for (int i = 0; i < cities; i++) {
		gx[i] = -1;gy[i] = -1;
	}
	/*
	例如:
	9 8 4 |3 6 7 1| 5 2 0
	8 7 1 |2 3 9 5| 4 6 0
	*/
	for (int i = crossStart; i <= crossEnd; i++) {
		temp = group[x].path[i];
		group[x].path[i] = group[y].path[i];
		group[y].path[i] = temp;
		gx[i] = group[x].path[i];
		gy[i] = group[y].path[i];
	}
	/*
	9 8 4 |2 3 9 5| 5 2 0
	8 7 1 |3 6 7 1| 4 6 0
	*/
	for (int i = crossStart; i <= crossEnd; i++) {
		for (int j = crossStart; j <= crossEnd; j++) {
			if (gx[i] == gy[j]) {
				gx[i] = -1;
				gy[j] = -1;
				break;
			}
		}
	}
	/*
	9 8 4 |2 3(-1) 9 5| 5 2 0
	8 7 1 |3(-1) 6 7 1| 4 6 0
	*/
	int yj = crossStart;
	for (int i = crossStart; i <= crossEnd; i++) {
		while (gx[i] == -1 && i < crossEnd)i++;
		while (gy[yj] == -1 && yj < crossEnd)yj++;
		if (gx[i] == -1 || gy[yj] == -1)continue;
		/*cout << "gx[i]" << gx[i]<<"group[x].path:";
		for (int j = 0; j < cities; j++) {
			cout << group[x].path[j] << " ";
		}*/
		int xp = findp(gx[i], x);

	//	cout << "xp:" << xp << endl;

		int yp = findp(gy[yj], y);
		
		group[x].path[xp] = gy[yj];
		group[y].path[yp] = gx[i];
		yj++;
	}
	/*
	7 8 4 |2 3 9 5| 1 6 0
	8 9 5 |3 6 7 1| 4 2 0
	*/
	getLength(x);
	getLength(y);
	//更换染色体交换的始末点
	crossStart = produce();
	crossEnd = produce();
	if (crossStart > crossEnd) {
		int tempC = crossStart;
		crossStart = crossEnd;
		crossEnd = tempC;
	}
}

void printbest(Group g[]) {
	sort(g, g + groupNum);
	cout << "族群中的最短路径:";
	for (int j = 0; j < cities; j++) {
		cout << g[0].path[j] << " ";
	}
	cout << "The shortest length:" << g[0].length;
	cout << endl;
}

//轮盘赌选择
int choose(Group g[]) {
	double p[groupNum], range[groupNum], r;
	double sum = 0;
	for (int i = 0; i < groupNum; i++) {
		//个体选择概率分配策略1:取适应度倒数为概率分配参数
		if(chooseStrategy==1)
			p[i] = 1 / double(g[i].length);
		//个体选择概率分配策略2:取适应度倒数的平方为参数,使适应度小的个体优势更明显
		else
			p[i] = 1 / double(g[i].length*g[i].length);

		sum += p[i];
	}
	range[0] = p[0] / sum;
	for (int i = 1; i < groupNum; i++) {
		range[i] = range[i - 1] + p[i] / sum;
	}
	//产生一个随机小数c
	r = rand() % 100;
	double c = r / 100;
	for (int i = 0; i < groupNum; i++) {
		if (c < range[i]) {
			return i;
		}
	}
}

//第x个个体进行突变
void mutate(int x) {
	double mp = (double(rand() % 100001)) / 100000;
	//cout << "mp:" << mp;
	if (mp < mutatePro) {
		int temp, py, px = produce();
		while (1) {
			py = produce();
			if (px != py)	
				break;
		}
		//互换突变
		if (mutateStrategy == 1) {
			cout << x << "号个体发生互换突变" << endl;
			temp = group[x].path[px];
			group[x].path[px] = group[x].path[py];
			group[x].path[py] = temp;
		}
		//逆转突变
		else {
			cout << x << "号个体发生逆转突变" << endl;
			int tempPath[cities];//路径逆序装入tempPath
			memset(tempPath, 0, sizeof(tempPath));
			for (int i = py; i >= px; i--) {
				int tempP = py + px - i;
				tempPath[tempP] = group[x].path[i];
			}
			for (int i = py; i >= px; i--) {
				group[x].path[i] = tempPath[i];
			}
		}
	}
	return;
}

//用x,y的路径覆盖掉群体中适应性最差的两个个体
void coverWorst(int x,int y) {
	int wno = 0, lengthMax = 0;
	for (int i = 0; i < groupNum; i++) {
		if (group[i].length > lengthMax) {
			wno = i;
			lengthMax = group[i].length;
		}
	}
	for (int i = 0; i < cities; i++) {
		group[wno].path[i] = group[x].path[i];
	}
	int wno1 = 0, lengthMax1 = 0;
	for (int i = 0; i < groupNum; i++) {
		if (group[i].length > lengthMax1&&i != wno) {
			wno1 = i;
			lengthMax1 = group[i].length;
		}
	}
	for (int i = 0; i < cities; i++) {
		group[wno1].path[i] = group[y].path[i];
	}
	getLength(wno);
	getLength(wno1);
}

void iterate() {
	//每次迭代进行二十次选择交叉，产生的结果覆盖掉适应度最差的两个个体
	for (int i = 0; i < iterationNum; i++) {
		for (int j = 0; j < crossNum; j++) {
			int cx, cy;
			cx = choose(group);
			cy = choose(group);
			while (cx == cy) {
				cy = choose(group);
			}
			coverWorst(cx, cy);
			cross(cx, cy);
		}
		cout<<"第"<<i+1<<"代:"; 
		printbest(group);
		for (int j = 0; j < groupNum; j++) {
			mutate(j);
		}
	}
}

int main() {
	srand((unsigned)time(NULL));
	init();
	iterate();
}
