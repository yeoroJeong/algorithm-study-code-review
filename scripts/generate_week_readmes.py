from __future__ import annotations

import argparse
import sys
from pathlib import Path
from typing import Any

import yaml

ROOT = Path(__file__).resolve().parents[1]
PROBLEMS_ROOT = ROOT / "problems"
SOLUTIONS_ROOT = ROOT / "solutions"
DEFAULT_EXTENSIONS = {".py"}
LEVEL_ORDER = ("쉬움", "중간", "어려움")
LEVEL_ICONS = {"쉬움": "🟢", "중간": "🟡", "어려움": "🔴", "미분류": "⚪"}

class ConfigError(ValueError):
    pass

def esc(value: object) -> str:
    return str(value).replace("|", r"\|").replace("\n", " ").strip()

def week_label(value: object, directory_name: str) -> str:
    if isinstance(value, int): return f"{value}주차"
    text = str(value or "").strip()
    if text: return text if text.endswith("주차") else f"{text}주차"
    digits = "".join(ch for ch in directory_name if ch.isdigit())
    return f"{int(digits)}주차" if digits else directory_name

def members(raw: Any) -> list[dict[str,str]]:
    if not isinstance(raw, list) or not raw:
        raise ConfigError("members에는 스터디원을 한 명 이상 작성해야 합니다.")
    result=[]
    for item in raw:
        if not isinstance(item, dict): raise ConfigError("members 항목은 객체여야 합니다.")
        folder=str(item.get("folder","")).strip(); name=str(item.get("name") or folder).strip(); github=str(item.get("github","")).strip()
        if not folder: raise ConfigError("모든 멤버에는 folder가 필요합니다.")
        if "/" in folder or "\\" in folder: raise ConfigError(f"잘못된 멤버 폴더명: {folder}")
        result.append({"folder":folder,"name":name,"github":github})
    return result

def problem_item(item: Any, index: int, level: str) -> dict[str,str]:
    if not isinstance(item, dict): raise ConfigError(f"{level} {index}번째 문제는 객체여야 합니다.")
    site=str(item.get("site","")).strip(); number=str(item.get("number","")).strip(); title=str(item.get("title","")).strip(); folder=str(item.get("folder","")).strip()
    if not site or not number or not title: raise ConfigError(f"{level} {index}번째 문제에 site, number, title이 필요합니다.")
    if not folder: folder=f"{site}_{number}".replace(" ","_")
    return {"level":level,"site":site,"number":number,"title":title,"folder":folder,"difficulty":str(item.get("difficulty","-")).strip() or "-","category":str(item.get("category","-")).strip() or "-","url":str(item.get("url","")).strip()}

def problems(raw: Any) -> list[dict[str,str]]:
    if isinstance(raw, list): return [problem_item(v,i,"미분류") for i,v in enumerate(raw,1)]
    if not isinstance(raw, dict) or not raw: raise ConfigError("problems가 비어 있습니다.")
    unknown=set(raw)-set(LEVEL_ORDER)
    if unknown: raise ConfigError(f"지원하지 않는 난이도 그룹: {', '.join(sorted(unknown))}")
    result=[]
    for level in LEVEL_ORDER:
        group=raw.get(level,[]) or []
        if not isinstance(group,list): raise ConfigError(f"{level} 그룹은 목록이어야 합니다.")
        result.extend(problem_item(v,i,level) for i,v in enumerate(group,1))
    if not result: raise ConfigError("문제를 한 개 이상 작성해야 합니다.")
    return result

def extensions(config: dict[str,Any]) -> set[str]:
    submission=config.get("submission") or {}
    raw=submission.get("extensions") or list(DEFAULT_EXTENSIONS)
    if not isinstance(raw,list): raise ConfigError("submission.extensions는 목록이어야 합니다.")
    values=set()
    for value in raw:
        ext=str(value).strip().lower()
        if ext and not ext.startswith('.'): ext='.'+ext
        if ext: values.add(ext)
    return values or set(DEFAULT_EXTENSIONS)

def has_submission(week_dir: str, member_folder: str, problem_folder: str, allowed: set[str]) -> bool:
    target=SOLUTIONS_ROOT/week_dir/member_folder/problem_folder
    if not target.is_dir(): return False
    return any(p.is_file() and p.suffix.lower() in allowed for p in target.rglob('*'))

def link(problem: dict[str,str]) -> str:
    label=f"{problem['number']} {problem['title']}"
    return f"[{esc(label)}]({problem['url']})" if problem['url'] else esc(label)

def append_problem_table(lines: list[str], items: list[dict[str,str]]) -> None:
    lines += ["| 사이트 | 문제 | SWEA 난이도 | 분류 | 제출 문제 폴더 |","|---|---|:---:|---|---|"]
    for p in items:
        lines.append("| "+" | ".join([esc(p['site']),link(p),esc(p['difficulty']),esc(p['category']),f"`{esc(p['folder'])}`"])+" |")

