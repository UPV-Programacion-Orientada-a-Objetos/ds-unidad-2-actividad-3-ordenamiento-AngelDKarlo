#ifndef PHASE2_H
#define PHASE2_H

#include "FileSource.h"
#include <cstdio>

class Phase2 {
private:
    FileSource** sources;  // Arreglo de punteros a FileSource
    int k;                 // Número de chunks (K-Way Merge)
    int* current_values;   // Valores actuales de cada fuente
    bool* has_value;       // Indica si cada fuente tiene un valor disponible
    
    // Genera el nombre del archivo chunk
    void getChunkFilename(int chunk_num, char* filename) {
        sprintf(filename, "chunk_%d.tmp", chunk_num);
    }
    
    // Encuentra el índice del valor mínimo entre las K fuentes
    int findMinIndex() {
        int min_idx = -1;
        int min_value = 0;
        
        for (int i = 0; i < k; i++) {
            if (has_value[i]) {
                if (min_idx == -1 || current_values[i] < min_value) {
                    min_value = current_values[i];
                    min_idx = i;
                }
            }
        }
        
        return min_idx;
    }
    
public:
    // Constructor
    Phase2(int num_chunks) : k(num_chunks) {
        sources = new FileSource*[k];
        current_values = new int[k];
        has_value = new bool[k];
        
        // Inicializar todas las fuentes
        for (int i = 0; i < k; i++) {
            char filename[50];
            getChunkFilename(i, filename);
            sources[i] = new FileSource(filename);
            
            if (!sources[i]->isOpen()) {
                printf("Error: No se pudo abrir %s\n", filename);
                has_value[i] = false;
            } else {
                has_value[i] = false;
            }
        }
    }
    
    // Destructor
    ~Phase2() {
        for (int i = 0; i < k; i++) {
            delete sources[i];
        }
        delete[] sources;
        delete[] current_values;
        delete[] has_value;
    }
    
    // Ejecuta la Fase 2: Fusión Externa (K-Way Merge)
    void execute(const char* output_filename = "output.sorted.txt") {
        printf("Iniciando Fase 2: Fusion Externa (K-Way Merge)\n");
        printf("Abriendo %d archivos fuente...\n", k);
        
        // Abrir el archivo de salida
        FILE* output = fopen(output_filename, "w");
        if (!output) {
            printf("Error: No se pudo crear el archivo de salida.\n");
            return;
        }
        
        printf("K=%d. Fusion en progreso...\n", k);
        
        // Cargar el primer valor de cada fuente
        for (int i = 0; i < k; i++) {
            if (sources[i]->hasMoreData()) {
                current_values[i] = sources[i]->getNext();
                has_value[i] = true;
            } else {
                has_value[i] = false;
            }
        }
        
        int total_written = 0;
        
        // Proceso de fusión K-Way
        while (true) {
            int min_idx = findMinIndex();
            
            if (min_idx == -1) {
                // No hay más datos en ninguna fuente
                break;
            }
            
            // Escribir el valor mínimo al archivo de salida
            int min_value = current_values[min_idx];
            fprintf(output, "%d\n", min_value);
            total_written++;
            
            // Mostrar progreso cada cierto número de elementos
            if (total_written % 100 == 0 || total_written <= 20) {
                printf(" - Escribiendo %d... (total: %d)\n", min_value, total_written);
            }
            
            // Avanzar en la fuente del cual se extrajo el mínimo
            if (sources[min_idx]->hasMoreData()) {
                current_values[min_idx] = sources[min_idx]->getNext();
                has_value[min_idx] = true;
            } else {
                has_value[min_idx] = false;
            }
        }
        
        fclose(output);
        
        printf("\nFusion completada. Archivo final: %s\n", output_filename);
        printf("Total de elementos escritos: %d\n\n", total_written);
    }
};

#endif
