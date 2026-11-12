CXX ?= g++
CXXFLAGS ?= -std=gnu++17 -O2 -Wall -Wextra -Wno-unused-function -Wno-unused-const-variable

.PHONY: all pdf compile test clean

all: test pdf

pdf:
	python3 scripts/build.py
	pdflatex -interaction=nonstopmode -halt-on-error -output-directory=build content/notebook.tex > build/latex.log || (tail -n 40 build/latex.log; exit 1)
	pdflatex -interaction=nonstopmode -halt-on-error -output-directory=build content/notebook.tex > build/latex.log || (tail -n 40 build/latex.log; exit 1)
	cp build/notebook.pdf notebook.pdf
	@echo "OK -> notebook.pdf"

compile:
	CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" bash scripts/compile-all.sh

test: compile
	CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" bash scripts/stress.sh

clean:
	rm -rf build notebook.pdf
