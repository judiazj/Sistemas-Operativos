#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void decimal_a_binario(int decimal, int* binario) {
    int i = 0;
    while (i < 32) {
        binario[i] = decimal & 1;
        decimal = decimal >> 1;
        i++;
    }
}

int binario_a_decimal(int* binario) {
    int decimal = 0;
    for(int j = 0; j < 32; j++ ){
        if(binario[j] == 1){
            decimal += (int)pow(2, j);
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

        // Verificar si la entrada es 's' o 'S' para salir
        if (input[0] == 's' || input[0] == 'S') {
            printf("Saliendo del programa...\n");
            break;
        } 

        // Convertir la entrada a entero
        direccion = atoi(input);
        printf("Direccion: %d \n", direccion);

        // Convertir decimal a binario
        decimal_a_binario(direccion, binario);
        printf("La direccion en binario es: ");
        for(int i=31; i >= 0; i--){
            printf("%d", binario[i]);
        }
        printf("\n");

        // Aquí puedes continuar con el resto de la lógica para el programa
        // Realizar la traducción de dirección, TLB, etc.
    }
    return 0;
}
