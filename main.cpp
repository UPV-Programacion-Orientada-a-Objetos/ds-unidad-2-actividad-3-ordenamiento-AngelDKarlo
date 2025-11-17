#include "Phase1.h"
#include "Phase2.h"
#include <cstdio>
#include <cstdlib>

int main() {
    printf("====================================================\n");
    printf("  Sistema de Ordenamiento Externo E-Sort\n");
    printf("  Telemetria Masiva con Arduino\n");
    printf("====================================================\n\n");
    
    // Configuración
    const char* SERIAL_PORT = "/dev/ttyACM0";  // Cambiar según tu sistema
    const int BUFFER_SIZE = 1000;               // Tamaño del buffer circular
    const int MAX_CHUNKS = 100;                 // Máximo de chunks a generar
    
    printf("Configuracion:\n");
    printf("  - Puerto Serial: %s\n", SERIAL_PORT);
    printf("  - Tamano del Buffer: %d elementos\n", BUFFER_SIZE);
    printf("  - Maximo de Chunks: %d\n\n", MAX_CHUNKS);
    
    // ========== FASE 1: ADQUISICIÓN Y SEGMENTACIÓN ==========
    printf("Conectando a puerto %s (Arduino)... ", SERIAL_PORT);
    fflush(stdout);
    
    Phase1* phase1 = new Phase1(SERIAL_PORT, BUFFER_SIZE, MAX_CHUNKS);
    
    if (!phase1->isConnected()) {
        printf("ERROR.\n");
        printf("\nNo se pudo conectar al Arduino.\n");
        printf("Verifica:\n");
        printf("  1. El Arduino esta conectado\n");
        printf("  2. El puerto serial es correcto (%s)\n", SERIAL_PORT);
        printf("  3. Tienes permisos para acceder al puerto\n");
        printf("     Ejecuta: sudo chmod 666 %s\n\n", SERIAL_PORT);
        delete phase1;
        return 1;
    }
    
    printf("Conectado.\n\n");
    
    // Ejecutar la Fase 1
    int chunk_count = phase1->execute();
    
    if (chunk_count == 0) {
        printf("Error: No se generaron chunks. Abortando.\n");
        delete phase1;
        return 1;
    }
    
    delete phase1;
    
    // ========== FASE 2: FUSIÓN EXTERNA (K-WAY MERGE) ==========
    Phase2* phase2 = new Phase2(chunk_count);
    phase2->execute("output.sorted.txt");
    delete phase2;
    
    // ========== FINALIZACIÓN ==========
    printf("====================================================\n");
    printf("  Proceso completado exitosamente\n");
    printf("====================================================\n\n");
    printf("Archivos generados:\n");
    for (int i = 0; i < chunk_count; i++) {
        printf("  - chunk_%d.tmp\n", i);
    }
    printf("  - output.sorted.txt (ARCHIVO FINAL)\n\n");
    
    printf("Liberando memoria... ");
    printf("Sistema apagado.\n\n");
    
    return 0;
}
