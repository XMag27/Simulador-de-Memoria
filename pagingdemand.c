#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PAGE_TABLE_ENTRIES 256
#define PAGE_SIZE 256
#define FRAME_COUNT 256
#define FRAME_SIZE 256
#define PHYSICAL_MEMORY_SIZE FRAME_COUNT * FRAME_SIZE
#define MAX_BUFFER_SIZE 10

typedef struct EntradaTablaPagina{
    uint8_t pfn;
    char valido;
} EntradaTablaPagina;

int main (int argc, char *argv[]) {


    //Validar uso correcto del programa
    if (argc != 3) {
        printf("Argumentos incorrectos\n");
        printf("Uso: ./pagingdemand <direcciones.txt> <salida.txt>\n");
        exit(1);
    }
    //Declarar archivos
    FILE *archivo_direcciones;
    FILE *archivo_salida;
    FILE *BACKING_STORE;
    char buffer[MAX_BUFFER_SIZE];
    //Abrir archivo de direcciones fisicas
    archivo_direcciones = fopen(argv[1], "r");
    if (archivo_direcciones == NULL) {
        printf("Error al abrir el archivo de direcciones\n");
        exit(1);
    }
    //Abrir archivo de salida
    archivo_salida = fopen(argv[2], "w");
    if (archivo_salida == NULL) {
        printf("Error al abrir el archivo de salida\n");
        exit(1);
    }

    //Abrir archivo de Backing Store
    BACKING_STORE = fopen("BACKING_STORE.bin", "r");
    if (BACKING_STORE == NULL) {
        printf("Error al abrir el archivo de Backing Store\n");
        exit(1);
    }
    //Lectura del archivo e imprime en pantalla cada una de las direcciones
    while(fgets(buffer, sizeof(buffer), archivo_direcciones) != NULL) {
        uint32_t direccion_virtual = strtoul(buffer, NULL, 10);
        //Separamos las partes que nos interesan de la direccion virtual
        //Para obtener los ultimos 8 bits, es decir el offset, se utiliza ,a operación AND con 0xFF, asi que siempre obtendremos los valores perados
        uint32_t offset = direccion_virtual & 0xFF;
        //Para obtener los 8 bits del VPN, se hace un corrimiento de 8 bits a la derecha y se vuelve a hacer una operación AND con 0xFF
        uint32_t vpn = (direccion_virtual >> 8) & 0xFF; 

    }
}