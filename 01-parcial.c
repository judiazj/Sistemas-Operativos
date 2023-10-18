#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/mman.h>


// Julian David Diaz Jaramillo
// Isabel Cristina Ramirez Ramirez

int main(int argc, char *args[]) {

    sem_t sem;
    sem_init(&sem, 0, 1); // Se crea el semaforo inicializado a 1
    int rc = fork();

    //Proceso Hijo
    if(rc == 0){
        //Tamaño en bytes del area de memoria compartida    
        const int SIZE = 4096;
        const char *name = "OS"; //nombre del area de memoria compartida

        int fd; // Descriptor de archivo

        char *ptr; // Puntero al area de memoria compartida

        // Se abre el área de memoria compartida 
        fd = shm_open(name, O_RDONLY, 0666);
        if (fd == -1) {
            perror("Error en shm_open");
            return -1;
        }

        // Se mapea el área de memoria compartida 
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
        if (ptr == MAP_FAILED) {
            perror("Error MAP_FAILED");
            return -1;
        }

        sem_wait(&sem); // Obtenemos el semaforo

        // Se leen los bytes que están en el área de
        // memoria compartida
        printf("%s\n",(char *)ptr);
        int i;
        for (i = 0; ptr[i] != '-'; i++) {
            ptr[i] = toupper(ptr[i]);
        }

        sem_post(&sem); // Liberamos el semaforo

        // Este proceso se desengancha del área de memoria compartida
        shm_unlink(name);
    }
    //Proceso Padre
    else{
        while (1) {
            //Tamaño en bytes del area de memoria compartida    
            const int SIZE = 4096;
            const char *name = "OS"; //nombre del area de memoria compartida

            //Recibimos la entrada del usuario por teclado
            char texto[255];
            printf("Ingrese cadena de texto:");
            memset(texto, '-', sizeof(texto));
            fgets(texto, 255, stdin);

            const char *m_0 = texto; //Cadena que se escribira en el area de memoria compartida

            int fd; // Descriptor de archivo

            char *ptr; // Puntero al area de memoria compartida

            // Se crea el area de memoria compartida
            fd = shm_open(name, O_CREAT | O_RDWR, 0666);
            if (fd == -1) {
                perror("Error en shm_open");
                return(-1);
            }

            ftruncate(fd, SIZE); // Se establece el tamaño del area de memoria compartida

            // Se mapea el area de memoria compartida
            ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            if (ptr == MAP_FAILED) {
                perror("Error MAP_FAILED");
                return (-1);
            }
            
            sem_wait(&sem); //Obtenemos el semaforo

            sprintf(ptr,"%s",m_0); // Se escriben los bytes al area de memoria
            ptr += strlen(m_0); // Se actualiza el apuntador

            sem_post(&sem); // Liberar el semaforo

            printf("%s", ptr);
        }
    }

    return 0;
}

