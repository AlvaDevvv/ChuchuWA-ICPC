#pragma once
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

#define rep(i, a, b) for (int i = (a); i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

mt19937 rng(12345);
int rnd(int lo, int hi) { return uniform_int_distribution<int>(lo, hi)(rng); }
