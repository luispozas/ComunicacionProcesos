#!/bin/bash
MPOINT="./mount-point"
rm -rf tmp
mkdir tmp
cp src/fuseLib.c tmp/
cp src/fuseLib.c $MPOINT/
echo "Copiando fuseLib.c"

cp src/myFS.h tmp/
cp src/myFS.h $MPOINT/
echo "Copiando myFS.h"

./my-fsck-static-64 virtual-disk
echo "Auditando disco"

(diff src/fuseLib.c $MPOINT/fuseLib.c)&&(diff src/myFS.h $MPOINT/myFS.h)
echo "Comparando diferencia entre archivo original y copiado"

truncate -s 16384 tmp/fuseLib.c
truncate -s 16384 $MPOINT/fuseLib.c
echo "Truncando archivos para que ocupen un bloque menos"

./my-fsck-static-64 virtual-disk
echo "Auditando disco"

(diff src/fuseLib.c $MPOINT/fuseLib.c) && (diff src/fuseLib.c tmp/fuseLib.c)
echo "Comparando diferencia entre archivo orignal y truncado"

cp src/myFS.c $MPOINT/myFS.c
echo "Copiando tercer archivo myFS.c"

./my-fsck-static-64 virtual-disk
echo "Auditando disco"

diff src/myFS.c $MPOINT/myFS.c
echo "Comparando diferencia entre archivo orignal y copiado"

truncate -s 12288 tmp/myFS.h
truncate -s 12288 $MPOINT/myFS.h
echo "Truncando myFS.h para que ocupe 2 bloques más"

./my-fsck-static-64 virtual-disk
echo "Auditanto disco"

(diff src/myFS.h $MPOINT/myFS.h) && (diff src/myFS.h tmp/myFS.h)
echo "Haciendo diff entre el fichero original myFS.h y el fichero truncado en el FS y la carpeta temp"


