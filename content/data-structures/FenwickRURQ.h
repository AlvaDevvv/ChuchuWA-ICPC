/**
 * Author: ChuchuWA
 * Date: 2026-11-13
 * License: CC0
 * Source: implementación propia
 * Description: Dos Fenwick trees 1-indexados. range\_update suma v en [l, r]; query(l, r) devuelve la suma de [l, r].
 * prefix(i) = B1(i) * i - B2(i). Cuidado: B1(i) * i puede desbordar ll antes que la respuesta.
 * Usage: FenwickRURQ ft(n); ft.range_update(l, r, v); ft.query(l, r);
 * Time: $O(\log N)$ por operación
 * Status: stress-tested, tested on SPOJ HORRIBLE
 */
#pragma once

struct FenwickRURQ {
	int n;
	vll b1, b2;
	FenwickRURQ(int tam) : n(tam), b1(tam + 2, 0), b2(tam + 2, 0) {}
	void add(vll& b, int pos, ll val) {
		for (; pos <= n; pos += LSOne(pos)) b[pos] += val;
	}
	ll sum(const vll& b, int i) {
		ll s = 0;
		for (; i; i -= LSOne(i)) s += b[i];
		return s;
	}
	void range_update(int l, int r, ll v) {
		add(b1, l, v), add(b1, r + 1, -v);
		add(b2, l, v * (l - 1)), add(b2, r + 1, -v * r);
	}
	ll prefix(int i) { return sum(b1, i) * i - sum(b2, i); }
	ll query(int l, int r) { return l > r ? 0 : prefix(r) - prefix(l - 1); }
};
