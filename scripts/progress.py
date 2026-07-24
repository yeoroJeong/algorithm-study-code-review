#!/usr/bin/env python3
"""Print problem submission counts per week and member."""
from __future__ import annotations
import json,re
from pathlib import Path
import yaml
ROOT=Path(__file__).resolve().parents[1]
def problem_names(week):
    path=ROOT/'problems'/week/'week.yml'
    if not path.is_file(): return []
    data=yaml.safe_load(path.read_text(encoding='utf-8')) or {}; raw=data.get('problems') or []; items=[]
    if isinstance(raw,list): items=raw
    elif isinstance(raw,dict):
        for group in raw.values():
            if isinstance(group,list): items.extend(group)
    return [str(i.get('folder') or f"{i.get('site')}_{i.get('number')}") for i in items if isinstance(i,dict)]
def main():
    config=json.loads((ROOT/'study-config.json').read_text(encoding='utf-8')); members=config['members']; weeks=sorted([p for p in (ROOT/'solutions').glob('week*') if p.is_dir()],key=lambda p:int(re.search(r'\d+',p.name).group()))
    if not weeks: print('등록된 주차가 없습니다.'); return 0
    print('| 주차 | '+' | '.join(m['name'] for m in members)+' | 총 제출 문제 |'); print('|---|'+'---|'*(len(members)+1)); grand=0
    for week in weeks:
        probs=problem_names(week.name); counts=[]
        for m in members:
            folder=m.get('folder') or m['name']; count=sum(any(f.is_file() and f.suffix=='.py' for f in (week/folder/problem).rglob('*')) if (week/folder/problem).is_dir() else False for problem in probs); counts.append(count)
        total=sum(counts); grand+=total; display=[f'{c}개' if c else '-' for c in counts]; print(f"| {week.name} | "+' | '.join(display)+f' | {total}개 |')
    print(f'\n전체 제출 문제: {grand}개'); return 0
if __name__=='__main__': raise SystemExit(main())
