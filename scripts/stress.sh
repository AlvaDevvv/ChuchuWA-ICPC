#!/usr/bin/env bash
set -u
cd "$(dirname "$0")/.."
CXX=${CXX:-g++}
CXXFLAGS=${CXXFLAGS:--std=gnu++17 -O2 -Wall -Wextra}
mkdir -p build/stress
fail=0
for t in $(find stress-tests -name '*.cpp' | sort); do
  bin=build/stress/$(echo "$t" | tr '/' '_' | sed 's/\.cpp$//')
  if ! $CXX $CXXFLAGS -I. "$t" -o "$bin" 2> build/_stress.err; then
    echo "FALLA (compila) $t"; sed 's/^/   /' build/_stress.err | head -n 20; fail=1; continue
  fi
  if timeout 60 "$bin" > build/_stress.out 2>&1; then
    echo "ok     $t"
  else
    echo "FALLA  $t"; sed 's/^/   /' build/_stress.out | tail -n 10; fail=1
  fi
done
exit $fail
