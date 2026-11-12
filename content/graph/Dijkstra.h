/**
 * Author: ChuchuWA
 * Date: 2026-11-12
 * License: CC0
 * Source: implementación propia
 * Description: Distancias mínimas desde s con pesos no negativos. g[u] = lista de (v, w). Inalcanzable = LLONG_MAX.
 * Time: $O((V + E) \log V)$
 * Status: stress-tested
 */
#pragma once

vector<ll> dijkstra(const vector<vector<pair<int, ll>>>& g, int s) {
	vector<ll> d(sz(g), LLONG_MAX);
	priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
	pq.push({d[s] = 0, s});
	while (!pq.empty()) {
		auto [du, u] = pq.top();
		pq.pop();
		if (du != d[u]) continue;
		for (auto [v, w] : g[u])
			if (du + w < d[v]) pq.push({d[v] = du + w, v});
	}
	return d;
}
