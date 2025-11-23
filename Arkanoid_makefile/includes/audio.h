#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"

    // Inicializa o dispositivo de áudio e carrega assets
void InitAudioSystem();

    // Carrega efeitos e música (chame uma vez)
void LoadAudioAssets();

    // Atualiza stream de música (chamar no loop se usar Music)
void UpdateAudio();

    // Funções para tocar efeitos
void PlayPaddleCollisionSound();
void PlayBlockCollisionSound();
void PlayWallCollisionSound();
void PlayBlockDestroySound();
void PlayPowerupSound();
void PlayLifeDownSound();

    // Funções para tocar efeitos do Menu
void PlayMenuNavigateSound();
void PlayMenuSelectSound();

    // Ajustes de volume
void SetEffectsVolume(float vol); // 0.0f - 1.0f
void SetMusicVolume(float vol);

    // Descarrega tudo e fecha áudio
void UnloadAudioSystem();


#endif // AUDIO_H