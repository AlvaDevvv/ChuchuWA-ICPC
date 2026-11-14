# Fenwick Tree (Binary Indexed Tree - BIT)

Estructura de datos ligera y extremadamente rápida para procesar consultas de sumas de prefijos dinámicas y actualizaciones puntuales.

Implementaciones del notebook: `content/data-structures/FenwickTree.h`, `FenwickRUPQ.h`, `FenwickRURQ.h` (todas **1-indexadas**).

## 📌 TL;DR (Resumen Rápido)
- **¿Qué resuelve?** Sumas de rangos con actualizaciones de un solo elemento.
- **¿Cuándo usarlo?** Cuando la memoria/tiempo es límite, como parte de algoritmos de barrido (Sweep Line), conteo de inversiones, o para buscar el k-ésimo elemento de un conjunto dinámico (`lower_bound`).
- **¿Cuándo NO usarlo?** Para consultas que no son invertibles (como RMQ, Mínimo/Máximo), a menos que solo se consulte el prefijo y los valores solo aumenten. Tampoco para **asignación** en rango (`a[l..r] = v`): eso es Segment Tree con lazy.
- **Variantes:** con un BIT sobre diferencias se obtiene **RUPQ**; con dos BITs, **RURQ** (ver al final).

## ⏱️ Complejidades
| Operación | Complejidad de Tiempo | Descripción |
| :--- | :--- | :--- |
| `update(i, v)` | $\mathcal{O}(\log m)$ | Suma `v` al elemento en la posición `i`. |
| `query(j)` | $\mathcal{O}(\log m)$ | Retorna la suma en el rango $[1 \dots j]$. |
| `query(i, j)` | $\mathcal{O}(\log m)$ | Retorna la suma en el rango $[i \dots j]$. |
| `lower_bound(k)` | $\mathcal{O}(\log m)$ | Menor `i` con `query(i) >= k` (valores no negativos). |
| Construcción | $\mathcal{O}(m \log m)$ o $\mathcal{O}(m)$ | Se puede construir en tiempo lineal. |

*Nota: $m$ es el valor máximo del índice, no necesariamente la cantidad de elementos $N$.*

| Variante | Update | Query | Memoria |
| :--- | :--- | :--- | :--- |
| BIT clásico (PURQ) | punto, $\mathcal{O}(\log m)$ | rango, $\mathcal{O}(\log m)$ | 1 BIT |
| RUPQ | rango, $\mathcal{O}(\log m)$ (2 updates) | punto, $\mathcal{O}(\log m)$ | 1 BIT |
| RURQ | rango, $\mathcal{O}(\log m)$ (4 updates) | rango, $\mathcal{O}(\log m)$ (4 consultas) | 2 BITs |

## 🔎 Patrones de reconocimiento
| Si el enunciado dice... | Pensar en |
| :--- | :--- |
| Sumas de rango + cambiar un elemento | BIT clásico |
| Sumar a un rango + consultar un elemento | RUPQ |
| Sumar a un rango + sumas de rango | RURQ (2 BITs) |
| Borrar el p-ésimo de una lista / posiciones que se desplazan | BIT de presencia + `lower_bound(p)` |
| Eliminación circular cada k (Josephus) | BIT de presencia + ranks mod m |
| Contar pares $i<j$ con una condición sobre los valores | Barrido + BIT sobre valores (comprimidos) |
| Muchas consultas de rango sin updates con "distintos" | Offline: ordenar por `r` + BIT |

## ⚠️ Checklist de Errores Comunes (¡Cuidado en Contests!)
- [ ] **Indexación 1-based:** El índice `0` causa un bucle infinito (Segment Fault / TLE). ¡Nunca llames a `update(0, v)`! (`query(0)` sí es seguro: devuelve 0.)
- [ ] **Una sola convención:** si el arreglo de entrada es 0-indexado y el BIT 1-indexado, convertir el índice **una vez** al leerlo. Antes de corregir un índice, listar cada objeto que lo usa y su convención.
- [ ] **Asignar ≠ sumar:** `a[p] = v` se hace con `update(p, v - a[p]); a[p] = v;`. Hay que guardar el arreglo original.
- [ ] **Límite de la estructura:** El bucle de `update` debe iterar hasta `m` (el valor máximo posible del índice), NO hasta `N` (el tamaño del arreglo de entrada).
- [ ] **Integer Overflow:** Si los valores son grandes, usar `long long` para el array interno y el retorno de las sumas.
- [ ] **RUPQ ≠ RURQ:** en RUPQ, `point_query(j) - point_query(i-1)` **no** es la suma del rango; da $A[j] - A[i-1]$. Para sumas de rango con updates de rango se necesitan dos BITs.
- [ ] **Compilar en local con** `-fsanitize=address,undefined -D_GLIBCXX_DEBUG`.

