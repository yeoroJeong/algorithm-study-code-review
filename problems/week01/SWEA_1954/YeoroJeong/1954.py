T = int(input())

# 우 → 하 → 좌 → 상 순서로 이동
dj = [1, 0, -1, 0]
di = [0, 1, 0, -1]

for test_case in range(1, T + 1):
    N = int(input())

    # N × N 크기의 배열을 0으로 초기화
    arr = [[0] * N for _ in range(N)]

    # 현재 이동 방향
    # 0: 오른쪽, 1: 아래, 2: 왼쪽, 3: 위
    direction = 0

    # 배열에 넣을 숫자
    num = 1

    # 현재 위치
    current_i, current_j = 0, 0

    while True:
        # 현재 위치에 숫자 기록
        arr[current_i][current_j] = num

        # N²까지 채웠다면 달팽이 배열 완성
        if num == N * N:
            break

        # 현재 방향으로 이동했을 때의 다음 위치
        next_i = current_i + di[direction]
        next_j = current_j + dj[direction]

        # 다음 위치가 배열 범위를 벗어나거나
        # 이미 숫자가 채워진 위치라면 시계 방향으로 회전
        if (
            not 0 <= next_i < N
            or not 0 <= next_j < N
            or arr[next_i][next_j] != 0
        ):
            direction = (direction + 1) % 4

        # 결정된 방향으로 한 칸 이동
        current_i += di[direction]
        current_j += dj[direction]

        # 다음에 기록할 숫자 증가
        num += 1

    print(f"#{test_case}")

    # 한 행씩 공백으로 구분하여 출력
    for row in arr:
        print(*row)