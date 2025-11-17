#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <cstdio>

// Nodo de la lista circular doblemente enlazada
struct Node {
    int data;
    Node* next;
    Node* prev;
    
    Node(int value) : data(value), next(nullptr), prev(nullptr) {}
};

class CircularBuffer {
private:
    Node* head;      // Apuntador al primer nodo
    int capacity;    // Tamaño máximo del buffer
    int size;        // Tamaño actual del buffer
    
public:
    // Constructor: crea un buffer con capacidad fija
    CircularBuffer(int cap) : head(nullptr), capacity(cap), size(0) {}
    
    // Destructor: libera toda la memoria
    ~CircularBuffer() {
        clear();
    }
    
    // Inserta un elemento en el buffer
    void insert(int value) {
        if (size >= capacity) {
            return; // Buffer lleno
        }
        
        Node* newNode = new Node(value);
        
        if (head == nullptr) {
            // Primer nodo
            head = newNode;
            head->next = head;
            head->prev = head;
        } else {
            // Insertar al final de la lista circular
            Node* tail = head->prev;
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }
        
        size++;
    }
    
    // Verifica si el buffer está lleno
    bool isFull() const {
        return size >= capacity;
    }
    
    // Obtiene el tamaño actual del buffer
    int getSize() const {
        return size;
    }
    
    // Verifica si el buffer está vacío
    bool isEmpty() const {
        return size == 0;
    }
    
    // Ordena los elementos del buffer usando Insertion Sort
    void sort() {
        if (size <= 1) return;
        
        // Convertir a arreglo temporal para facilitar el ordenamiento
        int* arr = new int[size];
        Node* current = head;
        
        for (int i = 0; i < size; i++) {
            arr[i] = current->data;
            current = current->next;
        }
        
        // Insertion Sort
        for (int i = 1; i < size; i++) {
            int key = arr[i];
            int j = i - 1;
            
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        
        // Copiar de vuelta a la lista
        current = head;
        for (int i = 0; i < size; i++) {
            current->data = arr[i];
            current = current->next;
        }
        
        delete[] arr;
    }
    
    // Escribe el contenido ordenado del buffer en un archivo
    void writeToFile(const char* filename) {
        FILE* file = fopen(filename, "w");
        if (!file) {
            return;
        }
        
        Node* current = head;
        for (int i = 0; i < size; i++) {
            fprintf(file, "%d\n", current->data);
            current = current->next;
        }
        
        fclose(file);
    }
    
    // Limpia el buffer (libera memoria)
    void clear() {
        if (head == nullptr) return;
        
        Node* current = head;
        Node* next;
        
        // Romper el círculo
        head->prev->next = nullptr;
        
        while (current != nullptr) {
            next = current->next;
            delete current;
            current = next;
        }
        
        head = nullptr;
        size = 0;
    }
    
    // Imprime el contenido del buffer (para debugging)
    void print() const {
        if (head == nullptr) {
            printf("[Buffer vacío]\n");
            return;
        }
        
        printf("[");
        Node* current = head;
        for (int i = 0; i < size; i++) {
            printf("%d", current->data);
            if (i < size - 1) printf(", ");
            current = current->next;
        }
        printf("]\n");
    }
};

#endif
