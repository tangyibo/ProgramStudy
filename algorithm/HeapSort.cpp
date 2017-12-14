/*
 * 基于STL的push_heap实现
 * 【算法伪代码】
 * 0-建立一个最小堆（优先队列）,最小堆的大小控制在m之内
 * 1-for 每个数：
 * 2-----if 这个数比最小堆的堆顶元素大：
 * 3---------弹出最小堆的最小元素
 * 4---------把这个数插入到最小堆
 * 5-最小堆中的m个元素就是所要求的元素
 * 6-其中最小堆的作用就是保持里面始终有m个最大元素,且m个元素中最小的元素在堆顶.
 * 【算法复杂度】O(nlogm) 遍历O(n) 最小堆O(logm)
 * 【其他】如果要求n个数中取最小的m个,只要把算法反一下即可
 * */
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <time.h>
using namespace std;

bool desc_cmp(int a,int b)
{
	return a > b; 
}

int main(int argc, char *argv[])
{
	vector<int> val, val2, val3;
	vector<int>::iterator it;
	int m =5, n = 50, t;

	cout<<"产生的随机数如下："<< endl;
	srand(time(NULL));
	for (int i = 0; i < n; i ++) 
	{
		t = rand()%100;
		val2.push_back(t);
		val3.push_back(t);
		cout << t << " ";
	}
	cout <<endl;

	for (int i = 0; i < n; i ++) 
	{
		int t = val2[i];
		if (val.size() < m) 
		{
			val.push_back(t);
			push_heap(val.begin(), val.end(),desc_cmp);
			continue;
		}

		if (t > val[0])
		{
			pop_heap(val.begin(), val.end(),desc_cmp);
			val[m - 1] = t;
			push_heap(val.begin(), val.end(),desc_cmp);
		}
	}

	cout << "采用大顶堆取得最大的前 " << m << "个数是:" << endl;
	for (int i = 0; i < m; i ++)
		cout << val[i] << " ";
	cout << endl;

	cout << "采用stl::sort排序后的结果是:" << endl;
	sort(val3.begin(), val3.end(),desc_cmp);
	for (int i = 0; i < val3.size(); i ++)
		cout << val3[i] << " ";
	cout << endl;

	return 0;
}

