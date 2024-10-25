[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/XglgMq0o)
# Documentación del Proyecto
---
Estudiante:  
Id:  
---
# Documentacion de ejercicios


## Ejercicio 5

En este ejercicio se tiene un codigo el cual el ejecutarse no pasa nada
![image](https://github.com/user-attachments/assets/db569554-458c-4d45-893c-0cc8cba7eba7)

y pues al analizar el codigo nos damos cuenta que es porque el hilo principal se cierra antes de que los otros hilos puedan hacer algo, lo que se puede hacer es utilizar una funcion join para que el programa espere que se ejecuten ambos hilos antes de cerrarse. 

## Ejercicio 6 

en este caso se presenta la solucion al problema anterior y al ejecutarlo se ve así

![image](https://github.com/user-attachments/assets/eb27f4b8-fb32-4d21-9c8a-de495375c473)

se nota que primero ejecuta todas las a y luego todas las b, por lo que efectivamente para esperar a que un hilo termine se debe de usar la funcion join 
en el fragmento de codigo que sigue 

```c
  pthread_create(&threadID1,NULL,&imprime, &threadParam1);
  pthread_join(threadID1,NULL);
  pthread_create(&threadID2,NULL,&imprime, &threadParam2);
  pthread_join(threadID2,NULL);
```

para este caso primero se crea un hilo y se epera que este termine para crear el otro y esperar que el otro termine es decir se ejecuta uno despues del otro, mientras que en el otro fragmento se ve asi:

```c
  pthread_create(&threadID1,NULL,&imprime, &threadParam1);
  pthread_create(&threadID2,NULL,&imprime, &threadParam2);
  pthread_join(threadID1,NULL);
  pthread_join(threadID2,NULL);
```

en este caso primero se crean ambos hilos y luego se ejecutan ambos casi de forma simultanea, se espera a que ambos terminen antes de terminar la ejecucion del programa.

## Ejercicio 7 

Aquí tienes la información solicitada sobre SDL2, junto con ejemplos en Markdown.

### 1. ¿Cómo se crea un hilo en SDL2?

En SDL2, se puede crear un hilo utilizando la función `SDL_CreateThread()`. Esta función toma como argumentos el nombre de la función que se ejecutará en el hilo, un puntero a datos que se pasarán a esa función y un puntero a la variable que almacenará el identificador del hilo.

### Ejemplo de creación de un hilo en SDL2

```c
#include <SDL.h>
#include <stdio.h>

int threadFunction(void *data) {
    const char *message = (const char *)data;
    for (int i = 0; i < 5; ++i) {
        printf("%s %d\n", message, i);
        SDL_Delay(100);  // Espera 100 ms
    }
    return 0;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Thread *thread = SDL_CreateThread(threadFunction, "MyThread", "Hello from thread!");

    SDL_WaitThread(thread, NULL);  // Espera a que el hilo termine
    SDL_Quit();
    return 0;
}
```

### 2. Equivalente de `join` en la API de SDL2

El equivalente de `pthread_join` en SDL2 es `SDL_WaitThread()`, que espera a que un hilo creado con `SDL_CreateThread()` termine su ejecución.

### Ejemplo de uso de `SDL_WaitThread`

```c
#include <SDL.h>
#include <stdio.h>

int threadFunction(void *data) {
    const char *message = (const char *)data;
    for (int i = 0; i < 5; ++i) {
        printf("%s %d\n", message, i);
        SDL_Delay(100);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Thread *thread = SDL_CreateThread(threadFunction, "MyThread", "Hello from thread!");

    SDL_WaitThread(thread, NULL);  // Espera a que el hilo termine
    SDL_Quit();
    return 0;
}
```

### 3. Semáforos en SDL2

Los semáforos en SDL2 se utilizan para controlar el acceso a recursos compartidos y evitar condiciones de carrera, donde dos o más hilos intentan acceder a los mismos datos simultáneamente.

### Ejemplo de semáforo para evitar una condición de carrera

```c
#include <SDL.h>
#include <stdio.h>

SDL_sem *semaphore;

void *threadFunction(void *data) {
    for (int i = 0; i < 5; ++i) {
        SDL_SemWait(semaphore);  // Espera el semáforo
        printf("Thread %d accessing shared resource\n", *(int *)data);
        SDL_Delay(100);  // Simular trabajo
        SDL_SemPost(semaphore);  // Libera el semáforo
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    semaphore = SDL_CreateSemaphore(1);  // Crea un semáforo binario

    int threadData1 = 1, threadData2 = 2;
    SDL_Thread *thread1 = SDL_CreateThread(threadFunction, "Thread1", &threadData1);
    SDL_Thread *thread2 = SDL_CreateThread(threadFunction, "Thread2", &threadData2);

    SDL_WaitThread(thread1, NULL);
    SDL_WaitThread(thread2, NULL);

    SDL_DestroySemaphore(semaphore);
    SDL_Quit();
    return 0;
}
```

### 4. Sincronizar dos hilos con un semáforo

El siguiente ejemplo utiliza un semáforo para sincronizar el acceso a un recurso compartido entre dos hilos.

```c
#include <SDL.h>
#include <stdio.h>

SDL_sem *semaphore;

void *threadFunction(void *data) {
    for (int i = 0; i < 5; ++i) {
        SDL_SemWait(semaphore);  // Espera el semáforo
        printf("Thread %d is running\n", *(int *)data);
        SDL_Delay(100);  // Simula trabajo
        SDL_SemPost(semaphore);  // Libera el semáforo
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    semaphore = SDL_CreateSemaphore(1);  // Crea un semáforo binario

    int threadData1 = 1, threadData2 = 2;
    SDL_Thread *thread1 = SDL_CreateThread(threadFunction, "Thread1", &threadData1);
    SDL_Thread *thread2 = SDL_CreateThread(threadFunction, "Thread2", &threadData2);

    SDL_WaitThread(thread1, NULL);
    SDL_WaitThread(thread2, NULL);

    SDL_DestroySemaphore(semaphore);
    SDL_Quit();
    return 0;
}
```

### ¿Qué es una condición de carrera?

Una **condición de carrera** ocurre cuando dos o más hilos acceden a recursos compartidos (como variables o estructuras de datos) al mismo tiempo, y el resultado de su ejecución depende del orden en que se ejecutan. Esto puede llevar a resultados inesperados y errores en el programa. Los semáforos y otros mecanismos de sincronización son esenciales para evitar estas situaciones.

