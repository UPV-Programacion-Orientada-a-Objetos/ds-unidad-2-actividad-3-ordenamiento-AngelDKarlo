# Makefile para el Sistema E-Sort
# Compilador y banderas
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Nombre del ejecutable
TARGET = esort

# Archivos fuente
SOURCES = main.cpp

# Archivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Archivos de encabezado
HEADERS = DataSource.h SerialSource.h FileSource.h CircularBuffer.h Phase1.h Phase2.h

# Regla principal
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(OBJECTS)
	@echo "Enlazando $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Compilacion completada: ./$(TARGET)"

# Compilar archivos objeto
%.o: %.cpp $(HEADERS)
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	@echo "Limpiando archivos generados..."
	rm -f $(OBJECTS) $(TARGET)
	rm -f chunk_*.tmp output.sorted.txt
	@echo "Limpieza completada."

# Limpiar solo los datos (mantener el ejecutable)
cleandata:
	@echo "Limpiando archivos de datos..."
	rm -f chunk_*.tmp output.sorted.txt
	@echo "Archivos de datos eliminados."

# Dar permisos al puerto serial (requiere sudo)
permissions:
	@echo "Configurando permisos del puerto serial..."
	@sudo chmod 666 /dev/ttyACM0
	@echo "Permisos configurados para /dev/ttyACM0"

# Ejecutar el programa
run: $(TARGET)
	@echo "Ejecutando $(TARGET)..."
	./$(TARGET)

# Mostrar ayuda
help:
	@echo "Makefile para E-Sort - Sistema de Ordenamiento Externo"
	@echo ""
	@echo "Uso:"
	@echo "  make           - Compila el proyecto"
	@echo "  make run       - Compila y ejecuta el programa"
	@echo "  make clean     - Elimina todos los archivos generados"
	@echo "  make cleandata - Elimina solo los archivos de datos (.tmp, .txt)"
	@echo "  make permissions - Configura permisos del puerto serial (requiere sudo)"
	@echo "  make help      - Muestra esta ayuda"
	@echo ""
	@echo "Antes de ejecutar:"
	@echo "  1. Conecta el Arduino"
	@echo "  2. Verifica el puerto serial en SerialSource.h o main.cpp"
	@echo "  3. Ejecuta: make permissions (si es necesario)"
	@echo "  4. Ejecuta: make run"

.PHONY: all clean cleandata permissions run help
