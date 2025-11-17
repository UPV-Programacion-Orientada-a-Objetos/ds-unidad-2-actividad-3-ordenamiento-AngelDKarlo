#ifndef PHASE1_H
#define PHASE1_H

#include "SerialSource.h"
#include "CircularBuffer.h"
#include <cstdio>

class Phase1 {
private:
    SerialSource* source;
    CircularBuffer* buffer;
    int chunk_count;
    int max_chunks;
    
    // Genera el nombre del archivo chunk
    void getChunkFilename(int chunk_num, char* filename) {
        sprintf(filename, "chunk_%d.tmp", chunk_num);
    }
    
public:
    // Constructor
    Phase1(const char* port, int buffer_size, int max_chunks_limit = 100) 
        : chunk_count(0), max_chunks(max_chunks_limit) {
        source = new SerialSource(port);
        buffer = new CircularBuffer(buffer_size);
    }
    
    // Destructor
    ~Phase1() {
        delete buffer;
        delete source;
    }
    
    // Verifica si la conexión al Arduino fue exitosa
    bool isConnected() const {
        return source->isConnected();
    }
    
    // Ejecuta la Fase 1: Adquisición y Segmentación
    int execute() {
        printf("Iniciando Fase 1: Adquisicion de datos...\n\n");
        
        if (!source->isConnected()) {
            printf("Error: No se pudo conectar al Arduino.\n");
            return 0;
        }
        
        int total_readings = 0;
        
        while (source->hasMoreData() && chunk_count < max_chunks) {
            int value = source->getNext();
            printf("Leyendo -> %d\n", value);
            
            buffer->insert(value);
            total_readings++;
            
            if (buffer->isFull()) {
                printf("Buffer lleno. Ordenando internamente...\n");
                
                // Ordenar el buffer
                buffer->sort();
                
                printf("Buffer ordenado: ");
                buffer->print();
                
                // Generar nombre de archivo
                char filename[50];
                getChunkFilename(chunk_count, filename);
                
                // Escribir a archivo
                printf("Escribiendo %s... ", filename);
                buffer->writeToFile(filename);
                printf("OK.\n");
                
                // Limpiar el buffer
                buffer->clear();
                printf("Buffer limpiado.\n\n");
                
                chunk_count++;
            }
        }
        
        // Si quedan datos en el buffer (último chunk parcial)
        if (!buffer->isEmpty()) {
            printf("Procesando ultimo chunk parcial...\n");
            printf("Buffer con %d elementos. Ordenando internamente...\n", buffer->getSize());
            
            buffer->sort();
            printf("Buffer ordenado: ");
            buffer->print();
            
            char filename[50];
            getChunkFilename(chunk_count, filename);
            
            printf("Escribiendo %s... ", filename);
            buffer->writeToFile(filename);
            printf("OK.\n");
            
            chunk_count++;
        }
        
        printf("\nFase 1 completada. %d chunks generados.\n", chunk_count);
        printf("Total de lecturas: %d\n\n", total_readings);
        
        return chunk_count;
    }
    
    // Obtiene el número de chunks generados
    int getChunkCount() const {
        return chunk_count;
    }
};

#endif
