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


//Estructura para representar una entrada en la tabla de paginas
typedef struct EntradaTablaPagina{
    uint8_t pfn;
    char valido; //Ver si existe o no un Page Fault
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

    //Asignar memoria para la memoria fisica
    char **memoria_fisica = (char **) malloc(sizeof(char *) * PHYSICAL_MEMORY_SIZE);
    //Asignar memoria para la tabla de paginas
    EntradaTablaPagina *tabla_paginas = (EntradaTablaPagina *) malloc(sizeof(EntradaTablaPagina) * PAGE_TABLE_ENTRIES);
    //Lectura del archivo e imprime en pantalla cada una de las direcciones
    while(fgets(buffer, sizeof(buffer), archivo_direcciones) != NULL) {
        uint32_t direccion_virtual = strtoul(buffer, NULL, 10);
        //Separamos las partes que nos interesan de la direccion virtual
        //Para obtener los ultimos 8 bits, es decir el offset, se utiliza ,a operación AND con 0xFF, asi que siempre obtendremos los valores perados
        uint32_t offset = direccion_virtual & 0xFF;
        //Para obtener los 8 bits del VPN, se hace un corrimiento de 8 bits a la derecha y se vuelve a hacer una operación AND con 0xFF
        uint32_t vpn = (direccion_virtual >> 8) & 0xFF; 
        //Si la entrada en la tabla de paginas no es valida, se carga la pagina desde el Backing Store
        if (!(tabla_paginas[vpn].valido)) {
            //Se asigna memoria para el buffer de la pagina
            char *buffer_pagina = (char *) malloc(sizeof(char) * PAGE_SIZE);
            //Preparar la pagina desde el Backing Store
            if (fseek(BACKING_STORE, vpn * PAGE_SIZE, SEEK_SET) > 0) {
                printf("Error al leer el archivo\n");
                exit(1);
            }
            if (fread(buffer_pagina, 1, PAGE_SIZE, BACKING_STORE) < 0) {
                printf("Error al leer el archivo\n");
                exit(1);
            }
            //Busca un espacio libre en la memoria fisica
            int i;
            for (i = 0; i < FRAME_COUNT; i++) {
                if (memoria_fisica[i]) continue;
                //Asigna la pagina a la memoria fisica
                memoria_fisica[i] = buffer_pagina;
                //Escribe el numero de frame en la tabla de paginas
                tabla_paginas[vpn].pfn = i;
                //Escribe que la entrada es valida
                tabla_paginas[vpn].valido = 1;
                break;
            }
        }
        //Obtener el valor de la direccion fisica
        uint32_t direccion_fisica = (tabla_paginas[vpn].pfn * FRAME_SIZE) + offset;
        //Obtener el valor de la direccion fisica
        int8_t valor = memoria_fisica[tabla_paginas[vpn].pfn][offset];
        //Escribir en el archivo de salida
        fprintf(archivo_salida, "Virtual address: %d Physical address: %d Value: %d\n", direccion_virtual, direccion_fisica, valor);
        
    }
}