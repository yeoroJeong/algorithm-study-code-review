#!/usr/bin/env python3
"""Validate solution paths, member/problem folders, size, encoding and Python syntax."""
from __future__ import annotations
import argparse, json, subprocess
from pathlib import Path
import yaml
ROOT=Path(__file__).resolve().parents[1]; CONFIG_PATH=ROOT/'study-config.json'; MAX_FILE_SIZE=500_000

def load_config(): return json.loads(CONFIG_PATH.read_text(encoding='utf-8'))
def changed(base,head):
    r=subprocess.run(['git','diff','--name-only','--diff-filter=ACMR',base,head],cwd=ROOT,check=True,capture_output=True,text=True)
    return [ROOT/x for x in r.stdout.splitlines() if x.strip()]
def all_files(): return sorted((ROOT/'solutions').rglob('*.py'))
def problem_folders(week: str) -> set[str]:
    path=ROOT/'problems'/week/'week.yml'
    if not path.is_file(): return set()
    data=yaml.safe_load(path.read_text(encoding='utf-8')) or {}; raw=data.get('problems') or []; items=[]
    if isinstance(raw,list): items=raw
    elif isinstance(raw,dict):
        for group in raw.values():
            if isinstance(group,list): items.extend(group)
    return {str(item.get('folder') or f"{item.get('site')}_{item.get('number')}").strip() for item in items if isinstance(item,dict)}
def validate(path: Path, config: dict):
    errors=[]; warnings=[]; rel=path.relative_to(ROOT).as_posix(); parts=path.relative_to(ROOT).parts
    if len(parts)<5 or parts[0]!='solutions' or not parts[1].startswith('week'):
        return [f"{rel}: solutions/weekXX/개인폴더/문제폴더/파일.py 위치여야 합니다."], warnings
    week,member,problem=parts[1],parts[2],parts[3]
    folders={m.get('folder') or m['name'] for m in config['members']}
    if member not in folders: errors.append(f"{rel}: study-config.json에 등록되지 않은 개인 폴더입니다.")
    valid_problems=problem_folders(week)
    if valid_problems and problem not in valid_problems: errors.append(f"{rel}: {week}/week.yml에 등록되지 않은 문제 폴더입니다.")
    if not path.exists(): return errors,warnings
    if path.stat().st_size==0: errors.append(f"{rel}: 빈 파일입니다.")
    elif path.stat().st_size>MAX_FILE_SIZE: errors.append(f"{rel}: 파일 크기가 500KB를 초과합니다.")
    try: content=path.read_text(encoding='utf-8')
    except UnicodeDecodeError: errors.append(f"{rel}: UTF-8로 읽을 수 없습니다."); return errors,warnings
    try: compile(content,str(path),'exec')
    except SyntaxError as e: errors.append(f"{rel}: Python 문법 오류 ({e.lineno}:{e.offset}) {e.msg}")
    header='\n'.join(content.splitlines()[:20]); missing=[w for w in ['문제:','링크:','시간 복잡도:'] if w not in header]
    if missing: warnings.append(f"{rel}: 상단 설명에서 {', '.join(missing)} 항목을 찾지 못했습니다.")
    if 'input("' in content or "input('" in content: warnings.append(f"{rel}: input() 안내 문자열은 출력 오류를 만들 수 있습니다.")
    return errors,warnings

def main():
    p=argparse.ArgumentParser(); p.add_argument('--base'); p.add_argument('--head'); a=p.parse_args()
    if bool(a.base)!=bool(a.head): print('ERROR: --base와 --head는 함께 사용해야 합니다.'); return 2
    config=load_config(); candidates=changed(a.base,a.head) if a.base else all_files(); files=[x for x in candidates if x.suffix=='.py' and 'solutions' in x.parts]
    if not files: print('검사할 풀이 Python 파일이 없습니다.'); return 0
    errors=[]; warnings=[]
    for f in files:
        e,w=validate(f,config); errors+=e; warnings+=w
        if not e: print(f"PASS: {f.relative_to(ROOT)}")
    if warnings:
        print('\nWARNINGS'); [print(f'- {x}') for x in warnings]
    if errors:
        print('\nERRORS'); [print(f'- {x}') for x in errors]; return 1
    print(f"\n검사 통과: 파일 {len(files)}개, 경고 {len(warnings)}개"); return 0
if __name__=='__main__': raise SystemExit(main())
