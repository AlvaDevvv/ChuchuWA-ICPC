/**
 * Author: ChuchuWA
 * Date: 2026-11-12
 * License: CC0
 * Source: implementación propia
 * Description: pi[i] = longitud del mayor borde propio de s[0..i]. match devuelve las posiciones donde empieza p en s.
 * Time: $O(N + M)$
 * Status: stress-tested
 */
#pragma once

vi prefixFunction(const string& s) {
	vi pi(sz(s));
	rep(i, 1, sz(s)) {
		int k = pi[i - 1];
		while (k && s[i] != s[k]) k = pi[k - 1];
		pi[i] = k + (s[i] == s[k]);
	}
	return pi;
}

vi match(const string& s, const string& p) {
	vi pi = prefixFunction(p + '\0' + s), res;
	rep(i, sz(p) + 1, sz(pi))
		if (pi[i] == sz(p)) res.push_back(i - 2 * sz(p));
	return res;
}