def render(week_directory: Path, config: dict[str,Any]) -> str:
    label=week_label(config.get('week'),week_directory.name)
    period=config.get('period') or {}; start=str(period.get('start','')).strip(); end=str(period.get('end','')).strip(); deadline=str(config.get('deadline','')).strip(); notes=str(config.get('notes','')).strip()
    member_list=members(config.get('members')); problem_list=problems(config.get('problems')); allowed=extensions(config)
    lines=["<!-- AUTO-GENERATED FILE: week.yml과 solutions 폴더를 기준으로 생성됩니다. 직접 수정하지 마세요. -->","",f"# {esc(label)} 알고리즘 문제",""]
    if start or end: lines.append(f"- **진행 기간:** {esc(start)} ~ {esc(end)}")
    if deadline: lines.append(f"- **제출 마감:** {esc(deadline)}")
    if start or end or deadline: lines.append("")
    lines += ["## 문제 목록",""]
    grouped=any(p['level']!='미분류' for p in problem_list)
    if grouped:
        for level in LEVEL_ORDER:
            items=[p for p in problem_list if p['level']==level]
            if items:
                lines += [f"### {LEVEL_ICONS[level]} {level}",""]; append_problem_table(lines,items); lines.append("")
    else:
        append_problem_table(lines,problem_list); lines.append("")
    lines += ["## 제출 현황",""]
    lines.append("| 스터디원 | "+" | ".join(esc(p['number']) for p in problem_list)+" | 진행률 |")
    lines.append("|---|"+"".join(":---:|" for _ in problem_list)+":---:|")
    for m in member_list:
        states=[]; completed=0
        for p in problem_list:
            ok=has_submission(week_directory.name,m['folder'],p['folder'],allowed); states.append('✅' if ok else '❌'); completed+=int(ok)
        display=f"{esc(m['name'])} (`{esc(m['folder'])}`)" if m['name']!=m['folder'] else f"`{esc(m['folder'])}`"
        lines.append(f"| {display} | "+" | ".join(states)+f" | **{completed}/{len(problem_list)}** |")
    lines += ["","## 제출 규칙","",f"- 브랜치: `{week_directory.name}/GitHub아이디`",f"- PR 제목: `[{label}] 이름 - 문제 번호`","- 제출 경로: `solutions/주차/개인폴더/문제폴더/자유파일명.py`","- 파일명은 자유이며 개인폴더와 문제폴더 위치로 제출 여부를 판정합니다.","- 다른 스터디원의 PR을 최소 1개 리뷰합니다.",""]
    if notes: lines += ["## 추가 안내","",notes,""]
    lines += ["---","","`week.yml` 또는 `solutions`의 풀이가 `main`에 반영되면 이 문서는 자동 갱신됩니다.",""]
    return "\n".join(lines)

def load(path: Path) -> dict[str,Any]:
    try: data=yaml.safe_load(path.read_text(encoding='utf-8'))
    except yaml.YAMLError as e: raise ConfigError(f"YAML 문법 오류: {e}") from e
    if not isinstance(data,dict): raise ConfigError("week.yml 최상위 값은 객체여야 합니다.")
    return data

def paths(selected: str|None) -> list[Path]:
    if selected:
        p=PROBLEMS_ROOT/selected/'week.yml'
        if not p.is_file(): raise ConfigError(f"설정 파일을 찾을 수 없습니다: {p}")
        return [p]
    return sorted(PROBLEMS_ROOT.glob('week*/week.yml'))

def main() -> int:
    parser=argparse.ArgumentParser(); parser.add_argument('--week'); parser.add_argument('--check',action='store_true'); args=parser.parse_args()
    try: config_paths=paths(args.week)
    except ConfigError as e: print(f"ERROR: {e}",file=sys.stderr); return 1
    stale=[]
    for config_path in config_paths:
        week_dir=config_path.parent; readme=week_dir/'README.md'
        try: output=render(week_dir,load(config_path))
        except ConfigError as e: print(f"ERROR [{config_path}]: {e}",file=sys.stderr); return 1
        current=readme.read_text(encoding='utf-8') if readme.is_file() else None
        if current==output: print(f"UNCHANGED: {readme.relative_to(ROOT)}"); continue
        stale.append(readme)
        if args.check: print(f"STALE: {readme.relative_to(ROOT)}")
        else: readme.write_text(output,encoding='utf-8'); print(f"UPDATED: {readme.relative_to(ROOT)}")
    if args.check and stale: return 1
    return 0

if __name__=='__main__': raise SystemExit(main())
