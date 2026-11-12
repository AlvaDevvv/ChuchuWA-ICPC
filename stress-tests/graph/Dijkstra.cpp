#include "stress-tests/utilities/template.h"
#include "content/graph/Dijkstra.h"

int main() {
	rep(it, 0, 500) {
		int n = rnd(1, 12), m = rnd(0, 40);
		vector<vector<pair<int, ll>>> g(n);
		const ll INF = LLONG_MAX / 4;
		vector<vector<ll>> fw(n, vector<ll>(n, INF));
		rep(i, 0, n) fw[i][i] = 0;
		rep(e, 0, m) {
			int u = rnd(0, n - 1), v = rnd(0, n - 1);
			ll w = rnd(0, 20);
			g[u].push_back({v, w});
			fw[u][v] = min(fw[u][v], w);
		}
		rep(k, 0, n) rep(i, 0, n) rep(j, 0, n) fw[i][j] = min(fw[i][j], fw[i][k] + fw[k][j]);
		int s = rnd(0, n - 1);
		auto d = dijkstra(g, s);
		rep(v, 0, n) assert(d[v] == (fw[s][v] >= INF ? LLONG_MAX : fw[s][v]));
	}
	cout << "Dijkstra ok\n";
}
