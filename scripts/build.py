import hashlib
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
CONTENT = ROOT / "content"
BUILD = ROOT / "build"
FIELDS = ["Author", "Date", "License", "Source", "Description", "Usage", "Time", "Status"]


def tex_escape(s):
    rep = {"\\": r"\textbackslash{}", "&": r"\&", "%": r"\%", "#": r"\#", "_": r"\_", "{": r"\{", "}": r"\}", "~": r"\textasciitilde{}", "^": r"\textasciicircum{}"}
    out = []
    for part in re.split(r"(\$[^$]*\$)", s):
        if part.startswith("$") and part.endswith("$") and len(part) > 1:
            out.append(part)
        else:
            out.append("".join(rep.get(c, c) for c in part))
    return "".join(out)


def parse(path):
    text = path.read_text(encoding="utf-8")
    meta = {}
    code = text
    m = re.match(r"\s*/\*\*(.*?)\*/\s*\n", text, re.S)
    if m:
        cur = None
        for raw in m.group(1).splitlines():
            line = re.sub(r"^\s*\*\s?", "", raw).rstrip()
            fm = re.match(r"([A-Za-z]+):\s*(.*)", line)
            if fm and fm.group(1) in FIELDS:
                cur = fm.group(1)
                meta[cur] = fm.group(2)
            elif cur and line:
                meta[cur] += " " + line.strip()
        code = text[m.end():]
    lines = [l for l in code.splitlines() if l.strip() != "#pragma once"]
    while lines and not lines[0].strip():
        lines.pop(0)
    while lines and not lines[-1].strip():
        lines.pop()
    code = "\n".join(lines)
    return meta, code


def code_hash(code):
    return hashlib.md5(re.sub(r"\s", "", code).encode()).hexdigest()[:6]


def render(path, rel):
    errors = []
    if path.suffix == ".h":
        meta, code = parse(path)
        for f in ("Description", "Status"):
            if f not in meta:
                errors.append(f"{rel}: falta el campo '{f}' en el encabezado")
    else:
        meta, code = {}, path.read_text(encoding="utf-8").rstrip()
    lang = "bash" if path.suffix in (".sh", ".bashrc") else ("" if path.suffix == ".txt" else "C++")
    out = [r"\needspace{6\baselineskip}", r"\kactlsection{%s}{%s}" % (tex_escape(path.stem), code_hash(code))]
    if meta.get("Description"):
        out.append(r"\kactldesc{%s}" % tex_escape(meta["Description"]))
    if meta.get("Usage"):
        out.append(r"\kactlusage{%s}" % tex_escape(meta["Usage"]))
    if meta.get("Time"):
        out.append(r"\kactltime{%s}" % tex_escape(meta["Time"]))
    opt = f"[language={lang}]" if lang else "[language={}]"
    out.append(r"\begin{lstlisting}" + opt)
    out.append(code)
    out.append(r"\end{lstlisting}")
    return "\n".join(out) + "\n", errors


def main():
    BUILD.mkdir(exist_ok=True)
    errors = []
    imported = 0
    for chapter in sorted(CONTENT.glob("*/chapter.tex")):
        for m in re.finditer(r"^[^%\n]*\\kactlimport\{([^}]+)\}", chapter.read_text(encoding="utf-8"), re.M):
            src = chapter.parent / m.group(1)
            rel = src.relative_to(ROOT)
            if not src.exists():
                errors.append(f"{rel}: listado en {chapter.relative_to(ROOT)} pero no existe")
                continue
            tex, errs = render(src, rel)
            errors += errs
            dst = BUILD / chapter.parent.name / (src.name + ".tex")
            dst.parent.mkdir(parents=True, exist_ok=True)
            dst.write_text(tex, encoding="utf-8")
            imported += 1
    for e in errors:
        print("ERROR:", e, file=sys.stderr)
    if errors:
        sys.exit(1)
    print(f"build.py: {imported} archivos procesados")


if __name__ == "__main__":
    main()
