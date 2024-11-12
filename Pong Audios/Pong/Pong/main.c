#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_audio.h>
#include "constants.h"

typedef struct {
    Uint8* audioData;
    Uint32 audioLength;
    Uint32 audioPosition;
    SDL_bool audioFinished;
} AudioContext;
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

typedef struct {
    float x;
    float y;
    float width;
    float height;
    bool destroyed; // Si el ladrillo está destruido o no
} Brick;

#define NUM_ROWS 5
#define NUM_COLS 10
#define BRICK_WIDTH (WINDOW_WIDTH / NUM_COLS)
#define BRICK_HEIGHT 30

Brick bricks[NUM_ROWS][NUM_COLS];


#define MAX_AUDIO_EVENTS 10

AudioContext wallHitAudio, paddleHitAudio, loseAudio, brickHitAudio, sonidoganar;
SDL_AudioSpec audioSpec;
SDL_AudioDeviceID audioDevice;
SDL_sem* audioSemaphore;
SDL_mutex* audioQueueMutex;

void AudioCallback(void* userdata, Uint8* stream, int len) {
    AudioContext* audioContext = (AudioContext*)userdata;
    if (audioContext->audioPosition >= audioContext->audioLength) {
        audioContext->audioFinished = SDL_TRUE;  // Marca el audio como terminado solo si se acabó el archivo
        SDL_memset(stream, 0, len); // Evita ruido llenando el stream con silencio
        return;
    }

    int remainingBytes = audioContext->audioLength - audioContext->audioPosition;
    int bytesToCopy = (len < remainingBytes) ? len : remainingBytes;

    SDL_memcpy(stream, audioContext->audioData + audioContext->audioPosition, bytesToCopy);
    audioContext->audioPosition += bytesToCopy;
}

void load_audio(const char* filename, AudioContext* audioContext) {
    if (SDL_LoadWAV(filename, &audioSpec, &audioContext->audioData, &audioContext->audioLength) == NULL) {
        printf("Error loading %s: %s\n", filename, SDL_GetError());
    }
    else {
        audioContext->audioPosition = 0;
        audioContext->audioFinished = SDL_FALSE;
    }
}

void setup_audio_files() {
    load_audio("Sound1.wav", &wallHitAudio);
    load_audio("Sound4.wav", &paddleHitAudio);
    load_audio("Sound3.wav", &loseAudio);
    load_audio("Sound2.wav", &brickHitAudio);
    load_audio("Sound5.wav", &sonidoganar);
}


void play_audio(AudioContext* context) {
    SDL_LockAudioDevice(audioDevice);
    context->audioPosition = 0;       // Reinicia la posición de audio al inicio del archivo
    context->audioFinished = SDL_FALSE; // Marca el audio como no terminado
    audioSpec.userdata = context;     // Establece el contexto de audio
    SDL_UnlockAudioDevice(audioDevice);
    SDL_PauseAudioDevice(audioDevice, 0); // Reanuda el audio si está en pausa
}




