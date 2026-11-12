/**
 * Author: ChuchuWA
 * Date: 2026-11-12
 * License: CC0
 * Source: implementación propia
 * Description: Aritmética modular con MOD primo. inv usa el pequeño teorema de Fermat.
 * Time: $O(1)$ por operación, $O(\log MOD)$ para pow e inv
 * Status: stress-tested
 */
#pragma once

const ll MOD = 1e9 + 7;
struct Mint {
	ll x;
	Mint(ll v = 0) : x(v % MOD) { if (x < 0) x += MOD; }
	Mint operator+(Mint o) const { return x + o.x; }
	Mint operator-(Mint o) const { return x - o.x; }
	Mint operator*(Mint o) const { return x * o.x; }
	Mint pow(ll e) const {
		Mint r = 1, b = *this;
		for (; e; e >>= 1, b = b * b)
			if (e & 1) r = r * b;
		return r;
	}
	Mint inv() const { return pow(MOD - 2); }
	Mint operator/(Mint o) const { return *this * o.inv(); }
	bool operator==(Mint o) const { return x == o.x; }
};
