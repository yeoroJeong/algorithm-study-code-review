"""Regression and seeded oracle comparisons for the attached week 08 solutions."""
from collections import deque
from itertools import permutations
from pathlib import Path
import random
import subprocess
import sys
import unittest

ROOT = Path(__file__).resolve().parents[1] / 'problems/week08'


def run_solution(folder, filename, cases, grid=False):
    lines = [str(len(cases))]
    for arr in cases:
        lines.append(f'{len(arr)} {len(arr[0])}' if grid else str(len(arr)))
        lines.extend(' '.join(map(str, row)) for row in arr)
    result = subprocess.run([sys.executable, str(ROOT / folder / 'YeoroJeong' / filename)],
                            input='\n'.join(lines) + '\n', text=True, capture_output=True,
                            check=True, timeout=20)
    output = result.stdout.splitlines()
    assert len(output) == len(cases), output
    for i, line in enumerate(output, 1):
        assert line.startswith(f'#{i} '), line
    return [int(line.split()[1]) for line in output]


def escape_oracle(arr):
    # Build an explicit state graph and relax edges to compute shortest paths.
    n, m = len(arr), len(arr[0])
    start = next((r, c, 0) for r in range(n) for c in range(m) if arr[r][c] == 3)
    dist = {start: 0}
    for _ in range(2 * n * m):
        changed = False
        for (r, c, used), cost in list(dist.items()):
            for dr, dc in ((0, 1), (0, -1), (1, 0), (-1, 0)):
                nr, nc = r + dr, c + dc
                if not (0 <= nr < n and 0 <= nc < m):
                    continue
                next_used = used + (arr[nr][nc] == 1)
                state = nr, nc, next_used
                if next_used <= 1 and cost + 1 < dist.get(state, float('inf')):
                    dist[state] = cost + 1
                    changed = True
        if not changed:
            break
    return min((cost for (r, c, _), cost in dist.items() if arr[r][c] == 2), default=-1)


def assignment_oracle(arr):
    return min((sum(arr[i][job] for i, job in enumerate(order))
                for order in permutations(range(len(arr)))
                if all(arr[i][job] > 0 for i, job in enumerate(order))), default=-1)


class Week08Tests(unittest.TestCase):
    def test_escape(self):
        cases = [[[3, 2]], [[3, 1, 2]], [[3, 1, 1, 2]],
                 [[3, 0, 0], [1, 1, 0], [2, 0, 0]],
                 [[3, 0, 1, 0], [1, 0, 1, 2], [0, 0, 0, 0]]]
        rng = random.Random(8081)
        for _ in range(200):
            n, m = rng.randint(2, 5), rng.randint(2, 5)
            arr = [[int(rng.random() < .45) for _ in range(m)] for _ in range(n)]
            a, b = rng.sample(range(n * m), 2)
            arr[a // m][a % m], arr[b // m][b % m] = 3, 2
            cases.append(arr)
        self.assertEqual(run_solution('ETC_wall_break_escape', 'algo1_jeong.py', cases, True),
                         [escape_oracle(arr) for arr in cases])

    def test_assignment(self):
        cases = [[[7]], [[0]], [[1, 0], [1, 0]], [[9, 1], [1, 9]],
                 [[3, 3], [3, 3]]]
        rng = random.Random(8082)
        for _ in range(120):
            n = rng.randint(1, 7)
            cases.append([[rng.randint(0, 12) for _ in range(n)] for _ in range(n)])
        self.assertEqual(run_solution('ETC_task_assignment', 'algo2.py', cases),
                         [assignment_oracle(arr) for arr in cases])


if __name__ == '__main__':
    unittest.main()
