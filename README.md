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

## Ejercicio 8 

### pregunta 1

para que se reproduzca el sonido el jugador debe presionar la letra p

### pregunta 2

En este programa, las funciones principales para reproducir el sonido son:
- `play_audio`: carga el archivo de audio y configura el dispositivo.
- `AudioCallback`: realiza la reproducción del sonido copiando fragmentos al búfer.
- `SDL_Delay`: introduce una espera en el programa mientras el sonido se reproduce.

### pregunta 3

el principal problema es que se usa una funcion delay que espera que el audio termine, lo que hace que se pare todo el juego en cuanto se presiona la tecla p 

### pregunta 4

la solucion que se nos ocurre es hacer uso de hilos, ya que estos mismos permieten que se ejecuten tareas de forma simultanea para evitar que se pause el juego

### Solución con Semáforos

Para resolver esto sin usar otra API de SDL2, podemos utilizar un semáforo para coordinar que la función `play_audio` sea ejecutada cuando ocurra el evento `SDL_KEYUP` para la tecla `SDLK_p`. El semáforo sincroniza el evento con la reproducción de audio sin bloquear el flujo principal.

#### Implementación del Semáforo

´´´c
      #include <stdio.h>
      #include <stdbool.h>
      #include <SDL.h>
      #include "constants.h"
      #include <SDL_audio.h>
      
      typedef struct {
          Uint8* audioData; // Pointer to audio data    
          Uint32 audioLength; // Length of audio data in bytes    
          Uint32 audioPosition; // Current position in audio data    
          SDL_bool audioFinished; //flag set to True whether audio playback has finished
      } AudioContext;
      
      SDL_sem* audioSemaphore;
      AudioContext audioContext;
      SDL_AudioSpec audioSpec;
      SDL_AudioDeviceID audioDevice;
      
      void AudioCallback(void* userdata, Uint8* stream, int len) {
          AudioContext* audioContext = (AudioContext*)userdata;
      
          if (audioContext->audioPosition >= audioContext->audioLength) {
              audioContext->audioFinished = SDL_TRUE;
              return;
          }
      
          int remainingBytes = audioContext->audioLength - audioContext->audioPosition;
          int bytesToCopy = (len < remainingBytes) ? len : remainingBytes;
      
          SDL_memcpy(stream, audioContext->audioData + audioContext->audioPosition, bytesToCopy);
          audioContext->audioPosition += bytesToCopy;
      }
      
      int audio_thread(void* arg) {
          while (1) {
              SDL_SemWait(audioSemaphore); // Espera a que se desbloquee el semáforo
              audioContext.audioPosition = 0;
              audioContext.audioFinished = SDL_FALSE;
      
              if (SDL_LoadWAV("tap.wav", &audioSpec, &audioContext.audioData, &audioContext.audioLength) != NULL) {
                  SDL_PauseAudioDevice(audioDevice, 0);
                  while (!audioContext.audioFinished) {
                      SDL_Delay(100);
                  }
                  SDL_FreeWAV(audioContext.audioData);
              }
              else {
                  printf("Unable to load WAV file: %s\n", SDL_GetError());
              }
          }
          return 0;
      }
      void initialize_audio() {
          SDL_memset(&audioSpec, 0, sizeof(audioSpec));
          audioSpec.freq = 44100;
          audioSpec.format = AUDIO_S16SYS;
          audioSpec.channels = 1;
          audioSpec.samples = 2048;
          audioSpec.callback = AudioCallback;
          audioSpec.userdata = &audioContext;
      
          audioDevice = SDL_OpenAudioDevice(NULL, 0, &audioSpec, NULL, 0);
          if (audioDevice == 0) {
              printf("Unable to open audio device: %s\n", SDL_GetError());
          }
      }
      
      
      void play_audio(void) {
          static uint8_t isaudioDeviceInit = 0;
          static SDL_AudioSpec audioSpec;
          static SDL_AudioDeviceID audioDevice = 0;
          static AudioContext audioContext;
      
          if (isaudioDeviceInit == 0) {
              /*
                    audioSpec.freq = 44100;
                    audioSpec.format = AUDIO_S16SYS;
                    audioSpec.channels = 1;
                    audioSpec.samples = 2048;        */
      
              audioSpec.callback = AudioCallback;
              audioSpec.userdata = &audioContext;
      
              audioDevice = SDL_OpenAudioDevice(NULL, 0, &audioSpec, NULL, 0);
              if (audioDevice == 0) {
                  printf("Unable to open audio device: %s\n", SDL_GetError());
                  return 1;
              }
              isaudioDeviceInit = 1;
          }
      
          audioContext.audioPosition = 0;
          audioContext.audioFinished = SDL_FALSE;
          if (SDL_LoadWAV("tap.wav", &audioSpec, &audioContext.audioData, &audioContext.audioLength) != NULL) {
              SDL_PauseAudioDevice(audioDevice, 0); // Start audio playback    
          }
          else {
              printf("Unable to load WAV file: %s\n", SDL_GetError());
          }
          while (audioContext.audioFinished != SDL_TRUE) {
              SDL_Delay(100);
          }
      
          printf("Audio finished\n");
          SDL_CloseAudio(audioDevice);
          SDL_FreeWAV(audioContext.audioData); // Free the loaded WAV data
      }
      
      
      int game_is_running = false;
      SDL_Window* window = NULL;
      SDL_Renderer* renderer = NULL;
      int last_frame_time = 0;
      
      struct game_object {
          float x;
          float y;
          float width;
          float height;
          float vel_x;
          float vel_y;
      } ball, paddle;
      
      int initialize_window(void) {
          if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
              fprintf(stderr, "Error initializing SDL.\n");
              return false;
          }
          window = SDL_CreateWindow(
              NULL,
              SDL_WINDOWPOS_CENTERED,
              SDL_WINDOWPOS_CENTERED,
              WINDOW_WIDTH,
              WINDOW_HEIGHT,
              SDL_WINDOW_BORDERLESS
          );
          if (!window) {
              fprintf(stderr, "Error creating SDL Window.\n");
              return false;
          }
          renderer = SDL_CreateRenderer(window, -1, 0);
          if (!renderer) {
              fprintf(stderr, "Error creating SDL Renderer.\n");
              return false;
          }
          return true;
      }
      
      void process_input(void) {
          SDL_Event event;
          SDL_PollEvent(&event);
          switch (event.type) {
          case SDL_QUIT:
              game_is_running = false;
              break;
          case SDL_KEYDOWN:
              if (event.key.keysym.sym == SDLK_ESCAPE)
                  game_is_running = false;
              if (event.key.keysym.sym == SDLK_LEFT)
                  paddle.vel_x = -400;
              if (event.key.keysym.sym == SDLK_RIGHT)
                  paddle.vel_x = +400;
              break;
          case SDL_KEYUP:
              if (event.key.keysym.sym == SDLK_LEFT)
                  paddle.vel_x = 0;
              if (event.key.keysym.sym == SDLK_RIGHT)
                  paddle.vel_x = 0;
              if (event.key.keysym.sym == SDLK_p) {
                  SDL_SemPost(audioSemaphore); // Desbloquea el semáforo para reproducir el audio
              }
      
              break;
          }
      }
      
      void setup(void) {
          // Initialize values for the the ball object    
          ball.width = 15;
          ball.height = 15;
          ball.x = 20;
          ball.y = 20;
          ball.vel_x = 300;
          ball.vel_y = 300;
      
          // Initialize the values for the paddle object    
          paddle.width = 100;
          paddle.height = 20;
          paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
          paddle.y = WINDOW_HEIGHT - 40;
          paddle.vel_x = 0;
          paddle.vel_y = 0;
      }
      
      void update(void) {
          // Calculate how much we have to wait until we reach the target frame time    
          int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
      
          // Only delay if we are too fast too update this frame    
          if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
              SDL_Delay(time_to_wait);
      
          // Get a delta time factor converted to seconds to be used to update my objects    
          float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;
      
          // Store the milliseconds of the current frame    
          last_frame_time = SDL_GetTicks();
      
          // update ball and paddle position    
          ball.x += ball.vel_x * delta_time;
          ball.y += ball.vel_y * delta_time;
          paddle.x += paddle.vel_x * delta_time;
          paddle.y += paddle.vel_y * delta_time;
      
          // Check for ball collision with the walls    
          if (ball.x <= 0 || ball.x + ball.width >= WINDOW_WIDTH)
              ball.vel_x = -ball.vel_x;
          if (ball.y < 0)
              ball.vel_y = -ball.vel_y;
      
          // Check for ball collision with the paddle    
          if (ball.y + ball.height >= paddle.y && ball.x + ball.width >= paddle.x && ball.x <= paddle.x + paddle.width)
              ball.vel_y = -ball.vel_y;
      
          // Prevent paddle from moving outside the boundaries of the window    
          if (paddle.x <= 0)
              paddle.x = 0;
          if (paddle.x >= WINDOW_WIDTH - paddle.width)
              paddle.x = WINDOW_WIDTH - paddle.width;
      
          // Check for game over    
          if (ball.y + ball.height > WINDOW_HEIGHT) {
              ball.x = WINDOW_WIDTH / 2;
              ball.y = 0;
          }
      }
      
      void render(void) {
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          SDL_RenderClear(renderer);
      
          // Draw a rectangle for the ball object    
          SDL_Rect ball_rect = {
          (int)ball.x,
          (int)ball.y,
          (int)ball.width,
          (int)ball.height
          };
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
          SDL_RenderFillRect(renderer, &ball_rect);
      
          // Draw a rectangle for the paddle object    
          SDL_Rect paddle_rect = {
          (int)paddle.x,
          (int)paddle.y,
          (int)paddle.width,
          (int)paddle.height
          };
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
          SDL_RenderFillRect(renderer, &paddle_rect);
      
          SDL_RenderPresent(renderer);
      }
      
      void destroy_window(void) {
          SDL_DestroyRenderer(renderer);
          SDL_DestroyWindow(window);
          SDL_Quit();
      }
      
      int main(int argc, char* args[]) {
          SDL_Init(SDL_INIT_EVERYTHING);
          audioSemaphore = SDL_CreateSemaphore(0);
          initialize_audio();
      
          SDL_Thread* threadID = SDL_CreateThread(audio_thread, "AudioThread", NULL);
      
          // Configuración del juego
          game_is_running = initialize_window();
          setup();
      
          while (game_is_running) {
              process_input();
      
              // Reproducción de audio en segundo plano
              update();
              render();
          }
      
          SDL_DestroySemaphore(audioSemaphore);
          SDL_CloseAudioDevice(audioDevice);
          destroy_window();
          return 0;
      }

  ´´´
  El semáforo asegura que la reproducción del sonido no interfiera con el flujo del juego y se ejecute cuando se produzca el evento, resolviendo el problema de sincronización sin bloquear el juego.

