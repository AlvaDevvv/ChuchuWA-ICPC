/**
 * Author: ChuchuWA
 * Date: 2026-11-12
 * License: CC0
 * Source: implementación propia
 * Description: Segment tree iterativo. Actualización puntual y consulta en [l, r). op debe ser asociativa con neutro e.
 * Usage: SegTree st(n); st.update(i, v); st.query(l, r);
 * Time: $O(\log N)$ por operación
 * Status: stress-tested
 */
#pragma once

struct SegTree {
	typedef ll T;
	static constexpr T e = LLONG_MIN;
	T op(T a, T b) { return max(a, b); }
	int n;
	vector<T> t;
	SegTree(int n = 0) : n(n), t(2 * n, e) {}
	void update(int i, T v) {
		for (t[i += n] = v; i >>= 1;) t[i] = op(t[2 * i], t[2 * i + 1]);
	}
	T query(int l, int r) {
		T a = e, b = e;
		for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
			if (l & 1) a = op(a, t[l++]);
			if (r & 1) b = op(t[--r], b);
		}
		return op(a, b);
	}
};
