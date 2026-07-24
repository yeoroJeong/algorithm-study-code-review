# 반복 실수 기록

스터디에서 여러 번 나온 실수를 공통 문서로 관리합니다. 새로운 실수가 반복되면 예시와 해결 원칙을 함께 추가합니다.

## 좌표와 배열

### 행·열과 x·y 혼동

```python
# 권장
row, col = 0, 0
next_row = row + dr[d]
next_col = col + dc[d]
```

한 문제 안에서는 `row/col` 또는 `x/y` 중 하나만 사용합니다.

### 경계 검사보다 배열 접근을 먼저 함

```python
# 잘못된 순서
if board[next_row][next_col] == 1 and 0 <= next_row < n:
    ...

# 올바른 순서
if 0 <= next_row < n and 0 <= next_col < m:
    if board[next_row][next_col] == 1:
        ...
```

## BFS와 DFS

### BFS에서 list.pop(0) 사용

`pop(0)`은 나머지 원소를 이동하므로 O(N)입니다.

```python
from collections import deque

queue = deque([start])
current = queue.popleft()
```

### 방문 표시 시점이 늦음

큐에 넣을 때 방문 처리하지 않으면 같은 좌표가 여러 번 들어갈 수 있습니다.

```python
visited[next_row][next_col] = True
queue.append((next_row, next_col))
```

## 백트래킹

### 상태 복구 누락

```python
selected.append(value)
backtrack()
selected.pop()  # 재귀 호출 이후 원상 복구
```

변경한 값과 복구해야 할 값을 같은 위치에서 확인할 수 있게 작성합니다.

### 매번 전체 배열 복사

구현은 쉽지만 입력과 재귀 깊이가 크면 메모리와 시간이 증가합니다. 변경 위치만 기록해 복구하는 방식과 비교합니다.

## Python

### 문자열을 직접 수정하려고 함

문자열은 불변 객체입니다.

```python
chars = list(text)
chars[index] = "A"
text = "".join(chars)
```

### 얕은 복사로 2차원 배열을 복사함

```python
# 각 행을 별도로 복사
copied = [row[:] for row in board]
```

### 온라인 저지 input에 안내 문구 사용

```python
# 제출 코드에서 사용하지 않음
n = int(input("숫자를 입력하세요: "))

# 올바른 형태
n = int(input())
```

안내 문구가 출력되어 오답이 될 수 있습니다.

## 복잡도

### 반복문 안의 숨은 비용을 놓침

다음 연산은 반복문 안에서 사용될 때 전체 복잡도를 키울 수 있습니다.

- 리스트에서 `value in values`: O(N)
- `list.pop(0)`: O(N)
- 정렬: O(N log N)
- 슬라이싱: 슬라이스 길이에 비례
- 2차원 배열 전체 복사: O(NM)

코드에 보이는 반복문의 개수뿐 아니라 내부 연산 비용까지 계산합니다.
