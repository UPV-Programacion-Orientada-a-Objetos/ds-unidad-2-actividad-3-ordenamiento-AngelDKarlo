# Guía de Pruebas - Sistema E-Sort

## 🧪 Cómo Probar el Sistema

### Prueba 1: Verificar la Conexión del Arduino

```bash
# Verificar que el Arduino está conectado
ls /dev/tty*

# Ver los datos que envía el Arduino
cat /dev/ttyACM0
# (Presiona Ctrl+C para salir)
```

### Prueba 2: Compilar el Proyecto

```bash
# Limpiar compilaciones anteriores
make clean

# Compilar
make

# Verificar que se creó el ejecutable
ls -lh esort
```

### Prueba 3: Configurar Permisos Automáticamente

```bash
# Usar el script de configuración
./setup_serial.sh
```

### Prueba 4: Ejecución Básica (Pocos Datos)

Para hacer una prueba rápida, puedes modificar temporalmente el tamaño del buffer:

1. Edita `main.cpp` y cambia:
```cpp
const int BUFFER_SIZE = 10;  // En lugar de 1000
```

2. Recompila y ejecuta:
```bash
make clean
make
./esort
```

3. Deja que capture unos 30-40 números y detén con `Ctrl+C`

4. Verifica los resultados:
```bash
# Ver los chunks generados
ls -lh chunk_*.tmp

# Ver el contenido de un chunk
cat chunk_0.tmp

# Ver el archivo final ordenado
cat output.sorted.txt
```

### Prueba 5: Verificar que el Ordenamiento es Correcto

```bash
# Contar elementos procesados
wc -l output.sorted.txt

# Ver primeros 20 elementos (deben estar ordenados)
head -20 output.sorted.txt

# Ver últimos 20 elementos
tail -20 output.sorted.txt

# Verificar que están ordenados (solo en sistemas Linux)
# Si está ordenado, no debería mostrar diferencias
sort -n output.sorted.txt > sorted_check.txt
diff output.sorted.txt sorted_check.txt
```

### Prueba 6: Prueba de Gran Volumen

```bash
# Restaurar el buffer original en main.cpp
const int BUFFER_SIZE = 1000;

# Compilar
make clean && make

# Ejecutar y dejar correr por 1-2 minutos
./esort

# Detener con Ctrl+C

# Verificar estadísticas
echo "Chunks generados:"
ls chunk_*.tmp | wc -l

echo "Total de elementos:"
wc -l output.sorted.txt

echo "Tamaño del archivo final:"
ls -lh output.sorted.txt
```

## 🔍 Verificación Manual

### Verificar Chunks Individuales

```bash
# Ver primeras líneas de cada chunk
for file in chunk_*.tmp; do
    echo "=== $file ==="
    head -5 "$file"
    echo ""
done
```

### Verificar que cada Chunk está Ordenado

```bash
# Script para verificar que cada chunk está ordenado
for file in chunk_*.tmp; do
    echo "Verificando $file..."
    
    prev=-1
    ordered=true
    
    while read num; do
        if [ "$prev" -gt "$num" ]; then
            ordered=false
            echo "  ❌ Desordenado: $prev > $num"
            break
        fi
        prev=$num
    done < "$file"
    
    if $ordered; then
        echo "  ✓ Ordenado correctamente"
    fi
done
```

## 📊 Análisis de Resultados

### Ver Estadísticas del Archivo Final

```bash
# Primeras y últimas lecturas
echo "Primer valor: $(head -1 output.sorted.txt)"
echo "Último valor: $(tail -1 output.sorted.txt)"
echo "Total elementos: $(wc -l < output.sorted.txt)"
```

### Buscar Valores Específicos

```bash
# Buscar un valor específico
grep "^12345$" output.sorted.txt

# Contar cuántas veces aparece un valor
grep -c "^50000$" output.sorted.txt
```

## 🐛 Depuración

### Si el Programa no Lee del Arduino

```bash
# Ver si hay datos en el puerto
cat /dev/ttyACM0

# Verificar baudrate con stty
stty -F /dev/ttyACM0

# Reiniciar el Arduino
# (Desconectar y reconectar el USB)
```

### Si hay Errores de Compilación

```bash
# Verificar versión del compilador
g++ --version

# Compilar con más información de debug
g++ -Wall -Wextra -std=c++11 -g main.cpp -o esort
```

### Ver Mensajes de Debug

Puedes agregar prints adicionales en el código:

```cpp
// En SerialSource.h, en readLine():
printf("[DEBUG] Leído del serial: %s\n", line);

// En CircularBuffer.h, en insert():
printf("[DEBUG] Insertando: %d (size: %d/%d)\n", value, size, capacity);
```

## 🧹 Limpieza

```bash
# Eliminar todos los archivos generados
make clean

# Eliminar solo los datos, mantener el ejecutable
make cleandata

# Eliminar manualmente
rm -f chunk_*.tmp output.sorted.txt esort *.o
```

## ✅ Checklist de Verificación

- [ ] El Arduino está conectado y enviando datos
- [ ] El puerto serial tiene permisos correctos
- [ ] El proyecto compila sin errores ni warnings
- [ ] Se generan archivos `chunk_X.tmp`
- [ ] Cada chunk está ordenado internamente
- [ ] Se genera el archivo `output.sorted.txt`
- [ ] El archivo final está completamente ordenado
- [ ] No hay fugas de memoria (usar valgrind si está disponible)

## 🔬 Prueba con Valgrind (Opcional)

Si tienes valgrind instalado, puedes verificar fugas de memoria:

```bash
# Instalar valgrind
sudo apt-get install valgrind

# Ejecutar con valgrind (modificar BUFFER_SIZE a 10 primero)
valgrind --leak-check=full --show-leak-kinds=all ./esort
```

## 📈 Medición de Rendimiento

```bash
# Medir tiempo de ejecución
time ./esort

# Ver uso de memoria
/usr/bin/time -v ./esort
```

## 🎯 Casos de Prueba Sugeridos

1. **Caso pequeño**: Buffer de 10, capturar 30 elementos
2. **Caso mediano**: Buffer de 100, capturar 1000 elementos
3. **Caso grande**: Buffer de 1000, capturar 100,000+ elementos
4. **Caso límite**: Dejar correr por varios minutos hasta MAX_CHUNKS

¡Éxito con las pruebas! 🚀
