[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Yb9MZvBR)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=21686670)
# Caso de Estudio: Sistema de Ordenamiento Externo para Telemetría Masiva (E-Sort)

## Definición del Problema a Resolver (El Caso de Estudio)

Un laboratorio de física de partículas está realizando un experimento a largo plazo. Un detector (simulado por un **Arduino**) genera lecturas de energía (valores numéricos enteros) a una velocidad que **excede la capacidad de la RAM** del sistema de adquisición.

El objetivo es diseñar un sistema en C++ que capture este flujo masivo de datos del puerto serial, lo ordene completamente y lo guarde en un archivo final.

Debido a la restricción de memoria, el sistema no puede cargar todos los datos en un arreglo. En su lugar, debe implementar un **Ordenamiento Externo (External Mergesort)**. El sistema deberá usar una **Lista Circular** como un *buffer* de memoria fijo para la entrada de datos.

### El Desafío del Diseño

El sistema se dividirá en dos fases que deben ser encapsuladas usando **POO (Clases Abstractas y Herencia)**:

1. **Fase 1 (Adquisición y Segmentación):** Leer el flujo de datos del Arduino. Los datos entrantes se almacenarán temporalmente en una **Lista Circular (Buffer)**. Cuando esta lista circular se llena, su contenido se ordena internamente (usando un algoritmo simple) y se vuelca a un archivo temporal (`chunk_01.tmp`). Este proceso se repite, creando múltiples "trozos" (chunks) de datos ordenados.
2. **Fase 2 (Fusión Externa):** Implementar la fase de **"Merge"** del algoritmo **Mergesort** para fusionar los K trozos ordenados (`chunk_01.tmp`, `chunk_02.tmp`...) en un único archivo de salida (`output.sorted.txt`).

## Temas Relacionados y Necesarios

Para resolver este caso de estudio, los alumnos deberán dominar la integración de los siguientes conceptos:

| Tema Principal | Concepto a Aplicar |
| :--- | :--- |
| **POO Avanzada (Herencia/Polimorfismo)** | **Clase Base Abstracta** (`DataSource`) y **Clases Derivadas** (`SerialSource`, `FileSource`) para abstraer el origen de los datos. |
| **Algoritmos de Ordenamiento (Mergesort)** | Implementación de la fase **K-Way Merge** (fusión de K vías) del algoritmo Mergesort para el ordenamiento externo. |
| **Listas Circulares (Manual)** | Implementación de una `CircularBuffer` (Lista Circular Doblemente Enlazada) de tamaño fijo para la ingesta de datos en memoria. |
| **Comunicación Serial (Hardware)** | Lectura de datos en tiempo real desde un puerto COM (simulado por el Arduino). |
| **Ordenamiento Externo** | El concepto de dividir un conjunto de datos masivo en "chunks" ordenados y luego fusionarlos. |
| **Punteros y Gestión de Memoria** | Uso de `new`/`delete` para todos los nodos y la gestión de múltiples punteros de archivos. |

-----

## Definición y Detalles del Proceso a Desarrollar

### Diseño de la Jerarquía de Clases (POO)

El sistema debe ser modular. Se sugiere una clase base abstracta para la lectura de datos, ya que leeremos del serial (Fase 1) y luego de los archivos (Fase 2).

1. **Clase Base Abstracta (`DataSource`):**

* **Propósito:** Define una interfaz para una fuente de datos que puede ser leída.
* **Métodos Virtuales Puros:** `virtual int getNext() = 0;`, `virtual bool hasMoreData() = 0;`, `virtual ~DataSource() {}`. (El destructor virtual es obligatorio).

2. **Clase Concreta (`SerialSource`):**

* Hereda de `DataSource`.
* Se conecta al puerto serial (Arduino) en su constructor.
* `getNext()`: Lee, parsea (ej. `atoi`) y devuelve el siguiente entero del serial.

3. **Clase Concreta (`FileSource`):**

* Hereda de `DataSource`.
* Abre un archivo (`chunk_XX.tmp`) en su constructor.
* `getNext()`: Lee y devuelve el siguiente entero del archivo.

### Fase 1: Adquisición y Segmentación (La Lista Circular)

