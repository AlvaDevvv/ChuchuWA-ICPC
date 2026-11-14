/**
 * Author: ChuchuWA
 * Date: 2026-11-13
 * License: CC0
 * Source: implementación propia
 * Description: Fenwick tree 1-indexado sobre el arreglo de diferencias. range\_update suma v en [i, j]; point\_query(i) devuelve a[i].
 * El constructor recibe A 1-indexado (A[0] se ignora).
 * Usage: FenwickRUPQ ft(A); ft.range_update(l, r, v); ft.point_query(i);
 * Time: $O(\log N)$ por operación, construcción $O(N)$
 * Status: stress-tested, tested on CSES 1651
 */
#pragma once

struct FenwickRUPQ {
	int n;
	vll bit;
	FenwickRUPQ(int tam) : n(tam), bit(tam + 2, 0) {}
	FenwickRUPQ(const vll& A) : n(sz(A) - 1), bit(n + 2, 0) {
		for (int i = 1; i <= n; ++i) {
			bit[i] += A[i] - A[i - 1];
			if (i + LSOne(i) <= n) bit[i + LSOne(i)] += bit[i];
		}
	}
	void update(int i, ll v) {
		for (; i <= n; i += LSOne(i)) bit[i] += v;
	}
	ll point_query(int i) {
		ll s = 0;
		for (; i; i -= LSOne(i)) s += bit[i];
		return s;
	}
	void range_update(int i, int j, ll v) {
		update(i, v);
		update(j + 1, -v);
	}
};
