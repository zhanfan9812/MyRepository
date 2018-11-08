#include<iostream>
#include<stdio.h>
#include<deque>
#include<queue>
#include<algorithm>

using namespace std;

const int N=1001;

//priority_queue��Ϊ��������,sortΪ��������.

struct ant{
	int dis;
	bool operator <(const ant &c)const{
	    return dis<c.dis;//dis>c.dis���Եߵ�����˳��
		//��ʱΪ����˳��
	}
};


deque<ant> dq;
priority_queue<ant> pq;

int main(){
	int n;
//	scanf("%d",&n);
	ant a[5];
	for(int i=0;i<5;i++){
		scanf("%d",&a[i].dis);
		dq.push_front(a[i]);
		pq.push(a[i]);
	}
	sort(dq.begin(),dq.end());
	cout<<"priority_queue:"<<endl;
	while(!pq.empty()) {
        cout<<pq.top().dis<<endl;
        pq.pop();
    }
    cout<<"deque:"<<endl;
	while(!dq.empty()) {
        cout<<dq.front().dis<<endl;
        dq.pop_front();
    }
}
