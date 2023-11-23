// Isabel Cristina Ramirez Ramirez - 
// Julian David Diaz Jaramillo

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

void mostrarBinarios(char arreglo[], int longitud) {
    for (int i = 0; i < longitud; i++) {
        printf("%c ", arreglo[i]);
    }
    printf("\n");
}

void decimal_a_binario(int decimal, int* binario) {
    int i = 0;
    while (i < 32) {
        binario[i] = decimal & 1;
        decimal = decimal >> 1;
        i++;
    }
}

int binario_a_decimal(char cadBinario[], int longitud){
    int decimal = 0;
    for(int i=longitud-1, j=0; i>=0; i--, j++){
        if(cadBinario[i] == '1'){
            decimal = decimal + pow(2, j);
        }
    }
    return decimal;
}

int main() {
    char input[100];
    int direccion;
    int binario[32]; 

    char *TLB = (char *) malloc(230);
    int longitud = 46;
    int ptr = 0;

    clock_t start_time, end_time;
    double tiempo_transcurrido;

    while (1) {
        printf("Ingrese la dirección virtual: ");
        scanf("%s", input);

        if (input[0] == 's' || input[0] == 'S') {
            printf("Saliendo del programa...\n");
            break;
        } 

        // Convertir la entrada a entero
        direccion = atoi(input);
        printf("Direccion: %d \n", direccion);

        start_time = clock();

        // Verificar si la dirección ya está en el TLB (en el heap)
        int estaLaDireccion = 0;
        int i;
        for (i = 0; i < 230; i += longitud) {
            int *direccionAuxiliar = (int *)(TLB + i);
            if (*direccionAuxiliar == direccion) {
                estaLaDireccion = 1;
                printf("TLB Hit\n");            
                // Obtener los datos directamente del TLB
                int pagDecimal, despDecimal;
                char pagBinario[20], despBinario[12];

                memcpy(&pagDecimal, TLB + i + sizeof(int), sizeof(int));
                memcpy(&despDecimal, TLB + i + 2 * sizeof(int), sizeof(int));
                memcpy(&pagBinario, TLB + i + 3 * sizeof(int), sizeof(pagBinario));
                memcpy(&despBinario, TLB + i + 3 * sizeof(int) + sizeof(pagBinario), sizeof(despBinario));

                printf("Pagina: %d\n", pagDecimal);
                printf("Desplazamiento: %d\n", despDecimal);

                printf("Pagina en binario: ");
                mostrarBinarios(pagBinario, sizeof(pagBinario) / sizeof(char));
                printf("Desplazamiento en binario: ");
                mostrarBinarios(despBinario, sizeof(despBinario) / sizeof(char));
                break;
            }
        }

        if (!estaLaDireccion) {
            // Mover el puntero al inicio si alcanza el final del TLB
            if (ptr + longitud > 230) {
                ptr = 0;
            }
            
            printf("TLB Miss\n");

            decimal_a_binario(direccion, binario);

            char pagBinario[20]; 
            char despBinario[12]; 

            for(int i=31, j=0; i >=11; i--, j++){
                pagBinario[j] = (char) binario[i] + '0'; // Convertir el bit a su representación ASCII ('0' o '1')
            }

            for(int j=0, i=11; i>=0; i--, j++){
                despBinario[j] = (char)binario[i] + '0'; // Convertir el bit a su representación ASCII ('0' o '1')
            }

            int pagDecimal = binario_a_decimal(pagBinario, sizeof(pagBinario)/sizeof(char));
            int despDecimal = binario_a_decimal(despBinario, sizeof(despBinario)/sizeof(char));
            printf("Pagina: %d\n", pagDecimal);
            printf("Desplazamiento: %d\n", despDecimal);

            printf("Pagina en binario: ");
            mostrarBinarios(pagBinario, sizeof(pagBinario)/sizeof(char));
            printf("Desplazamiento en binario: ");
            mostrarBinarios(despBinario, sizeof(despBinario)/sizeof(char));

            // Almacenar las variables de traducción en el TLB (en el heap)
            memcpy(TLB + ptr, &direccion, sizeof(int));
            memcpy(TLB + ptr + sizeof(int), &pagDecimal, sizeof(int));
            memcpy(TLB + ptr + 2 * sizeof(int), &despDecimal, sizeof(int));
            memcpy(TLB + ptr + 3 * sizeof(int), &pagBinario, sizeof(pagBinario));
            memcpy(TLB + ptr + 3 * sizeof(int) + sizeof(pagBinario), &despBinario, sizeof(despBinario));

            // Mover el puntero al siguiente espacio disponible para la siguiente entrada
            ptr += longitud;
        }

        end_time = clock();
        tiempo_transcurrido = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Tiempo: %.6f segundos\n", tiempo_transcurrido);

    }
    free(TLB);
    return 0;
}