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
    for (int j = i - 1; j >= 0; j--) {
        printf("%d", binario[j]);
    }
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
    int direccion;
    printf("Direccion: ");
    scanf("%d", &direccion);
    // printf("La direccion es: %d", direccion);
    // decimal_a_binario(direccion);
    printf("El numero de binario a decimal es: %d", binario_a_decimal(direccion));
}