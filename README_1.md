# ChuchuWA – UTEC · Notebook de programación competitiva

Estructura inspirada en KACTL, con implementaciones propias.

## Estructura

```
content/
  notebook.tex            documento principal (orden de capítulos, nombre del equipo)
  <tema>/chapter.tex      título del capítulo + lista \kactlimport{Archivo.h}
  <tema>/*.h              un algoritmo por archivo, con encabezado
stress-tests/<tema>/*.cpp un test por algoritmo (compara contra fuerza bruta)
scripts/                  build.py, compile-all.sh, stress.sh
```

## Comandos

| Comando        | Qué hace                                                    |
|----------------|-------------------------------------------------------------|
| `make compile` | compila cada `.h` con el template (detecta errores rápido)  |
| `make test`    | compile + corre todos los stress tests                      |
| `make pdf`     | genera `notebook.pdf`                                       |
| `make`         | test + pdf                                                  |

Requisitos (Ubuntu/WSL): `sudo apt install g++ make python3 texlive-latex-extra texlive-fonts-recommended lmodern`

## Agregar un algoritmo

1. Crear `content/<tema>/Nombre.h` con este encabezado (Description y Status son obligatorios):

```cpp
/**
 * Author: ChuchuWA
 * Date: AAAA-MM-DD
 * License: CC0
 * Source: de dónde salió la idea
 * Description: qué hace y cómo se usa
 * Usage: (opcional) ejemplo corto
 * Time: $O(N \log N)$
 * Status: stress-tested | tested on CF XXXX | untested
 */
#pragma once
```

2. Agregar `\kactlimport{Nombre.h}` en `content/<tema>/chapter.tex`.
3. Crear `stress-tests/<tema>/Nombre.cpp` que incluya
   `stress-tests/utilities/template.h` y `content/<tema>/Nombre.h`, compare contra una fuerza bruta y use `assert`.
4. `make` y abrir PR.

Los `.h` asumen el template de `content/contest/template.cpp` (`rep`, `all`, `sz`, `ll`, `pii`, `vi`) y no incluyen headers.

## Tema nuevo

1. Crear `content/<tema>/chapter.tex` con `\kactlchapter{Título}`.
2. Agregar `\kactlinclude{<tema>}` en `content/notebook.tex`.

## Hash

Cada archivo muestra un hash de 6 caracteres en el PDF. En concurso: `bash hash.sh < archivo.h` (sin el encabezado ni `#pragma once`) debe dar el mismo valor.
