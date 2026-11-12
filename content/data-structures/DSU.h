/**
 * Author: ChuchuWA
 * Date: 2026-11-12
 * License: CC0
 * Source: implementación propia
 * Description: Disjoint Set Union con union by size y path compression. join devuelve false si ya estaban unidos.
 * Time: $O(\alpha(N))$ amortizado
 * Status: stress-tested
 */
#pragma once

struct DSU {
	vi p;
	DSU(int n) : p(n, -1) {}
	int find(int x) { return p[x] < 0 ? x : p[x] = find(p[x]); }
	bool same(int a, int b) { return find(a) == find(b); }
	int size(int x) { return -p[find(x)]; }
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (p[a] > p[b]) swap(a, b);
		p[a] += p[b];
		p[b] = a;
		return true;
	}
};