# Notas Profundas: Fenwick Tree

## 1. La Magia detrás de la Estructura: `LSOne(S)`
El corazón del Fenwick Tree es la operación bit a bit `LSOne`, que aísla el bit encendido menos significativo (el "1" más a la derecha) de un número.

**Fórmula:** `LSOne(S) = S & (-S)`

**¿Por qué funciona matemáticamente?**
En C++, los números negativos se representan usando el **Complemento a Dos**.
Para obtener `-S`, la computadora invierte todos los bits de `S` (operación `~S`) y le suma `1`.
- Ejemplo: $S = 10 = (01010)_2$
- `~S` = $(10101)_2$
- `-S = ~S + 1` = $(10110)_2$
- `S & -S` = $(01010)_2 \ \& \ (10110)_2 = (00010)_2 = 2$

En general, si $S = x\,1\,0\dots0$ (con $t$ ceros finales), entonces $-S = \bar{x}\,1\,0\dots0$: el `+1` se propaga por los unos de `~S` hasta el primer cero. Al hacer `&`, $x$ y $\bar{x}$ se anulan y solo sobrevive $2^t$.

## 2. Anatomía de los Nodos
A diferencia de un Segment Tree donde los rangos se dividen por la mitad exacta, en un BIT la longitud del rango que cubre la celda `ft[i]` es exactamente `LSOne(i)`.
- El rango cubierto por `ft[i]` es: `[i - LSOne(i) + 1 ... i]`
- Ejemplo para $i = 12$:
  - $12 = (1100)_2$
  - `LSOne(12) = 4`
  - `ft[12]` guarda la suma de los elementos desde $12 - 4 + 1 = 9$ hasta $12$. (Rango `[9..12]`).

```
pos     1    2    3    4    5    6    7    8
lb=8  [------------------T8------------------]
lb=4  [--------T4--------]
lb=2  [---T2---]          [---T6---]
lb=1  [ T1]     [ T3]     [ T5]     [ T7]
```

> [!warning] Bloque ≠ prefijo
> `ft[i]` es la suma de un **bloque**, no de `[1, i]`. Solo coinciden cuando `i` es potencia de 2. Ejemplo: con `A = [1,0,1,1,0,1,1,1]`, `ft[6] = A[5] + A[6] = 1`, mientras que `prefix(6) = 4`.

**Propiedad estructural:** los bloques son diádicos (empiezan después de un múltiplo de su tamaño), así que dos bloques cualesquiera son **disjuntos o uno contiene al otro**. Eso es lo que convierte a los bloques en un árbol: el padre de `i` es `i + LSOne(i)`.

`ft` no es el arreglo: es el conjunto de sumas de bloques sobre un arreglo **implícito** (valores, presencia 0/1, conteos por valor...). Ese arreglo puede no aparecer nunca en el código.

## 3. Justificación de Complejidades $\mathcal{O}(\log m)$
¿Por qué garantizamos que toma $\mathcal{O}(\log m)$?

**Para Consultas `query(j)`:**
El algoritmo resta `LSOne(j)` en cada paso: `j = j - LSOne(j)`.
Esta operación literalmente apaga el bit "1" más a la derecha de `j`. Como un número `j` (que llega hasta el límite $m$) se representa con un máximo de $\approx \log_2(m)$ bits, solo podemos apagar bits un máximo de $\log_2(m)$ veces antes de llegar a `0`. Los bloques visitados son disjuntos y cubren exactamente `[1, j]`.

