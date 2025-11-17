#ifndef SERIALSOURCE_H
#define SERIALSOURCE_H

#include "DataSource.h"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>

class SerialSource : public DataSource {
private:
    int serial_fd;  // File descriptor del puerto serial
    bool connected;
    char buffer[256];
    int buffer_pos;
    int buffer_len;
    
    // Lee un carácter del serial
    bool readChar(char& c) {
        if (buffer_pos >= buffer_len) {
            buffer_len = read(serial_fd, buffer, sizeof(buffer));
            buffer_pos = 0;
            if (buffer_len <= 0) {
                return false;
            }
        }
        c = buffer[buffer_pos++];
        return true;
    }
    
    // Lee una línea completa del serial
    bool readLine(char* line, int max_len) {
        int pos = 0;
        char c;
        
        while (pos < max_len - 1) {
            if (!readChar(c)) {
                return false;
            }
            
            if (c == '\n' || c == '\r') {
                if (pos > 0) {
                    line[pos] = '\0';
                    return true;
                }
                continue;
            }
            
            line[pos++] = c;
        }
        line[pos] = '\0';
        return pos > 0;
    }

public:
    // Constructor: se conecta al puerto serial
    SerialSource(const char* port = "/dev/ttyACM0", int baudrate = 9600) {
        connected = false;
        buffer_pos = 0;
        buffer_len = 0;
        
        // Abrir el puerto serial
        serial_fd = open(port, O_RDWR | O_NOCTTY);
        
        if (serial_fd < 0) {
            return;
        }
        
        // Configurar el puerto serial
        struct termios tty;
        memset(&tty, 0, sizeof(tty));
        
        if (tcgetattr(serial_fd, &tty) != 0) {
            close(serial_fd);
            return;
        }
        
        // Configurar baudrate
        speed_t speed = B9600;
        if (baudrate == 115200) speed = B115200;
        else if (baudrate == 57600) speed = B57600;
        else if (baudrate == 38400) speed = B38400;
        else if (baudrate == 19200) speed = B19200;
        
        cfsetospeed(&tty, speed);
        cfsetispeed(&tty, speed);
        
        // Configuración 8N1 (8 bits, sin paridad, 1 stop bit)
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;
        tty.c_cflag &= ~CRTSCTS;
        tty.c_cflag |= CREAD | CLOCAL;
        
        // Modo raw
        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO;
        tty.c_lflag &= ~ECHOE;
        tty.c_lflag &= ~ECHONL;
        tty.c_lflag &= ~ISIG;
        
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
        
        tty.c_oflag &= ~OPOST;
        tty.c_oflag &= ~ONLCR;
        
        tty.c_cc[VTIME] = 10;
        tty.c_cc[VMIN] = 0;
        
        if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
            close(serial_fd);
            return;
        }
        
        // Esperar a que el Arduino se inicialice
        sleep(2);
        
        connected = true;
    }
    
    // Obtiene el siguiente entero del serial
    int getNext() override {
        char line[256];
        if (readLine(line, sizeof(line))) {
            // Convertir string a entero manualmente
            int value = 0;
            int sign = 1;
            int i = 0;
            
            if (line[0] == '-') {
                sign = -1;
                i = 1;
            }
            
            while (line[i] >= '0' && line[i] <= '9') {
                value = value * 10 + (line[i] - '0');
                i++;
            }
            
            return value * sign;
        }
        return 0;
    }
    
    // Verifica si todavía hay datos disponibles
    bool hasMoreData() override {
        return connected;
    }
    
    // Verifica si la conexión se estableció
    bool isConnected() const {
        return connected;
    }
    
    // Destructor: cierra el puerto serial
    ~SerialSource() {
        if (serial_fd >= 0) {
            close(serial_fd);
        }
    }
};

#endif
