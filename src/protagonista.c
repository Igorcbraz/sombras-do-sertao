#include "headers/protagonista.h"
#include "headers/helper.h"
#include "headers/enemies.h"
#include "headers/colision.h"
#include "headers/sound.h"
#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

struct BulletProtagonista bullets_protagonista[BULLETS_PROTAGONISTA_COUNT];
struct Protagonista *protagonista;

void setupProtagonista(int stage) {
  if (!protagonista) {
    protagonista = malloc(sizeof(struct Protagonista));
    protagonista->width = 250;
    protagonista->height = 330;
    protagonista->speed = 20;
    protagonista->direction = 1;
    protagonista->lives = 3;
    protagonista->score = 0;
    protagonista->stageX = 20;
    protagonista->estagioAtual = 4; 
    protagonista->last_shoot = 0;
    protagonista->image = al_load_bitmap("assets/images/characters/protagonista.png");
    protagonista->image_bullet = al_load_bitmap("assets/images/addons/ammo.png");
  }
  if (stage > protagonista->estagioAtual) {
    protagonista->estagioAtual = stage;
  }
  printf("Stage: %d\n", protagonista->estagioAtual);

  protagonista->x = 20;
  protagonista->y = HEIGHT_SCREEN / 2;
  protagonista->bullets = 3;
}

void drawProtagonista() {
  al_draw_bitmap(protagonista->image, protagonista->x, protagonista->y, 0);
}

void moveProtagonista() {
  if (al_key_down(&GAME_INFO->key_state, ALLEGRO_KEY_UP) || al_key_down(&GAME_INFO->key_state, ALLEGRO_KEY_W)) {
    if (protagonista->y + protagonista->height <= HEIGHT_SCREEN - protagonista->height) {
      return;
    }

    protagonista->y -= protagonista->speed;
  }
  if (al_key_down(&GAME_INFO->key_state, ALLEGRO_KEY_DOWN) || al_key_down(&GAME_INFO->key_state, ALLEGRO_KEY_S)) {
    if (protagonista->y + protagonista->height >= HEIGHT_SCREEN) {
      return;
    }

    protagonista->y += protagonista->speed;
  }
  if (al_key_down(&GAME_INFO->key_state, ALLEGRO_KEY_LEFT) || al_key_down(&GAME_INFO->key_state, ALLEGRO_KEY_A)) {
    protagonista->x -= protagonista->speed;
    protagonista->stageX -= protagonista->speed;
  }
  if (al_key_down(&GAME_INFO->key_state, ALLEGRO_KEY_RIGHT) || al_key_down(&GAME_INFO->key_state, ALLEGRO_KEY_D)) {
    protagonista->x += protagonista->speed;
    protagonista->stageX += protagonista->speed;
  }
}

void setupBulletsProtagonista() {
  for (int i = 0; i < BULLETS_PROTAGONISTA_COUNT; i++) {
    bullets_protagonista[i].x = 0;
    bullets_protagonista[i].y = 0;
    bullets_protagonista[i].width = 32;
    bullets_protagonista[i].height = 32;
    bullets_protagonista[i].speed = 50;
    bullets_protagonista[i].active = false;
    bullets_protagonista[i].image = al_load_bitmap("assets/images/addons/municao_revolver.png");
  }
}

void shootProtagonista() {
  double current_time = al_get_time();

  if (al_key_down(&GAME_INFO->key_state, ALLEGRO_KEY_SPACE) && 
      current_time - protagonista->last_shoot >= SHOOT_DELAY && 
      protagonista->bullets > 0
    ) {
    for (int i = 0; i < BULLETS_PROTAGONISTA_COUNT; i++) {
      if (!bullets_protagonista[i].active) {
        bullets_protagonista[i].active = true;
        bullets_protagonista[i].x = protagonista->x + protagonista->width;
        bullets_protagonista[i].y = protagonista->y + protagonista->height / 2 - 70;

        protagonista->last_shoot = current_time;
        protagonista->bullets--;

        playSound(SHOOT);
        break;
      }
    }
  }

  for (int i = 0; i < BULLETS_PROTAGONISTA_COUNT; i++) {
    if (!bullets_protagonista[i].active) continue;

    bool colision = false;

    bullets_protagonista[i].x += bullets_protagonista[i].speed;

    al_draw_bitmap(bullets_protagonista[i].image, bullets_protagonista[i].x, bullets_protagonista[i].y, 0);

    if (bullets_protagonista[i].x > WIDTH_SCREEN) {
      colision = true;
    }

    for (int j = 0; j < ENEMIES_COUNT; j++) {
      if (!enemies[j].active) continue;

      if (colision_bullet_in_enemy(&bullets_protagonista[i], &enemies[j])) {
        colision = true;
        enemies[j].active = false;
        break;
      }
    }

    if (colision) {
      bullets_protagonista[i].active = false;
    }
  }
}

void drawBulletCount(int bullets) {
  al_draw_bitmap(protagonista->image_bullet, 0, 10, 0);
  al_draw_textf(GAME_INFO->font_bullet, AL_COLOR_BLACK, 100, 25, ALLEGRO_ALIGN_LEFT, "%d", bullets);
}

void handlerProtagonista() {
  drawBulletCount(protagonista->bullets);
  drawProtagonista(protagonista);
  moveProtagonista(protagonista);
  shootProtagonista(protagonista);
}