**Para Actualizaciones `update(i, v)`:**
El algoritmo suma `LSOne(i)` en cada paso: `i = i + LSOne(i)`.
Al sumar `LSOne(i)` se apaga ese bit y el acarreo sube hacia la izquierda, así que el nuevo índice tiene **al menos un cero final más**: `LSOne` como mínimo se duplica en cada paso. Como `LSOne(i) ≤ i ≤ m`, solo puede duplicarse $\approx \log_2(m)$ veces antes de superar $m$. Los índices visitados son exactamente los bloques que contienen la posición `i`.

## 4. Construcción en $\mathcal{O}(m)$
Dos formas:
- **Empujar al padre** (la del notebook): recorrer `i = 1..n`, sumar `a[i]` a `ft[i]` y luego `ft[i + LSOne(i)] += ft[i]`. Funciona porque todos los hijos de `i` son menores que `i`, así que cuando se empuja `ft[i]` ya está completo.
- **Prefijos:** `ft[i] = P[i] - P[i - LSOne(i)]`, con `P` las sumas de prefijo.
- Caso especial: si el arreglo es todo unos, `ft[i] = LSOne(i)` directamente (el bloque tiene `LSOne(i)` unos).

## 5. Requisitos sobre la operación
| Lo que quieres | Requisito | Válido | Falla con |
| :--- | :--- | :--- | :--- |
| `prefix(i)` + `a[p] ⊕= x` | Asociativa + conmutativa + neutro | `+`, `xor`, `max`, `gcd` | operaciones no conmutativas |
| `query(l, r)` | Además, **inversa** | `+` (resta), `xor` (sí mismo) | `max`, `min`, `gcd` |

Con `max`, el prefix-max solo funciona si los valores **nunca bajan**.

## 6. `lower_bound(k)`: el k-ésimo en $\mathcal{O}(\log m)$
Encuentra el menor `i` con `query(i) >= k`. Requiere valores **no negativos** (prefijos monótonos). Si `k` supera la suma total devuelve `n + 1`.

```cpp
int lower_bound(ll k){
  int pos = 0;
  for(int pw = 1 << __lg(n); pw; pw >>= 1)
    if(pos + pw <= n && ft[pos + pw] < k){ k -= ft[pos + pw]; pos += pw; }
  return pos + 1;
}
```

**Idea:** caminar desde la izquierda con saltos de 8, 4, 2, 1 y detenerse **justo antes** de llegar a `k`. `pos` es el mayor índice con `prefix(pos) < k`; la respuesta es `pos + 1`.

**Por qué de mayor a menor:** `pos` solo tiene bits mayores que `pw`, así que `LSOne(pos + pw) = pw` y `ft[pos + pw]` cubre **exactamente** `(pos, pos + pw]`: cada bloque empieza donde terminó el anterior, sin llamar a `query`. En orden ascendente los bloques no se alinean (con `pos = 1, pw = 2`, `ft[3]` cubre solo `[3,3]` y la posición 2 se pierde).

**Por qué `<` y no `<=`:** si un bloque tiene exactamente los que faltan, saltarlo dejaría `pos` **sobre** la respuesta, no antes.

Traza con `A = [1,0,0,1,0,1,1,1]`, `ft = [1,1,0,2,0,1,1,5]`, `k = 4`:

```
pw  pos+pw  bloque  ft   ¿<k?  pos  k
 8    8     [1,8]   5    no     0   4
 4    4     [1,4]   2    sí     4   2
 2    6     [5,6]   1    sí     6   1
 1    7     [7,7]   1    no     6   1
return 7   (4.º uno de A)
```

## 7. Aplicación: BIT de presencia (List Removals, Josephus)
- No mover elementos: dejar cada uno en su índice original y mantener un BIT con `1` si sigue vivo.
- `query(i)` = posición actual del índice `i`; el elemento en la posición `p` es `lower_bound(p)`; borrar es `update(idx, -1)`.
- **Josephus (saltar k, eliminar el siguiente):** con `m` vivos y `cur` el rank (desde 0) donde empieza el conteo, sale `r = (cur + k) % m` → `idx = lower_bound(r + 1)`. Tras borrar, el siguiente **hereda** el rank: `cur = r % m` (con `m` ya decrementado). Error típico: `cur = r + 1`.

