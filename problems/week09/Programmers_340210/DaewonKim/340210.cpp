#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

/*
1. 문자열 파싱

2. 수식 탐색. -> 2~9진법 까지 가능한 진법의 후보 추출
-> for base in 2~9까지
	for e in expres
		숫자를 모두 탐색 
		- if (현재 숫자가 base보다 크다면)
			 종료
		- 모든 숫자가 base보다 작으므로,
		- base 진법에 대한 수식을 실행.
			- base 진법을 기반으로 stoi(숫자, nullptr, base)로 해서 10진법 변환
			- 연산자 종료는 +, -만 있으므로 +면 그대로, 아니면 -실행.
			- 결과 값과 수식 C의 값(10진법으로 변환한 후의 값)을 비교
			- if( 값이 동일)
				- 현재 진법은 가능하다는 뜻이므로, 진법 후보에 추가

3.  X판별
- 가능한 진법이 1개라면 그 값을 X에 채움.
- 진법이 여러 개, 
	- 진법들에 대해서 result를 만듬.
	- result가 같으면, 그 result를 반환
	- 다르면, X를 그대로 설정.
			
	
*/

struct Expr {
    string a, op, b, c;   // A, 연산자(+/-), B, 결과 C ("X"일 수 있음)
};


/*
    10진수 정수 n → base진법 문자열
    stoi의 역방향은 표준 함수가 없으므로 직접 구현
    예) toBase(6, 6) = "10", toBase(6, 7) = "6"
    +
    -> 표기를 특정 진법으로 해야하기 때문에.
*/
string toBase(int n, int base) {
    if (n == 0) return "0";
    string s;
    while (n > 0) {
        s += char('0' + n % base);
        n /= base;
    }
    reverse(s.begin(), s.end());
    return s;
}

/*

    - stoi는 무효 숫자를 만나면 조용히 멈추므로 -> ex: (stoi("17", nullptr, 7) == 1)
    - stoi함수를 사용하기 위해 아래 유효성을 만족하는지 체크하는 함수를 먼저 시행해야함.
    - 현재 진법보다 숫자가 더 크다면, 그 진법으로 표현이 안되서
*/
bool validDigits(const string& s, int base) {
    for (char ch : s) {
        if (ch - '0' >= base) return false;
    }
    return true;
}

// A op B 를 base진법으로 계산해 10진수로 반환
int calc(const Expr& e, int base) {
    int x = stoi(e.a, nullptr, base);   // base진법 문자열 → 10진수
    int y = stoi(e.b, nullptr, base);
    return (e.op == "+") ? x + y : x - y;
}

vector<string> solution(vector<string> expressions) {
    // 파싱 
    vector<Expr> exprs;
    for (const string& line : expressions) {
        stringstream ss(line);
        Expr e;
        string eq;
        ss >> e.a >> e.op >> e.b >> eq >> e.c;
        exprs.push_back(e);
    }

    // 가능한 진법 후보 찾기
    vector<int> candidates;
    for (int base = 2; base <= 9; ++base) {
        bool ok = true;

        // 현재 진법에 대해서 모든 수식을 순회하면서 비교.
        for (const Expr& e : exprs) {
            // base 진법의 숫자가 현재 수식의 숫자보다 더 작거나 같으면 불가.
            // -> 6진법인데 숫자 7이 수식에 있다면, 불가능한 경우라고 판단.
            if (!validDigits(e.a, base) || !validDigits(e.b, base)) { ok = false; break; }
            if (e.c == "X") continue;
            if (!validDigits(e.c, base)) { ok = false; break; }

            // 위에서 유효성 검사를 통과했으므로 stoi가 안전하게 전체를 읽음
            // base 진법으로 계산한 결과가 수식과 다르다면? 
            if (calc(e, base) != stoi(e.c, nullptr, base)) 
            { 
                ok = false; 
                break; 
            }
        }

        // 모든 조건을 만족했으면 후보에 추가.
        if (ok) candidates.push_back(base);
    }

    // X 채우기 
    vector<string> answer;
    for (const Expr& e : exprs) {
        // X인 것만 보기 위해 x가 아닌 건 건너뜀.
        if (e.c != "X") continue;

        // 결과를 각 진법의 "표기 문자열"로 비교
        string result = "";
        bool same = true;
        for (int base : candidates) {
            string r = toBase(calc(e, base), base);
            if (result.empty()) result = r;
            else if (result != r) { same = false; break; }
        }

        answer.push_back(e.a + " " + e.op + " " + e.b + " = " + (same ? result : "?"));
    }

    return answer;
}