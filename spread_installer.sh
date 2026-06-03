#!/bin/bash
echo "1. Instalando herramientas necesarias..."
sudo apt-get update -y -qq
sudo apt-get install -y flex bison wget build-essential -qq

echo "2. Descargando Spread Toolkit en silencio..."
wget -q https://www.spread.org/direct_download/spread-src-5.0.1.tar.gz

echo "3. Descomprimiendo..."
tar -xzf spread-src-5.0.1.tar.gz
cd spread-src-5.0.1

echo "4. Compilando ..."
./configure CFLAGS="-fcommon" --quiet
make -s

echo "5. Instalando en el sistema..."
sudo make install > /dev/null 2>&1
sudo ldconfig

echo "6. Limpiando archivos temporales..."
cd ..
rm -rf spread-src-5.0.1 spread-src-5.0.1.tar.gz

echo "¡Listo! Spread está instalado."
