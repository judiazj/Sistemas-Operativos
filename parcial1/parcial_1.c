#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Julian David Diaz Jaramillo
// Isabel Cristina Ramirez Ramirez

int main(int argc, char *args[]) {
    
    //Descriptores de lectura y escritura
    int fildes1[2], fildes2[2];

    // se crean tuberias sin nombre
    if (pipe(fildes1) < 0 || pipe(fildes2) < 0) {
        perror("Error al crear tuberia\n");
        return(1);
    }

    int rc = fork();

    if(rc < 0){
        //Fallo en la creacion del proceso hijo
        printf("Fallo en creacion proceso hijo\n");
        exit(1);
    }
    //Proceso hijo
    else if (rc == 0) {
        while (1) {
            char minuscula[255];
            char mayuscula[255];

            //Leemos el dato proveniente del proceso padre a traves de la tuberia
            read(fildes1[0], minuscula, sizeof(minuscula));
            memset(mayuscula, '-', sizeof(mayuscula));

            //Pasamos el texto de miniscula a mayuscula con la funcion toupper
            for (int i = 0; minuscula[i] != '-'; i++) {
                mayuscula[i] = toupper(minuscula[i]);
            }

            //Escribimos la cadena de caracteres en la tuberia
            write(fildes2[1], mayuscula, sizeof(mayuscula));
        }
        
    } else {
         //Proceso padre 
         //dato que sale de las tuberias
        while (1) {

            char texto[255];
            printf("Ingrese cadena de texto:");
            memset(texto, '-', sizeof(texto));
            fgets(texto, 255, stdin);

            //Proceso Padre escribe en la tuberia y se bloquea
            write(fildes1[1], texto, sizeof(texto));

            char resultado_final[255] = { 0 };
            memset(resultado_final, '-', sizeof(resultado_final));
            read(fildes2[0], resultado_final, sizeof(resultado_final));
            printf("%s", resultado_final);
        }
       
    }

    //Cerrar descriptores
    close(fildes1[0]);
    close(fildes1[1]);
    close(fildes2[0]);
    close(fildes2[1]);
    return 0;
}