## 8. Aplicación: Conteo de Inversiones

- **Idea clave:** Usar el BIT como tabla de frecuencias. Los índices del BIT representan los VALORES del arreglo (comprimir coordenadas si son grandes o negativos).
- **Dirección de barrido:** Recorrer de derecha a izquierda.
- **Fórmula:** 
  1. `inversiones_actuales = bit.query(A[i] - 1)` (elementos a la derecha que son menores)
  2. `bit.update(A[i], 1)`
- **Equivalente de izquierda a derecha:** `inv_j = (j - 1) - bit.query(A[j])` (todos los anteriores menos los que no son mayores).
- **Cuidado:** El resultado total puede superar $2 \times 10^9$ (llega a $n(n-1)/2$), usar siempre `long long`.

**Compresión de coordenadas:**
```cpp
vi vals = a;
sort(all(vals));
vals.erase(unique(all(vals)), vals.end());
auto id = [&](int v){ return int(std::lower_bound(all(vals), v) - vals.begin()) + 1; };
```

## Mejoras: RUPQ y RURQ
- Repensar el arreglo BIT: en vez de guardar sumas acumuladas de bloques de $A$, almacena sumas de bloques de las **diferencias** de $A$.
- **Range Update, Point Query (RUPQ):** actualizar un rango y retornar el valor exacto en la posición `i`.
- **Range Update, Range Query (RURQ):** actualizar un rango y retornar la suma de `[a...b]`.

### 9. Arreglo de diferencias

$$D[i] = A[i] - A[i-1], \qquad A[0] = 0$$

Cualquier valor se reconstruye como suma de prefijo de las diferencias:

$$A[j] = D[1] + D[2] + \dots + D[j]$$

**Update de rango → solo 2 cambios en $D$.** Sumar $v$ a $A[l..r]$:

| posición $k$ | ¿$A[k]$ sube? | ¿$A[k-1]$ sube? | cambio en $D[k]$ |
| :--- | :--- | :--- | :--- |
| $k < l$ | no | no | $0$ |
| $k = l$ | **sí** | no | **$+v$** |
| $l < k \le r$ | sí | sí | $0$ (suben juntos, la resta no cambia) |
| $k = r+1$ | no | **sí** | **$-v$** |
| $k > r+1$ | no | no | $0$ |

```
A después: 0  3  3  3  0  0     (update [2,4] con v = 3)
D después: 0 +3  0  0 -3  0
              ↑        ↑
              l       r+1
```

### 10. RUPQ (un BIT)

Un BIT sobre $D$:
- `range_update(l, r, v)` → `update(l, v)` y `update(r+1, -v)`.
- `point_query(i)` → `query(i)` $= \sum_{k \le i} D[k] = A[i]$.

Notebook: `FenwickRUPQ.h` (incluye además construcción $\mathcal{O}(n)$ desde `A` 1-indexado).

```cpp
struct FenwickRUPQ{
  int n; vll bit;
  FenwickRUPQ(int tam) : n(tam), bit(tam + 2, 0) {}

  void update(int pos, ll val){
    for(; pos <= n; pos += LSOne(pos)) bit[pos] += val;
  }

  void range_update(int l, int r, ll v){
    update(l, v); update(r + 1, -v);
  }

  ll point_query(int i){
    ll s = 0;
    for(; i; i -= LSOne(i)) s += bit[i];
    return s;
  }
};
```

> [!warning] Error típico
> `point_query(j) - point_query(i-1)` da $A[j] - A[i-1]$, **no** $\sum A[i..j]$. Para eso hace falta RURQ.

La misma idea funciona con `xor` en vez de suma (`a[l..r] ^= x` + consultar `a[p]`): `x ^ x = 0` cancela el efecto en `r+1`, y la consulta es un **prefix-xor**.

### 11. RURQ: de dónde sale la fórmula

**Objetivo:** calcular $\text{prefix}(i) = A[1] + A[2] + \dots + A[i]$ usando solo sumas de prefijo de arreglos que se puedan mantener con updates puntuales.

**Paso 1: expandir cada $A[j]$ en diferencias.** Con $i = 4$:

