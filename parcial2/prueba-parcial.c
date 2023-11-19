#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int decimal_a_binario(int decimal){
    int binario[32];
    int i = 0;
    
    while (i < 32) {
        binario[i] = decimal & 1;
        decimal = decimal >> 1;
        i++;
    }
    // for (int j = i - 1; j >= 0; j--) {
    //     printf("%d", binario[j]);
    // }
    return binario;
}

int binario_a_decimal(int binario){
    int decimal = 0;
    int decimales[31];
    for(int i=0; i < 32; i++){
        if(binario < 0){
            decimales[i] = 0;
            continue;
        }
        decimales[i] = binario % 10;
        binario /= 10;
    }

    for(int j=0; j < 32; j++ ){
        if(decimales[j] == 1){
            decimal = decimal + pow(2, j);
        }
    }

    return decimal;
}

int main() {
    char input[100];
    int direccion;
    while (1) {
        printf("Ingrese la dirección virtual: ");
        scanf("%s", input);

        // Verificar si la entrada es 's' o 'S' para salir
        if (input[0] == 's' || input[0] == 'S') {
            printf("Saliendo del programa...\n");
            break;
        } 

        // Convertir la entrada a entero
        direccion = atoi(input);
        printf("Pagina: %d \n", (int) (direccion / 4096));
        printf("Desplazamiento: %d \n", (int) (direccion % 4096));

        int paginaBinario = decimal_a_binario((int) (direccion / 4096));
        int desplazamientoBinario = decimal_a_binario((int) (direccion % 4096));
        
        printf("Página en binario: %d \n", paginaBinario);
        printf("Desplazamiento en binario: %d \n", desplazamientoBinario);
    }
    return 0;
}