#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <string.h>
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

#define maxl 100
#define maxn 100

int n, l, w[maxl][maxl], a[maxn][maxl], multi_result[maxl];

int get_w(int x, int y) {
	int w = 0;
	for (int i = 1; i <= n; i++) {
		int t1, t2;
		t1 = a[i][x] == 0 ? -1 : a[i][x];
		t2 = a[i][y] == 0 ? -1 : a[i][y];
		w += t1 * t2;
	}
	return w;
}

void matrix_multi(int t[maxl]) {
	memset(multi_result, 0, sizeof(multi_result));
	for (int i = 1; i <= l; i++) {
		for (int j = 1; j <= l; j++) {
			multi_result[i] += t[j] * w[i][j];
		}
		multi_result[i] >= 0 ? multi_result[i] = 1 : multi_result[i] = 0;
	}
	for (int i = 1; i <= l; i++) {
		cout << multi_result[i] << " ";
	}
	cout << endl;
}

int main() {
	memset(a, 0, sizeof(a));
	memset(w, 0, sizeof(w));
	cout << "������Ҫ��������������ÿ�����ݵĳ���:" << endl;
	cin >> n >> l;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= l; j++) {
			cin >> a[i][j];
		}
	}
	for (int i = 1; i < l; i++) {
		for (int j = i + 1; j <= l; j++) {
			w[i][j] = get_w(i, j);
		}
	}
	for (int i = 2; i <= l; i++) {
		for (int j = 1; j < i; j++) {
			w[i][j] = w[j][i];
		}
	}
	cout << "����ȨֵΪ:" << endl;
	for (int i = 1; i <= l; i++) {
		for (int j = 1; j <= l; j++) {
			cout << w[i][j]<<" ";
		}
		cout << endl;
	}
	cout << "�������������:" << endl;
	int flag = 1;
	while (flag) {
		int t[maxl];
		memset(t, 0, sizeof(t));
		for (int i = 1; i <= l; i++) {
			cin >> t[i];
		}
		cout << "��ʼ����:" << endl;
		matrix_multi(t);
		while (memcmp(t, multi_result, l + 1)) {
			memcpy(t, multi_result, l * sizeof(int));
			matrix_multi(multi_result);
		}
		cout << "�ﵽ�ȶ�,�ȶ�״̬Ϊ:" << endl;
		for (int i = 1; i <= l; i++) {
			cout << multi_result[i] << " ";
		}
		cout << endl;
		cout << "��������������0������������������������:";
		cin >> flag;
	}
}