1. Implementar la clase `CircularBuffer` (basada en una **Lista Circular Doblemente Enlazada**) con un tamaño fijo (ej. 1000 elementos).
2. El programa principal usa un `SerialSource` para leer datos.
3. Por cada dato leído, se inserta en el `CircularBuffer`.
4. Cuando el buffer está lleno:
   * Se aplica un ordenamiento interno a los datos *dentro* del buffer (un `InsertionSort` o un `Mergesort` recursivo sobre los nodos de la lista es aceptable).
   * El contenido ordenado del buffer se escribe en un nuevo archivo: `chunk_01.tmp`.
   * El buffer se vacía y el proceso se repite para `chunk_02.tmp`, y así sucesivamente.

### Fase 2: Fusión Externa (El Mergesort)

1. Esta es la fase de "Merge" (Fusión).
2. El sistema debe crear un **arreglo de K punteros** a `DataSource` (específicamente, `K` objetos `FileSource`), donde K es el número de *chunks* creados en la Fase 1.
3. **Algoritmo K-Way Merge:**
   * Leer el primer elemento de cada uno de los K `FileSource`.
   * Buscar el elemento **mínimo** entre esos K elementos.
   * Escribir ese elemento mínimo en el archivo de salida final (`output.sorted.txt`).
   * Avanzar (llamar a `getNext()`) solo en el `FileSource` del cual se extrajo el mínimo.
   * Repetir este proceso (Buscar mínimo, escribir, avanzar) hasta que todos los `FileSource` reporten `hasMoreData() == false`.

-----

## 04\. Requerimientos Funcionales y No Funcionales

### Requisitos Funcionales

1. **Conexión Serial:** El programa debe conectarse y leer el flujo de enteros del Arduino.
2. **Buffer Circular:** Implementar la lista circular como un buffer de tamaño fijo.
3. **Generación de Chunks:** El sistema debe crear N archivos (`.tmp`) con datos ordenados internamente.
4. **Fusión Externa:** Implementar el K-Way Merge para fusionar todos los chunks en un único archivo de salida.
5. **Ordenamiento Correcto:** El archivo `output.sorted.txt` debe contener todos los números recibidos, ordenados de menor a mayor.

### Requisitos No Funcionales

1. **Exclusividad de Punteros:** Prohibido el uso de la STL (`std::vector`, `std::list`, `std::string`, `std::sort`). Todo debe ser manual.
2. **POO Avanzado:** El diseño debe usar Clases Abstractas y Herencia para la gestión de las fuentes de datos.
3. **Gestión de Memoria:** No debe haber fugas de memoria (los destructores virtuales y la limpieza de nodos son críticos).
4. **Eficiencia:** El sistema debe funcionar con una huella de memoria constante (definida por el tamaño del `CircularBuffer`).

-----

## 05\. Ejemplo de Entradas y Salidas del Problema en Consola

### Entrada (Datos enviados por el Arduino)

El Arduino enviará un flujo constante de números (uno por línea):

```Bash
105
5
210
99
1
... (continúa indefinidamente)
```

### Salida Esperada del Sistema (En la Consola de C++)

(Asumiendo un `CircularBuffer` de tamaño 4 para simplificar)

```Bash
Iniciando Sistema de Ordenamiento Externo E-Sort...
Conectando a puerto COM3 (Arduino)... Conectado.
Iniciando Fase 1: Adquisición de datos...

Leyendo -> 105
Leyendo -> 5
Leyendo -> 210
Leyendo -> 99
Buffer lleno. Ordenando internamente...
Buffer ordenado: [5, 99, 105, 210]
Escribiendo chunk_0.tmp... OK.
Buffer limpiado.

Leyendo -> 1
Leyendo -> 500
Leyendo -> 20
Leyendo -> 15
Buffer lleno. Ordenando internamente...
Buffer ordenado: [1, 15, 20, 500]
Escribiendo chunk_1.tmp... OK.
Buffer limpiado.

(El Arduino se detiene o se cierra la conexión)
Fase 1 completada. 2 chunks generados.

Iniciando Fase 2: Fusión Externa (K-Way Merge)
Abriendo 2 archivos fuente...
K=2. Fusión en progreso...
 - Min(chunk_0[5], chunk_1[1]) -> 1. Escribiendo 1.
 - Min(chunk_0[5], chunk_1[15]) -> 5. Escribiendo 5.
 - Min(chunk_0[99], chunk_1[15]) -> 15. Escribiendo 15.
 - Min(chunk_0[99], chunk_1[20]) -> 20. Escribiendo 20.
 - Min(chunk_0[99], chunk_1[500]) -> 99. Escribiendo 99.
 ... (etc.)

Fusión completada. Archivo final: output.sorted.txt
Liberando memoria... Sistema apagado.
```

