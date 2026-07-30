#include<iostream>
#include<string>
#include<algorithm>
#include<unordered_set>
using namespace std;

string input;
int len;
int maxchange;
string mx;

unordered_set <string> visited[11];

void back(string cur, int cnt) { // cnt: 현재 교환횟수
	if (mx < cur && cnt == maxchange) {
		mx = cur;
		return;
	}
	if (cnt >= maxchange) {
		return;
	}

	if (visited[cnt].count(cur)) {
		return;
	}
	visited[cnt].insert(cur);

	for (int i = 0; i < len; i++) {
		for (int j = i + 1; j < len; j++) {
			string sp = cur;
			swap(sp[i], sp[j]);
			back(sp, cnt + 1);
		}
	}
}


int main()
{
	int T;
	cin >> T;

	for (int t = 1; t <= T; t++) {
		cin >> input;
		len = input.length();
		cin >> maxchange;
		mx = "";

		for (int i = 0; i <= maxchange; i++) {
			visited[i].clear();
		}
		back(input, 0);

		cout << "#" << t << " " << mx <<  "\n";
	}
	return 0;
}