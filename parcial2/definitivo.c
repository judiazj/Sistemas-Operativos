#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

        decimal_a_binario(direccion, binario);

        char paginaBinario[20]; 
        char despBinario[12]; 

        for(int i=31, j=0; i >=11; i--, j++){
            paginaBinario[j] = (char) binario[i] + '0'; // Convertir el bit a su representación ASCII ('0' o '1')
        }

        for(int j=0, i=11; i>=0; i--, j++){
            despBinario[j] = (char)binario[i] + '0'; // Convertir el bit a su representación ASCII ('0' o '1')
        }

        printf("Pagina: %d\n", binario_a_decimal(paginaBinario, sizeof(paginaBinario)/sizeof(char)));
        printf("Desplazamiento: %d\n", binario_a_decimal(despBinario, sizeof(despBinario)/sizeof(char)));

        printf("Pagina en binario: ");
        mostrarBinarios(paginaBinario, sizeof(paginaBinario)/sizeof(char));
        printf("Desplazamiento en binario: ");
        mostrarBinarios(despBinario, sizeof(despBinario)/sizeof(char));

    
        // Aquí puedes continuar con el resto de la lógica para el programa
        // Realizar la traducción de dirección, TLB, etc.
    }

    return 0;
}