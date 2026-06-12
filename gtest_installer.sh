#!/bin/bash

# Salir inmediatamente si algún comando falla
set -e

echo "=== 1. Actualizando el sistema e instalando dependencias ==="
sudo apt update
sudo apt install -y build-essential cmake libgtest-dev

echo "=== 2. Creando directorio temporal para la compilación ==="
# Creamos una carpeta en /tmp para no ensuciar el sistema
mkdir -p /tmp/gtest_build
cd /tmp/gtest_build

echo "=== 3. Configurando y compilando GTest con CMake ==="
# Usamos las fuentes de /usr/src/gtest que instaló 'libgtest-dev'
cmake /usr/src/gtest
make

echo "=== 4. Instalando las librerías en el sistema ==="
# Copia los archivos .a y .so a las rutas globales de Linux
sudo cp lib/*.a /usr/local/lib/
sudo cp -r /usr/src/gtest/include/gtest /usr/local/include/

echo "=== 5. Limpiando archivos temporales ==="
rm -rf /tmp/gtest_build

echo "================================================="
echo " 🎉 ¡Google Test se ha instalado correctamente! "
echo "================================================="
