#ifndef FILESOURCE_H
#define FILESOURCE_H

#include "DataSource.h"
#include <cstdio>

class FileSource : public DataSource {
private:
    FILE* file;
    bool has_data;
    
public:
    // Constructor: abre el archivo
    FileSource(const char* filename) {
        file = fopen(filename, "r");
        has_data = (file != nullptr);
    }
    
    // Obtiene el siguiente entero del archivo
    int getNext() override {
        if (!has_data || !file) {
            return 0;
        }
        
        int value;
        if (fscanf(file, "%d", &value) == 1) {
            return value;
        } else {
            has_data = false;
            return 0;
        }
    }
    
    // Verifica si hay más datos en el archivo
    bool hasMoreData() override {
        if (!has_data || !file) {
            return false;
        }
        
        // Verificar si llegamos al final
        int c = fgetc(file);
        if (c == EOF) {
            has_data = false;
            return false;
        }
        
        // Devolver el carácter leído
        ungetc(c, file);
        return true;
    }
    
    // Verifica si el archivo se abrió correctamente
    bool isOpen() const {
        return file != nullptr;
    }
    
    // Destructor: cierra el archivo
    ~FileSource() {
        if (file) {
            fclose(file);
        }
    }
};

#endif
