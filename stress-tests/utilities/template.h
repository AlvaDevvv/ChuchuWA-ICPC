#pragma once
#define main template_main_
#include "../../content/contest/template.cpp"
#undef main

mt19937 rng(12345);
int rnd(int lo, int hi) { return uniform_int_distribution<int>(lo, hi)(rng); }
