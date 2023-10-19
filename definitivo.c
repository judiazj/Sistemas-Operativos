#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

int main() {
    const int SIZE = 4096;
    const char *name = "OS"; // Nombre del área de memoria compartida

    int fd; // Descriptor de archivo

    char *ptr; // Puntero al área de memoria compartida

    // Crear el área de memoria compartida
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Error en shm_open");
        return -1;
    }

    ftruncate(fd, SIZE); // Establecer el tamaño del área de memoria compartida

    // Mapear el área de memoria compartida
    ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("Error MAP_FAILED");
        return -1;
    }

    sem_t *sem_parent;
    sem_parent = sem_open("my_semaphore_parent", O_CREAT, 0666, 0); // Semáforo para el proceso padre
    sem_t *sem_child;
    sem_child = sem_open("my_semaphore_child", O_CREAT, 0666, 0); // Semáforo para el proceso hijo

    int rc = fork();

    // Proceso Hijo
    if (rc == 0) {
        while (1) {
            sem_wait(sem_child); // Esperar a que el proceso padre escriba los datos

            // Convertir la cadena de texto a mayúsculas
            for (int i = 0; ptr[i] != '\0'; i++) {
                ptr[i] = toupper(ptr[i]);
            }

            sem_post(sem_parent); // Liberar el semáforo para el proceso padre
        }
    }
    // Proceso Padre
    else {
        while (1) {
            // Recibimos la entrada del usuario por teclado
            char texto[255];
            printf("Ingrese cadena de texto (o escriba 'salir' para terminar): ");
            memset(texto, 0, sizeof(texto));
            fgets(texto, 255, stdin);

            // Si el usuario escribe "salir", terminar el programa
            if (strcmp(texto, "salir\n") == 0) {
                sem_post(sem_child); // Asegurarse de que el proceso hijo también termine
                break;
            }

            // Escribimos los bytes al área de memoria
            sprintf(ptr, "%s", texto);

            sem_post(sem_child); // Liberar el semáforo para el proceso hijo
            sem_wait(sem_parent); // Esperar a que el proceso hijo termine de convertir

            printf("Padre: Recibido desde el hijo: %s", ptr);
        }
    }

    sem_close(sem_parent); // Cerrar el semáforo del proceso padre
    sem_close(sem_child); // Cerrar el semáforo del proceso hijo
    sem_unlink("my_semaphore_parent"); // Eliminar el semáforo del proceso padre
    sem_unlink("my_semaphore_child"); // Eliminar el semáforo del proceso hijo
    shm_unlink(name); // Eliminar el área de memoria compartida

    return 0;
}
