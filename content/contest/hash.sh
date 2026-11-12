# Hash de un archivo para comparar con el notebook: bash hash.sh < archivo.h
tr -d '[:space:]' | md5sum | cut -c-6
