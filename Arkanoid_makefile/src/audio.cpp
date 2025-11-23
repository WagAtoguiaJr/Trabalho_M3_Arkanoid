#include "audio.h"
#include "raylib.h"

// Arquivos de som (carregue os seus .wav/.ogg em res/)
static Sound s_paddle_collision;
static Sound s_block_collision;
static Sound s_wall_collision;
static Sound s_blockDestroy;
static Sound s_powerup;
static Sound s_life_down;
static Sound s_menu_navigate;
static Sound s_menu_select;
static Music bgm;
static bool audioInitialized = false;



void InitAudioSystem() {
    if (!audioInitialized) {
        InitAudioDevice();
        audioInitialized = true;
    }
}

void LoadAudioAssets() {
    // Chame depois de InitAudioSystem, apenas uma vez
    // Substitua os nomes pelos seus arquivos em res/
    s_paddle_collision = LoadSound("SFX/colisao_pad_SFX_.wav");
    s_block_collision = LoadSound("SFX/colisao_blco_SFX_.wav");
    s_wall_collision = LoadSound("SFX/colisao_SFX_.wav");
    s_blockDestroy = LoadSound("SFX/blco_dest_SFX_.wav");
    s_powerup = LoadSound("SFX/power_up_SFX_.wav");
    s_life_down = LoadSound("SFX/life_down_SFX_.wav");
    s_menu_navigate = LoadSound("SFX/navigate_SFX_.wav");
    s_menu_select = LoadSound("SFX/select_SFX_.wav");

    // Música de fundo opcional
    bgm = LoadMusicStream("SFX/CMS_bgs.mp3");
    PlayMusicStream(bgm);

    // Ajustes iniciais
    SetSoundVolume(s_paddle_collision, 0.6f);
    SetSoundVolume(s_block_collision, 0.6f);
    SetSoundVolume(s_wall_collision, 0.6f);
    SetSoundVolume(s_blockDestroy, 0.8f);
    SetSoundVolume(s_powerup, 0.7f);
    SetSoundVolume(s_life_down, 0.7f);
    
    SetMusicVolume(bgm, 0.5f);
    SetSoundVolume(s_menu_navigate, 0.6f);
    SetSoundVolume(s_menu_select, 0.6f);
}

void UpdateAudio() {
    // Se estiver usando Music, atualize o stream no loop principal
    UpdateMusicStream(bgm);
}

void PlayPaddleCollisionSound() {
    // PlaySound corta som em reprodução; PlaySoundMulti permite sobreposição
    PlaySound(s_paddle_collision);
}

void PlayBlockCollisionSound() {
    // PlaySound corta som em reprodução; PlaySoundMulti permite sobreposição
    PlaySound(s_block_collision);
}

void PlayWallCollisionSound() {
    // PlaySound corta som em reprodução; PlaySoundMulti permite sobreposição
    PlaySound(s_wall_collision);
}

void PlayBlockDestroySound() {
    PlaySound(s_blockDestroy);
}

void PlayPowerupSound() {
    PlaySound(s_powerup);
}

void PlayLifeDownSound() {
    PlaySound(s_life_down);
}

void PlayMenuNavigateSound() {
    PlaySound(s_menu_navigate);
}

void PlayMenuSelectSound() {
    PlaySound(s_menu_select);
}

void SetEffectsVolume(float vol) {
    // Ajusta volume mestre para efeitos (não há função direta para todos os sounds,
    // mas podemos ajustar individualmente se quiser)
    SetSoundVolume(s_paddle_collision, vol);
    SetSoundVolume(s_block_collision, vol);
    SetSoundVolume(s_wall_collision, vol);
    SetSoundVolume(s_blockDestroy, vol);
    SetSoundVolume(s_powerup, vol);
    SetSoundVolume(s_life_down, vol);
    SetSoundVolume(s_menu_navigate, vol);
    SetSoundVolume(s_menu_select, vol);
}

void SetMusicVolume(float vol) {
    SetMusicVolume(bgm, vol);
}

void UnloadAudioSystem() {
    // Descarrega assets e fecha dispositivo
    UnloadSound(s_paddle_collision);
    UnloadSound(s_block_collision);
    UnloadSound(s_wall_collision);
    UnloadSound(s_blockDestroy);
    UnloadSound(s_powerup);
    UnloadSound(s_life_down);
    UnloadSound(s_menu_navigate);
    UnloadSound(s_menu_select);
    
    UnloadMusicStream(bgm);

    if (audioInitialized) {
        CloseAudioDevice();
        audioInitialized = false;
    }
}
