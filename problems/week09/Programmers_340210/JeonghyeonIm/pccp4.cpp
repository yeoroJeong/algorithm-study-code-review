/*
1. 수식을 A, 연산자, B, C로 쪼갠다.
2. 진법 2~9를 하나씩 가정하여 아래 두 가지를 검사한다
(1) 모든 수식의 A, B, 그리고 X가 아닌 C의 각 자릿수가 진법보다 작은가
(2) X가 아닌 수식은 그 진법으로 계산했을 때 A oper B == C 가 맞는가
3. 2의 두 가지 조건을 모두 통과해야 진법 후보가 됨
4. X 수식마다 후보 진법 각각으로 결과를 계산해 그 진법 문자열로 변환
(1) 후보가 달라도 결과 문자열이 전부 같으면 그 값을 씀
(2) 하나라도 다르다면 ? 박음
(2~9 진법 중 하나)
*/

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

using namespace std;

// base진법 문자열 s -> 10진수 정수
int toDec(const string& s, int base) {
    int v = 0;
    for (char c : s) v = v * base + (c - '0');
    return v;
}

// 10진수 정수 v -> base진법 문자열
string fromDec(int v, int base) {
    if (v == 0) return "0";
    string r;
    while (v > 0) {
        r += char('0' + v % base); // 낮은 자리부터 쌓임
        v /= base;
    }
    reverse(r.begin(), r.end()); // 뒤집어서 높은 자리가 앞으로
    return r;
}

// s의 모든 자릿수가 base 미만인지 (그 진법에서 쓸 수 있는 숫자인지)
bool okDigits(const string& s, int base) {
    for (char c : s) {
        if (c - '0' >= base) {
            return false;
        }
    }
    return true;
}

vector<string> solution(vector<string> expressions) {
    vector<string> answer;
    int n = expressions.size(); // 수식 몇개냐

    vector<string> A(n), B(n), C(n); // 각 수식의 A, B, C
    vector<char> op(n); // operator

    // 1. 파싱: "A + B = C" -> A, op, B, C
    for (int i = 0; i < n; i++) {
        stringstream ss(expressions[i]);
        string eq;
        ss >> A[i] >> op[i] >> B[i] >> eq >> C[i];
    }

    // 2. 가능한 진법 후보 찾기
    vector<int> cand; // 후보 진법 배열
    for (int base = 2; base <= 9; base++) {
        bool ok = true;
        for (int i = 0; i < n && ok; i++) {
            // A, B 자릿수 검사 (X 수식 포함)
            if (!okDigits(A[i], base) || !okDigits(B[i], base)) {
                ok = false;
                break;
            }
            if (C[i] == "X") continue; // 지워진 수식은 계산 검증 불가
            if (!okDigits(C[i], base)) {
                ok = false;
                break;
            }

            int a = toDec(A[i], base);
            int b = toDec(B[i], base);
            int c = toDec(C[i], base);
            int r = (op[i] == '+') ? a + b : a - b; // oper는 + 아니면 -밖에 없음
            if (r != c) ok = false; // 수식이 안 맞으면 탈락
        }
        if (ok) cand.push_back(base);
    }

    // 3. X 수식 채우기
    for (int i = 0; i < n; i++) {
        if (C[i] != "X") continue;

        set<string> results; // 후보 진법별 결과 모음 (중복 제거)
        for (int base : cand) {
            int a = toDec(A[i], base);
            int b = toDec(B[i], base);
            int r = (op[i] == '+') ? a + b : a - b;
            results.insert(fromDec(r, base));
        }

        // 결과가 한 종류면 확정, 여러 종류면 ?
        string res = (results.size() == 1) ? *results.begin() : "?";
        answer.push_back(A[i] + " " + op[i] + " " + B[i] + " = " + res);
    }
    return answer;
}