#!/bin/bash
echo "Actualizando repositorios..."
sudo apt-get update

echo "Instalando herramientas de compilación base (G++, CMake, GDB)..."
sudo apt-get install -y build-essential cmake gdb git

echo "Instalando SQLite..."
sudo apt-get install -y libsqlite3-dev sqlite3

echo "Instalando POCO C++ Libraries..."
sudo apt-get install -y libpoco-dev

echo "Instalando gSOAP..."
sudo apt-get install -y gsoap libgsoap-dev

echo "Instalando Spread Toolkit..."
# 'spread' es el demonio de mensajería, libspread2-dev son las cabeceras C/C++
sudo apt-get install -y spread libspread2-dev

echo "¡Entorno listo!"