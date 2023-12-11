#ifndef __sound
#define __sound
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// use mainplay("test.wav"); to play sounds and include this file into wherever you need it

Mix_Chunk *wave = NULL;

void sound_loop_then_quit() {
    if (Mix_Playing(-1))
        return;
  //  printf("Done audio\n");
    Mix_FreeChunk(wave);
    Mix_CloseAudio();
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
    #endif
   // printf("Shutting down\n");
//    REPORT_RESULT(1);
}

int mainplay(char* test){
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return -1;
        #ifdef __EMSCRIPTEN__
    int const frequency = EM_ASM_INT_V({
        var context;
        try {
            context = new AudioContext();
        } catch (e) {
            context = new webkitAudioContext(); // safari only
        }
        return context.sampleRate;
    });
    #endif
#ifdef __EMSCRIPTEN__
    if(Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
#else
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
#endif
        return -1;
    wave = Mix_LoadWAV(test);
    if (wave == NULL)
        return -1;
    if (Mix_PlayChannel(-1, wave, 0) == -1)
        return -1;
    // Ensure that the test gives an error if OGG support was not compiled into SDL2_Mixer. See #7879
    if (Mix_Init(MIX_INIT_OGG) == -1)
        return -1;
 //   printf("Starting sound play loop\n");
    sound_loop_then_quit();
//    emscripten_set_main_loop(sound_loop_then_quit, 0, 1);
    return 0;
}
#endif
