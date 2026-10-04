"""
문제: 최소 시간 업무 배정 (첨부 코드 기반 설명형 제목)
작성자: 정현수
원본 파일: algo2.py
공식 문제 번호·링크: 미제공
알고리즘: 백트래킹, 가지치기
시간 복잡도: O(N*N!) (리스트 복사 포함 상한)
공간 복잡도: O(N^2) (입력 및 재귀 중 남은 업무 리스트)
검증: 로컬 경계값 및 무작위 비교; 공식 채점 결과 미확인
"""

# ==============================================
# 코드 제출시 아래 2줄은 반드시 주석처리 하여 제출
# import sys
#
# sys.stdin = open('algo2_sample_in.txt')


# ==============================================
# 함수명처럼 문제를 풀기 위한 함수 문제풀이를 위한 매커니즘이 적용되어있다.
# 매개변수로는 현재 체크해야할 팀원 man(1번 팀원부터 N번 팀원까지) (인덱스는 0번 ~N-1번으로)
# 아직 수행하지 않은 업무들 remains
# 현재까지 처리시간의 합 cost
# 가 존재한다.
def solve(man, remains, cost):
    # 함수 내부에서 N값 사용 및 정답 수정이 가능하게 해준 뒤.
    global ans, N
    # 참고 칸에 모든 경우를 확인하는 방식이 시간제한을 넘을 수 있다고 했기에,
    # 현재 cost 가 관리하고 있는 정답보다 크거나 같을 시,
    # 업무 시간이 음수가 없기에 정답이 될수 없으므로 가지치기.
    if cost >= ans:
        return
    # 모든 업무를 처리했을 시, 팀원의 번호 man이 N이 되므로
    # 둘중 한 개만 판정해도 모든 업무처리가 원활히 끝났음을 판정할 수 있으나,
    # 시각적 편의상 둘다 판정한다.
    if len(remains) == 0 and man == N:
        # 가지치기 되지 않고 모든 업무를 처리했으면
        # 정답을 기존 정답이랑 cost 중 작은 값으로 최신화.
        ans = min(ans, cost)
        return
    # 남은 업무를 다 돌면서
    for i in range(len(remains)):
        r_t = remains[i]
        # 만약 해당 팀원이 해당 업무를 맡을 수 없으면
        if arr[man][r_t] == 0:
            # 건너뛰고
            continue
        # 아니면 해당업무 시켜보고, 남은 업무에서 해당업무 제외한채로
        # 재귀적 반복
        solve(man + 1, remains[:i] + remains[i + 1:], cost + arr[man][r_t])
    return


# 아래에 코드를 작성하세요.
T = int(input())
for test_case in range(1, T + 1):
    # 팀원 수이자, 업무 수를 받아오고
    N = int(input())
    # 팀원별 매핑된 업무 처리능력을 받아오고
    arr = [list(map(int, input().split())) for _ in range(N)]
    # 최소값을 찾는 문제이기에 , min 함수를 사용하기 편하도록 초기값을 굉장히 큰 값으로 설정
    ans = float('inf')

    # 전체 업무 리스트를 배열로 만든 뒤
    task = [i for i in range(N)]
    # 함수 실행
    solve(0, task, 0)
    # 정답이 바뀌지 않았다면 배정이 불가능 한 것이므로
    if ans == float('inf'):
        # 정답을 -1로 바꾸어 출력
        ans = -1
    print(f"#{test_case} {ans}")
