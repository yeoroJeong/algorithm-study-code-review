T = 10

# 대각선 이동 방향
# 오른쪽 아래: (1, 1)
# 왼쪽 아래: (1, -1)
di = [1, 1]
dj = [-1, 1]

for _ in range(1, T + 1):
    test_case = int(input())

    # 100 × 100 배열 입력
    arr = [list(map(int, input().split())) for _ in range(100)]

    # 행, 열, 대각선 합 중 최댓값
    answer = 0

    # 각 행의 합을 계산하여 최댓값 갱신
    for row in arr:
        answer = max(answer, sum(row))

    # 각 열의 합을 계산하여 최댓값 갱신
    for j in range(100):
        temp = 0

        for i in range(100):
            temp += arr[i][j]

        answer = max(answer, temp)

    # 왼쪽 위에서 오른쪽 아래로 내려가는 대각선 시작 위치
    down_i, down_j = 0, 0

    # 오른쪽 위에서 왼쪽 아래로 내려가는 대각선 시작 위치
    up_i, up_j = 0, 99

    # 두 대각선의 합
    down = 0
    up = 0

    # 두 대각선을 한 칸씩 내려가며 합 계산
    while True:
        # 행 인덱스가 배열의 범위를 벗어나면 종료
        if down_i == 100:
            break

        # 왼쪽 위 → 오른쪽 아래 대각선
        down += arr[down_i][down_j]

        # 오른쪽 위 → 왼쪽 아래 대각선
        up += arr[up_i][up_j]

        # 오른쪽 아래 방향으로 이동
        down_i += di[1]
        down_j += dj[1]

        # 왼쪽 아래 방향으로 이동
        up_i += di[0]
        up_j += dj[0]

    # 두 대각선의 합도 최댓값과 비교
    answer = max(answer, up, down)

    print(f"#{test_case} {answer}")