int audio_thread(void* arg) {
    while (1) {
        SDL_SemWait(audioSemaphore);
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
    audioSpec.userdata = &wallHitAudio; // Se inicia con wallHitAudio

    audioDevice = SDL_OpenAudioDevice(NULL, 0, &audioSpec, NULL, 0);
    if (audioDevice == 0) {
        printf("Unable to open audio device: %s\n", SDL_GetError());
    }

    audioQueueMutex = SDL_CreateMutex();
    audioSemaphore = SDL_CreateSemaphore(0);

    SDL_CreateThread(audio_thread, "AudioThread", NULL);
    setup_audio_files();  // Cargar los archivos de audio una sola vez
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
            paddle.vel_x = -700;
        if (event.key.keysym.sym == SDLK_RIGHT)
            paddle.vel_x = +700;
        break;
    case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_LEFT)
            paddle.vel_x = 0;
        if (event.key.keysym.sym == SDLK_RIGHT)
            paddle.vel_x = 0;
        break;
    }
}
void setup(void) {
    // Inicialización de valores de la pelota
    ball.width = 15;
    ball.height = 15;
    ball.x = (WINDOW_WIDTH / 2) - (ball.width / 2); // Centrado en la pantalla
    ball.y = paddle.y - ball.height; // Justo encima de la paleta
    ball.vel_x = 300;
    ball.vel_y = 300;

    // Inicialización de valores de la paleta
    paddle.width = 100;
    paddle.height = 20;
    paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
    paddle.y = WINDOW_HEIGHT - 40;
    paddle.vel_x = 0;
    paddle.vel_y = 0;

    // Inicializar ladrillos 
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            bricks[row][col].x = col * BRICK_WIDTH;
            bricks[row][col].y = row * BRICK_HEIGHT;
            bricks[row][col].width = BRICK_WIDTH;
            bricks[row][col].height = BRICK_HEIGHT;
            bricks[row][col].destroyed = false;
        }
    }
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Dibujar un rectángulo para el objeto pelota
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    // Dibujar un rectángulo para el objeto paleta
    SDL_Rect paddle_rect = {
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.width,
        (int)paddle.height
    };

    // Dibujar los ladrillos
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (!bricks[row][col].destroyed) {
                SDL_Rect brick_rect = {
                    (int)bricks[row][col].x,
                    (int)bricks[row][col].y,
                    (int)bricks[row][col].width,
                    (int)bricks[row][col].height
                };
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rojo para los ladrillos
                SDL_RenderFillRect(renderer, &brick_rect);
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);

    SDL_RenderPresent(renderer);
}


void update(void) 
{
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;
    last_frame_time = SDL_GetTicks();

    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;
    paddle.x += paddle.vel_x * delta_time;

    if (ball.x <= 0 || ball.x + ball.width >= WINDOW_WIDTH) {
        ball.vel_x = -ball.vel_x;
        play_audio(&wallHitAudio);
    }
    if (ball.y < 0) {
        ball.vel_y = -ball.vel_y;
        play_audio(&wallHitAudio);
    }

    if (ball.y + ball.height >= paddle.y && ball.x + ball.width >= paddle.x && ball.x <= paddle.x + paddle.width) {
        ball.vel_y = -ball.vel_y;
        ball.vel_x *= 1.05;
        ball.vel_y *= 1.05;
        play_audio(&paddleHitAudio);
    }
    // Colisiones con los ladrillos
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (!bricks[row][col].destroyed) {
                if (ball.x + ball.width > bricks[row][col].x &&
                    ball.x < bricks[row][col].x + bricks[row][col].width &&
                    ball.y + ball.height > bricks[row][col].y &&
                    ball.y < bricks[row][col].y + bricks[row][col].height) {

                    // Marcar el ladrillo como destruido
                    bricks[row][col].destroyed = true;

                    // Rebotar la pelota
                    ball.vel_y = -ball.vel_y;
                    play_audio(&wallHitAudio); // Sonido de colisión con ladrillo
                }
            }
        }
    }

    if (ball.y + ball.height > WINDOW_HEIGHT) {
        play_audio(&loseAudio);
        ball.x = WINDOW_WIDTH / 2;
        ball.y = WINDOW_HEIGHT / 2;
        ball.vel_x = 300;
        ball.vel_y = 300;
        for (int row = 0; row < NUM_ROWS; row++) {
            for (int col = 0; col < NUM_COLS; col++) {
                bricks[row][col].destroyed = false;
            }
        }
    }
    
}
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

void cleanup_audio() {
    SDL_FreeWAV(wallHitAudio.audioData);
    SDL_FreeWAV(paddleHitAudio.audioData);
    SDL_FreeWAV(loseAudio.audioData);
    SDL_CloseAudioDevice(audioDevice);
    SDL_DestroySemaphore(audioSemaphore);
    SDL_DestroyMutex(audioQueueMutex);
}
void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return -1;
    }

    initialize_audio();

    if (!initialize_window()) {
        fprintf(stderr, "Failed to initialize window.\n");
        return -1;
    }

    setup();

    // Configura el juego como "en ejecución" para que el bucle funcione
    game_is_running = true;

    while (game_is_running) {
        process_input();
        update();
        render();
    }

    cleanup_audio();
    destroy_window();
    SDL_Quit();
    return 0;
}

