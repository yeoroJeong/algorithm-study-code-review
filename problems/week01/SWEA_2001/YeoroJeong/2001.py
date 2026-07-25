T = int(input())

for test_case in range(1, T + 1):
    N, M = map(int, input().split())

    # N × N 크기의 파리 개수 배열 입력
    arr = [list(map(int, input().split())) for _ in range(N)]

    # 한 번에 잡을 수 있는 최대 파리 수
    answer = 0

    # M × M 파리채의 왼쪽 위 시작 위치를 모두 탐색
    # 시작 인덱스는 0부터 N-M까지 가능하므로 N-M+1번 반복
    for i in range(N - M + 1):
        for j in range(N - M + 1):
            temp_sum = 0

            # 현재 시작 위치 (i, j)부터 M × M 영역의 합 계산
            for row in range(i, i + M):
                for col in range(j, j + M):
                    temp_sum += arr[row][col]

            # 지금까지 계산한 파리 수 중 최댓값 갱신
            answer = max(answer, temp_sum)

    print(f"#{test_case} {answer}")