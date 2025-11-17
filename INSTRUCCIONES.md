# Instrucciones de Uso - Sistema E-Sort

## 📋 Descripción
Sistema de Ordenamiento Externo para telemetría masiva que lee datos de un Arduino, los ordena usando un algoritmo de ordenamiento externo (External Mergesort) y genera un archivo final ordenado.

## 🔧 Requisitos Previos

1. **Arduino conectado** con el código `test.ino` cargado
2. **Compilador g++** con soporte C++11
3. **Permisos de acceso** al puerto serial
4. **Linux** (el código está configurado para sistemas POSIX)

## 🚀 Pasos para Ejecutar

### 1. Conectar el Arduino

```bash
# Cargar el código test.ino en el Arduino usando el IDE de Arduino
# El Arduino generará números aleatorios de 0 a 65535
```

### 2. Identificar el Puerto Serial

```bash
# Listar puertos disponibles
ls /dev/tty*

# Normalmente será /dev/ttyACM0 o /dev/ttyUSB0
```

### 3. Configurar Permisos (si es necesario)

```bash
# Opción 1: Dar permisos temporales
sudo chmod 666 /dev/ttyACM0

# Opción 2: Usar el Makefile
make permissions
```

### 4. Ajustar el Puerto Serial (si es diferente a /dev/ttyACM0)

Editar `main.cpp` línea 12:
```cpp
const char* SERIAL_PORT = "/dev/ttyACM0";  // Cambiar según tu sistema
```

### 5. Compilar el Proyecto

```bash
# Compilar
make

# O compilar y ejecutar directamente
make run
```

### 6. Ejecutar el Programa

```bash
./esort
```

## 📊 Funcionamiento del Sistema

### Fase 1: Adquisición y Segmentación
1. Se conecta al Arduino por puerto serial
2. Lee los datos continuamente
3. Los almacena en un buffer circular de 1000 elementos
4. Cuando el buffer se llena:
   - Ordena los datos internamente (Insertion Sort)
   - Escribe el chunk ordenado a un archivo `chunk_X.tmp`
   - Limpia el buffer y continúa

### Fase 2: Fusión Externa (K-Way Merge)
1. Abre todos los archivos `chunk_X.tmp` generados
2. Lee el primer elemento de cada chunk
3. Selecciona el mínimo entre todos
4. Lo escribe al archivo final `output.sorted.txt`
5. Avanza en el chunk del cual se extrajo el mínimo
6. Repite hasta procesar todos los elementos

## 📁 Archivos Generados

- `chunk_0.tmp`, `chunk_1.tmp`, ... - Chunks ordenados intermedios
- `output.sorted.txt` - **Archivo final con todos los datos ordenados**

## 🛠️ Comandos del Makefile

```bash
make            # Compila el proyecto
make run        # Compila y ejecuta
make clean      # Elimina todos los archivos generados
make cleandata  # Elimina solo los datos (.tmp y .txt)
make permissions # Configura permisos del puerto serial
make help       # Muestra ayuda
```

## 🎯 Configuración

En `main.cpp` puedes ajustar:

```cpp
const char* SERIAL_PORT = "/dev/ttyACM0";  // Puerto del Arduino
const int BUFFER_SIZE = 1000;              // Tamaño del buffer circular
const int MAX_CHUNKS = 100;                // Máximo de chunks a generar
```

## 🔍 Verificar Resultados

```bash
# Ver los primeros 20 números ordenados
head -20 output.sorted.txt

# Ver los últimos 20 números ordenados
tail -20 output.sorted.txt

# Contar total de números procesados
wc -l output.sorted.txt
```

## ⚠️ Solución de Problemas

### Error: "No se pudo conectar al Arduino"
- Verifica que el Arduino esté conectado: `ls /dev/tty*`
- Revisa los permisos: `sudo chmod 666 /dev/ttyACM0`
- Asegúrate de que el puerto serial sea el correcto

### Error: "Permission denied"
- Ejecuta: `make permissions`
- O agrega tu usuario al grupo dialout: `sudo usermod -a -G dialout $USER`
- Cierra sesión y vuelve a entrar

### El programa no lee datos
- Verifica que el código `test.ino` esté cargado en el Arduino
- Abre el Monitor Serial del IDE de Arduino para verificar que envía datos
- Asegúrate de que el baudrate coincida (9600)

## 📝 Estructura del Proyecto

```
.
├── DataSource.h         - Clase base abstracta
├── SerialSource.h       - Lee datos del Arduino
├── FileSource.h         - Lee datos de archivos
├── CircularBuffer.h     - Buffer circular (lista circular doblemente enlazada)
├── Phase1.h            - Fase 1: Adquisición y segmentación
├── Phase2.h            - Fase 2: K-Way Merge
├── main.cpp            - Programa principal
├── Makefile            - Sistema de compilación
├── test.ino            - Código del Arduino
└── README.md           - Descripción del proyecto
```

## 🎓 Conceptos Implementados

✅ **POO Avanzada**: Herencia, Polimorfismo, Clases Abstractas  
✅ **Listas Circulares**: Implementación manual con punteros  
✅ **Ordenamiento Externo**: External Mergesort, K-Way Merge  
✅ **Comunicación Serial**: Lectura desde Arduino en tiempo real  
✅ **Gestión de Memoria**: new/delete, destructores virtuales  
✅ **Sin STL**: Todo implementado manualmente con punteros

## 💡 Notas Importantes

- El programa leerá datos **indefinidamente** del Arduino hasta que se detenga con `Ctrl+C` o alcance el máximo de chunks (100 por defecto)
- Para detener la lectura y procesar los datos, presiona `Ctrl+C` cuando tengas suficientes datos
- El buffer circular mantiene una huella de memoria constante (1000 elementos)
- Todos los datos se ordenan sin cargar todo en memoria simultáneamente

## 🧪 Ejemplo de Ejecución

```bash
$ make run
Compilando main.cpp...
Enlazando esort...
Compilacion completada: ./esort
Ejecutando esort...
====================================================
  Sistema de Ordenamiento Externo E-Sort
  Telemetria Masiva con Arduino
====================================================

Configuracion:
  - Puerto Serial: /dev/ttyACM0
  - Tamano del Buffer: 1000 elementos
  - Maximo de Chunks: 100

Conectando a puerto /dev/ttyACM0 (Arduino)... Conectado.

Iniciando Fase 1: Adquisicion de datos...

Leyendo -> 45321
Leyendo -> 12890
Leyendo -> 54123
...
```

¡Éxito en tu proyecto! 🚀
