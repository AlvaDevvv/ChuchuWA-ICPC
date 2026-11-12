#include "stress-tests/utilities/template.h"
#include "content/number-theory/ModInt.h"

int main() {
	rep(it, 0, 100000) {
		ll a = rnd(-1000000000, 1000000000), b = rnd(1, 1000000000);
		Mint x(a), y(b);
		assert((x * y / y).x == x.x);
		assert(((x + y) - y).x == x.x);
		assert((y * y.inv()).x == 1);
		assert(x.x >= 0 && x.x < MOD);
	}
	assert(Mint(2).pow(10).x == 1024);
	cout << "ModInt ok\n";
}
