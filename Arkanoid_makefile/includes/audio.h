#pragma once

#include "raylib.h"

void InitAudioSystem();

void LoadAudioAssets();

void UpdateAudio();

void PlayPaddleCollisionSound();
void PlayBlockCollisionSound();
void PlayWallCollisionSound();
void PlayBlockDestroySound();
void PlayPowerupSound();
void PlayLifeDownSound();

void PlayMenuNavigateSound();
void PlayMenuSelectSound();

void SetEffectsVolume(float vol); // 0.0f - 1.0f
void SetMusicVolume(float vol);

void UnloadAudioSystem();