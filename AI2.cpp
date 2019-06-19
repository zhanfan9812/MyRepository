#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <string.h>
#include <queue>
#include <math.h>

using namespace std;

struct state {
	int f = 0;	//��ֵ����
	int level;
	int zp;		//0��λ��
	string s;
	bool operator <(const state &s)const {
		return f > s.f;
	}
};

string dest = "123804765";//���������h2
int htype;
priority_queue<state>pq;
map<string, bool>visit;

//1:�α��ϵ�h(x):"����λ"��������
//2:h(x):���е���������Ŀ��λ��������ܲ���
//3:h(x):����
int h(string a) {	
	int x = 0;
	if (htype==1) {
		for (int i = 0; i < 9; i++) {
			if (a[i] == '0')	continue;
			if (a[i] != dest[i]) {
				x++;
			}
		}
	}
	if (htype == 2) {
		int hp[9] = { 4,0,1,2,5,8,7,6,3 };//ÿ����������Ŀ��λ��
		for (int i = 0; i < 9; i++) {
			int t, x1, y1, x0, y0;
			t = hp[a[i] - '0'];
			x0 = t % 3; y0 = t / 3;
			x1 = i % 3; y1 = i / 3;
			x += abs(x1 - x0) + abs(y1 - y0);
		}
	}
	return x;
}

int main(){
	int f = 0;
	int Gstart[9], szp;
	int nodecount = 0;
	int dir[4] = { -3,3,-1,1 };
	string sg;
	state start;
	cout << "�������ʼ״̬:" << endl;
	for (int i = 0; i < 9; i++) {
		cin >> Gstart[i];
		if (Gstart[i] == 0)
			szp = i;
		sg += Gstart[i] + '0';
	}
	cout << "��ѡ��h����:" << endl;
	cout << "1:h(x):\"����λ\"��������" << endl;
	cout << "2:h(x):���е���������Ŀ��λ��������ܲ���" << endl;
	cout << "3:h(x):�������h(x)=0" << endl;
	cin >> htype;
	cout << "��ʼͼΪ:" << endl;
	for (int i = 0; i < 9; i++) {
		cout << Gstart[i] << " ";
		if ((i + 1) % 3 == 0)	cout << endl;
	}
	cout << "Ŀ��ͼΪ:" << endl;
	for (int i = 0; i < 9; i++) {
		cout << dest[i] << " ";
		if ((i + 1) % 3 == 0)	cout << endl;
	}
	start.s = sg;
	start.zp = szp;
	start.f = h(start.s);
	start.level = 0;
	pq.push(start);
	visit[start.s] = true;
	nodecount++;
	while (!pq.empty()) {
		state now = pq.top();
		pq.pop();
		if (now.s == dest) {
			f = 1;
			cout << "����Ŀ��״̬" << endl;
			cout << "��������Ϊ: " << now.f << endl;
			cout << "��չ�ڵ���Ϊ: " << nodecount << endl;
			break;
		}
		for (int i = 0; i < 4; i++) {
			state next = now;
			next.zp += dir[i];
			if (next.zp < 0 || next.zp>8)
				continue;
			if ((now.zp == 3 && i == 2) || (now.zp == 5 && i == 3))//�м���Խ�����⴦��
				continue;
			next.s[now.zp] = next.s[next.zp];
			next.s[next.zp] = '0';
			if (visit.count(next.s))
				continue;
			next.level++;
			next.f = h(next.s) + next.level;
			pq.push(next);
			visit[next.s] = true;
			nodecount++;
		}
	}
	if (f = 0) {
		cout << "�޷�����Ŀ��״̬" << endl;
	}
	return 0;
}
