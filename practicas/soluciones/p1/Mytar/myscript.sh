#!/bin/sh

# miro si esta mytar
test -x mytar || { echo 'mytar no existe o no es ejecutable'; exit 1; }

# creo dir tmp
rm -rf tmp
mkdir tmp && cd tmp

# creo ficheros

echo 'Hello world 1' > f1.txt
echo 'Hello world 22' > f2.txt
echo 'Hello world 333' > f3.txt

# a continuacion comprimo
./../mytar -cf filetar.mtar *

#creo dir out
mkdir out && cp filetar.mtar out

# a continuacion descomprimo
cd out && ./../../mytar -xf filetar.mtar

# compruebo
diff -x out -x *.mtar -r ../ ./ && cd ../.. && echo 'Correct' && exit 0
cd ../.. && echo "Error" && exit 1