```
A[1] = D1
A[2] = D1 + D2
A[3] = D1 + D2 + D3
A[4] = D1 + D2 + D3 + D4
──────────────────────────────
suma = 4·D1 + 3·D2 + 2·D3 + 1·D4
```

Por columnas, $D[k]$ aparece en las filas $k, k+1, \dots, i$, o sea **$i - k + 1$ veces**:

$$\text{prefix}(i) = \sum_{k=1}^{i} D[k]\,(i - k + 1)$$

**Paso 2: separar $i$ de $k$.** No se puede guardar $D[k](i-k+1)$ porque $i$ recién se conoce en la consulta. Se parte el coeficiente en una parte que solo depende de $i$ y otra que solo depende de $k$:

$$i - k + 1 = i - (k - 1)$$

$$\text{prefix}(i) = i \cdot \underbrace{\sum_{k=1}^{i} D[k]}_{\text{BIT } B_1} \;-\; \underbrace{\sum_{k=1}^{i} D[k]\,(k-1)}_{\text{BIT } B_2}$$

Con el ejemplo: $4D_1 + 3D_2 + 2D_3 + D_4 = 4(D_1+D_2+D_3+D_4) - (0D_1 + 1D_2 + 2D_3 + 3D_4)$.

- $B_1[k] = D[k]$ → aporta la **pendiente** (cuánto suma cada posición).
- $B_2[k] = D[k]\cdot(k-1)$ → aporta la **corrección** (lo que $i \cdot B_1$ contó de más en las posiciones antes de que empezara el rango).

> [!tip] Idea clave
> Después de updates de rango, $\text{prefix}(i)$ es una recta por tramos $m\cdot i - c$. $B_1$ guarda $m$, $B_2$ guarda $c$. El factor $(k-1)$ depende de la **posición**, no de la consulta, por eso se puede guardar ya multiplicado desde el update.

### 12. RURQ: de dónde salen los 4 updates

Un `range_update(l, r, v)` solo cambia $D[l] \mathrel{+}= v$ y $D[r+1] \mathrel{-}= v$.

Como $(k-1)$ es un número fijo de cada posición, si $D[k]$ aumenta en $\Delta$:

$$(D[k] + \Delta)(k-1) = D[k](k-1) + \Delta(k-1)$$

o sea **$B_2[k]$ aumenta en $\Delta \cdot (k-1)$**. Aplicándolo a las dos posiciones:

| cambio en $D$ | en $B_1$ | en $B_2$ |
| :--- | :--- | :--- |
| $D[l] \mathrel{+}= v$ | $B_1[l] \mathrel{+}= v$ | $B_2[l] \mathrel{+}= v(l-1)$ |
| $D[r+1] \mathrel{-}= v$ | $B_1[r+1] \mathrel{-}= v$ | $B_2[r+1] \mathrel{-}= v\big((r+1)-1\big) = v\,r$ |

### 13. RURQ: verificación por casos (un solo update)

| rango de $i$ | $\sum B_1$ | $\sum B_2$ | $i\sum B_1 - \sum B_2$ | real |
| :--- | :--- | :--- | :--- | :--- |
| $i < l$ | $0$ | $0$ | $0$ | $0$ ✓ |
| $l \le i \le r$ | $v$ | $v(l-1)$ | $v(i-l+1)$ | $v(i-l+1)$ ✓ |
| $i > r$ | $0$ | $v(l-1) - vr$ | $v(r-l+1)$ | $v(r-l+1)$ ✓ |

Todo es lineal → con varios updates los efectos simplemente se suman.

**Ejemplo** ($n=6$): update $[2,4]$ con $v=3$, luego $[3,6]$ con $v=2$ (aquí $r+1=7>n$, se ignora).

```
k     :  1  2  3  4  5   6
k−1   :  0  1  2  3  4   5
A     :  0  3  5  5  2   2
B1    :  0  3  2  0 -3   0
B2    :  0  3  4  0 -12  0
```

$\text{prefix}(5) = 5\cdot(0+3+2+0-3) - (0+3+4+0-12) = 10 + 5 = 15 = 0+3+5+5+2$ ✓