*(El archivo `output.sorted.txt` contendría: 1, 5, 15, 20, 99, 105, 210, 500)*

-----

## Temas Adicionales de Investigación Necesarios

Para llevar a cabo esta actividad, el alumno deberá investigar y dominar a fondo:

1. **Algoritmos de Ordenamiento Externo:** El concepto de "K-Way Merge" (Fusión de K vías) y por qué es una aplicación de Mergesort.
2. **Programación Serial en C++ (El Reto Principal):** Cómo abrir, configurar (Baud Rate) y leer desde un puerto COM (ej. `\\.\COM3` en Windows o `/dev/ttyUSB0` en Linux) usando **solo librerías estándar** (como `<fstream>` o APIs de Win32/POSIX).
3. **Gestión de Múltiples Archivos:** El uso de `<fstream>` (o `FILE*` en C) para manejar K archivos de entrada simultáneamente y un archivo de salida.
4. **Parseo de Cadenas (C-style):** Convertir el texto leído del serial (ej. "105") a un entero (ej. `atoi()`), sin usar `std::string`.
5. **Destructores Virtuales:** La necesidad **crítica** de que `~DataSource()` sea virtual para asegurar que el destructor de `FileSource` (que cierra el archivo) se llame correctamente al hacer `delete` sobre un puntero.

-----

## ✅ IMPLEMENTACIÓN COMPLETADA

### 📁 Archivos del Proyecto

- **DataSource.h** - Clase base abstracta para fuentes de datos
- **SerialSource.h** - Clase que lee datos del Arduino por puerto serial
- **FileSource.h** - Clase que lee datos de archivos
- **CircularBuffer.h** - Lista circular doblemente enlazada de tamaño fijo
- **Phase1.h** - Fase 1: Adquisición y segmentación
- **Phase2.h** - Fase 2: Fusión externa (K-Way Merge)
- **main.cpp** - Programa principal que coordina ambas fases
- **Makefile** - Sistema de compilación
- **setup_serial.sh** - Script para configurar el puerto serial
- **INSTRUCCIONES.md** - Guía detallada de uso

### 🚀 Inicio Rápido

```bash
# 1. Configurar puerto serial (opcional)
./setup_serial.sh

# 2. Compilar
make

# 3. Ejecutar
./esort
```

### 📊 Características Implementadas

✅ **POO Avanzada**: Herencia, polimorfismo y clases abstractas  
✅ **Lista Circular**: Implementación manual con punteros (buffer de 1000 elementos)  
✅ **Ordenamiento Externo**: External Mergesort con K-Way Merge  
✅ **Comunicación Serial**: Lectura en tiempo real desde Arduino  
✅ **Sin STL**: Todo implementado manualmente (sin vector, list, string, sort)  
✅ **Gestión de Memoria**: Uso correcto de new/delete y destructores virtuales  

### 📖 Documentación

Consulta **INSTRUCCIONES.md** para una guía completa de uso, configuración y solución de problemas.

### 🏗️ Arquitectura del Sistema

```
Arduino (test.ino)
    ↓ [Serial Port]
SerialSource → CircularBuffer → Chunks (.tmp)
                                    ↓
                              K-Way Merge
                                    ↓
                            output.sorted.txt
```

### 🎯 Funcionamiento

1. **Fase 1**: Lee del Arduino, llena buffer circular, ordena y genera chunks
2. **Fase 2**: Fusiona todos los chunks en un archivo final ordenado

**Memoria constante**: Solo usa el buffer circular (1000 elementos), sin importar cuántos datos procese.