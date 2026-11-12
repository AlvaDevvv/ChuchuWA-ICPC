#!/usr/bin/env bash
set -u
cd "$(dirname "$0")/.."
CXX=${CXX:-g++}
CXXFLAGS=${CXXFLAGS:--std=gnu++17 -O2 -Wall -Wextra}
mkdir -p build
fail=0
n=0
for h in $(find content -name '*.h' | sort); do
  n=$((n + 1))
  {
    echo '#define main template_main_'
    echo '#include "content/contest/template.cpp"'
    echo '#undef main'
    echo "#include \"$h\""
    echo 'int main(){}'
  } >build/_compile.cpp
  if ! $CXX $CXXFLAGS -I. -fsyntax-only build/_compile.cpp 2>build/_compile.err; then
    echo "FALLA  $h"
    sed 's/^/   /' build/_compile.err | head -n 20
    fail=1
  else
    echo "ok     $h"
  fi
done
for c in $(find content -name '*.cpp' | sort); do
  n=$((n + 1))
  if ! $CXX $CXXFLAGS -fsyntax-only "$c" 2>build/_compile.err; then
    echo "FALLA  $c"
    sed 's/^/   /' build/_compile.err | head -n 20
    fail=1
  else
    echo "ok     $c"
  fi
done
echo "compile-all: $n archivos"
exit $fail
