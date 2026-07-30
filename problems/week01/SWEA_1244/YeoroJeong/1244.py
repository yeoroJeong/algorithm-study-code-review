T = int(input())


def sol(cnt):
    global change_cnt, answer

    # 현재 숫자 배열을 하나의 정수 상태로 변환
    state = int(''.join(map(str, arr)))

    # 같은 교환 횟수에서 동일한 숫자 상태를 이미 탐색했다면 중단
    if state in visited[cnt]:
        return

    # 현재 상태를 방문 처리
    # 종료 깊이에서도 중복 상태를 막기 위해 종료 조건보다 먼저 처리
    visited[cnt].add(state)

    # 정해진 교환 횟수를 모두 사용한 경우 최댓값 갱신
    if cnt == change_cnt:
        answer = max(answer, state)
        return

    # 서로 다른 두 위치를 선택하여 교환
    for i in range(len(arr) - 1):
        for j in range(i + 1, len(arr)):
            # 숫자판 교환
            arr[i], arr[j] = arr[j], arr[i]

            # 교환 횟수를 1 증가시켜 다음 경우 탐색
            sol(cnt + 1)

            # 다른 교환 경우를 탐색하기 위해 원래 상태로 복구
            arr[i], arr[j] = arr[j], arr[i]


for test_case in range(1, T + 1):
    score, change = input().split()

    # 숫자판의 각 자릿수를 수정 가능한 리스트로 변환
    arr = list(map(int, score))

    # 반드시 수행해야 하는 교환 횟수
    change_cnt = int(change)

    # 만들 수 있는 가장 큰 숫자
    answer = 0

    # visited[cnt]: cnt번 교환했을 때 이미 확인한 숫자 상태
    visited = [set() for _ in range(change_cnt + 1)]

    sol(0)

    print(f"#{test_case} {answer}")