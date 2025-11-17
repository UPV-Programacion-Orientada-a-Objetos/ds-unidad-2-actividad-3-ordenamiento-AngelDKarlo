#!/bin/bash

# Script de configuración para E-Sort
# Detecta y configura el puerto serial del Arduino

echo "======================================================"
echo "  E-Sort - Configuración del Puerto Serial"
echo "======================================================"
echo ""

# Buscar puertos Arduino
echo "Buscando Arduino conectado..."
ARDUINO_PORTS=$(ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null)

if [ -z "$ARDUINO_PORTS" ]; then
    echo "❌ No se encontró ningún Arduino conectado."
    echo ""
    echo "Verifica:"
    echo "  1. El Arduino está conectado por USB"
    echo "  2. El cable USB funciona correctamente"
    echo "  3. Los drivers están instalados"
    echo ""
    exit 1
fi

echo "✓ Arduino(s) encontrado(s):"
echo "$ARDUINO_PORTS"
echo ""

# Seleccionar el primer puerto encontrado
SELECTED_PORT=$(echo "$ARDUINO_PORTS" | head -n 1)
echo "Puerto seleccionado: $SELECTED_PORT"
echo ""

# Verificar permisos
echo "Verificando permisos..."
if [ -r "$SELECTED_PORT" ] && [ -w "$SELECTED_PORT" ]; then
    echo "✓ Tienes permisos de lectura/escritura"
else
    echo "⚠ No tienes permisos suficientes"
    echo ""
    echo "Configurando permisos (requiere sudo)..."
    sudo chmod 666 "$SELECTED_PORT"
    
    if [ $? -eq 0 ]; then
        echo "✓ Permisos configurados correctamente"
    else
        echo "❌ Error al configurar permisos"
        exit 1
    fi
fi

echo ""
echo "======================================================"
echo "  Configuración Completada"
echo "======================================================"
echo ""
echo "Puerto configurado: $SELECTED_PORT"
echo ""
echo "Próximos pasos:"
echo "  1. Asegúrate de que el Arduino tenga cargado test.ino"
echo "  2. Edita main.cpp y ajusta SERIAL_PORT si es necesario:"
echo "     const char* SERIAL_PORT = \"$SELECTED_PORT\";"
echo "  3. Compila: make"
echo "  4. Ejecuta: ./esort"
echo ""
