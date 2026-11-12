#!/usr/bin/env bash
set -u
cd "$(dirname "$0")/.."
CXX=${CXX:-g++}
CXXFLAGS=${CXXFLAGS:--std=gnu++17 -O2 -Wall -Wextra}
mkdir -p build
fail=0
n=0
for h in $(find content -name '*.h' | sort); do
  n=$((n+1))
  printf '#include <bits/stdc++.h>\nusing namespace std;\n#define rep(i,a,b) for(int i=(a);i<(b);++i)\n#define all(x) begin(x),end(x)\n#define sz(x) (int)(x).size()\ntypedef long long ll;\ntypedef pair<int,int> pii;\ntypedef vector<int> vi;\n#include "%s"\nint main(){}\n' "$PWD/$h" > build/_compile.cpp
  if ! $CXX $CXXFLAGS -fsyntax-only build/_compile.cpp 2> build/_compile.err; then
    echo "FALLA  $h"
    sed 's/^/   /' build/_compile.err | head -n 20
    fail=1
  else
    echo "ok     $h"
  fi
done
for c in $(find content -name '*.cpp' | sort); do
  n=$((n+1))
  if ! $CXX $CXXFLAGS -fsyntax-only "$c" 2> build/_compile.err; then
    echo "FALLA  $c"; sed 's/^/   /' build/_compile.err | head -n 20; fail=1
  else
    echo "ok     $c"
  fi
done
echo "compile-all: $n archivos"
exit $fail
