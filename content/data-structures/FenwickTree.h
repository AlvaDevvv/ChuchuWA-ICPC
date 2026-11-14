/**
 * Author: ChuchuWA
 * Date: 2026-11-13
 * License: CC0
 * Source: implementación propia
 * Description: Fenwick tree (BIT) 1-indexado. update suma val en pos; query(i) es la suma de [1, i]; query(l, r) la de [l, r].
 * lower\_bound(k) devuelve el menor i con query(i) >= k (n + 1 si no existe); requiere valores no negativos.
 * Usage: FenwickTree ft(a); ft.update(i, v); ft.query(l, r); ft.lower_bound(k);
 * Time: $O(\log N)$ por operación, construcción $O(N)$
 * Status: stress-tested, tested on CSES 1648, CSES 1749
 */
#pragma once

struct FenwickTree {
	int n;
	vll ft;
	FenwickTree(int tam) : n(tam), ft(tam + 1, 0) {}
	FenwickTree(const vll& a) : n(sz(a)), ft(n + 1, 0) {
		for (int i = 1; i <= n; ++i) {
			ft[i] += a[i - 1];
			if (i + LSOne(i) <= n) ft[i + LSOne(i)] += ft[i];
		}
	}
	void update(int pos, ll val) {
		for (; pos <= n; pos += LSOne(pos)) ft[pos] += val;
	}
	ll query(int i) {
		ll s = 0;
		for (; i; i -= LSOne(i)) s += ft[i];
		return s;
	}
	ll query(int l, int r) { return l > r ? 0 : query(r) - query(l - 1); }
	int lower_bound(ll k) {
		int pos = 0;
		for (int pw = 1 << __lg(n); pw; pw >>= 1)
			if (pos + pw <= n && ft[pos + pw] < k) { k -= ft[pos + pw]; pos += pw; }
		return pos + 1;
	}
};
