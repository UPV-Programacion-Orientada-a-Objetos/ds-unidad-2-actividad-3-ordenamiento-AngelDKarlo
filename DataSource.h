#ifndef DATASOURCE_H
#define DATASOURCE_H

// Clase base abstracta para fuentes de datos
class DataSource {
public:
    // Obtiene el siguiente elemento de la fuente
    virtual int getNext() = 0;
    
    // Verifica si hay más datos disponibles
    virtual bool hasMoreData() = 0;
    
    // Destructor virtual obligatorio para polimorfismo
    virtual ~DataSource() {}
};

#endif