### 14. RURQ: código

Versión directa (para entender, $\mathcal{O}(n)$ por consulta):

```cpp
struct Naive{
  int n; vll D1, D2;
  Naive(int tam) : n(tam), D1(tam + 2, 0), D2(tam + 2, 0) {}

  void range_update(int l, int r, ll v){
    D1[l] += v;           D1[r + 1] -= v;
    D2[l] += v * (l - 1); D2[r + 1] -= v * r;
  }

  ll prefix(int i){
    ll s1 = 0, s2 = 0;
    FOR(k, 1, i){ s1 += D1[k]; s2 += D2[k]; }
    return s1 * i - s2;
  }

  ll query(int l, int r){ return prefix(r) - prefix(l - 1); }
};
```

Solo se hacen **updates puntuales** y **sumas de prefijo** sobre `D1` y `D2` → exactamente lo que da un BIT. Se reemplaza `D[pos] += val` por `add` y el bucle por `sum` (notebook: `FenwickRURQ.h`):

```cpp
struct FenwickRURQ{
  int n; vll b1, b2;
  FenwickRURQ(int tam) : n(tam), b1(tam + 2, 0), b2(tam + 2, 0) {}

  void add(vll &b, int pos, ll val){
    for(; pos <= n; pos += LSOne(pos)) b[pos] += val;
  }

  ll sum(const vll &b, int i){
    ll s = 0;
    for(; i; i -= LSOne(i)) s += b[i];
    return s;
  }

  void range_update(int l, int r, ll v){
    add(b1, l, v);           add(b1, r + 1, -v);
    add(b2, l, v * (l - 1)); add(b2, r + 1, -v * r);
  }

  ll prefix(int i){ return sum(b1, i) * i - sum(b2, i); }

  ll query(int l, int r){ return l > r ? 0 : prefix(r) - prefix(l - 1); }
};
```

| fórmula | versión directa | versión BIT |
| :--- | :--- | :--- |
| $B_1[l] \mathrel{+}= v$ | `D1[l] += v` | `add(b1, l, v)` |
| $B_2[l] \mathrel{+}= v(l-1)$ | `D2[l] += v*(l-1)` | `add(b2, l, v*(l-1))` |
| $\sum B_1[1..i]$ | bucle `s1` | `sum(b1, i)` |
| $\sum B_2[1..i]$ | bucle `s2` | `sum(b2, i)` |
| $i\sum B_1 - \sum B_2$ | `s1*i - s2` | `sum(b1,i)*i - sum(b2,i)` |

### ⚠️ Checklist RURQ
- [ ] **No mezclar particiones:** también vale $i-k+1 = (i+1) - k$ → $\text{prefix}(i) = (i+1)\sum D - \sum D\cdot k$, con updates $v\,l$ y $-v(r+1)$. Elegir **una** y memorizarla tal cual.
- [ ] **Cuando $r = n$:** `add(b, n+1, ...)` no hace nada porque el `for` corta con `pos <= n`; no hace falta `if`.
- [ ] **Overflow:** `v * r` y `sum(b1, i) * i` deben ser `ll`. Ojo: `sum(b1, i) * i` puede desbordar **antes** que la respuesta final (con $n, q \le 2\cdot10^5$ y $v \le 10^9$, la suma llega a $\sim 4\cdot10^{19}$). Acotar antes de codear; si no cabe, usar `i128` en `prefix` o trabajar módulo.

## 📚 Problemas de práctica
| Problema | Técnica |
| :--- | :--- |
| CSES 1648 — Dynamic Range Sum Queries | BIT clásico (asignación como delta) |
| Library Checker — Point Add Range Sum | BIT clásico |
| CSES 1651 — Range Update Queries | RUPQ |
| SPOJ HORRIBLE — Horrible Queries | RURQ (multitest) |
| CSES 1749 — List Removals | BIT de presencia + `lower_bound` |
| CSES 2163 — Josephus Problem II | BIT de presencia + ranks mod m |
| SPOJ INVCNT — Inversion Count | Inversiones |
| CSES 1734 — Distinct Values Queries | Offline por `r` + BIT |
| CSES 1144 — Salary Queries | Compresión + BIT |
