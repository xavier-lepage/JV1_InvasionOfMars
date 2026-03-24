#pragma once

static const unsigned int SCREEN_WIDTH = 1280;
static const unsigned int SCREEN_HEIGHT = 720;

static const unsigned int WORLD_WIDTH = 3840;
static const unsigned int WORLD_HEIGHT = 2160;

const float PI = 3.14159265358979323846f;
const float FULL_TURN = 2.0f * PI;
const float DIAGONAL_RATIO = 0.70710678f;
const float NORMAL_SCALE = 1.0f;

const float PLAYER_RADIUS = 32.0f;
const float PLAYER_SPEED = 300.0f;
const float PLAYER_RESPAWN_COOLDOWN = 2.0f;
const float PLAYER_INVINCIBILITY_DURATION = 3.0f;
static const unsigned int PLAYER_LIFE_COUNT = 5;
static const unsigned int PLAYER_DEFAULT_OPACITY = 255;
static const unsigned int PLAYER_INVINCIBLE_OPACITY = 160;

const float BOOST_DURATION = 5.0f;
const float PLAYER_BOOST_SPEED = 400.0f;

static const unsigned int ALIEN_COUNT = 60;
const float ALIEN_SPAWN_COOLDOWN = 0.33f;
const float ALIEN_SPAWN_ANIMATION_DURATION = 1.0f;
const float MIN_ALIEN_DISTANCE = 200.0f;
const float ALIEN_SPEED = 200.0f;

static const unsigned int SCORE_TAG_COUNT = 60;
const float SCORE_TAG_DURATION = 1.0f;

static const unsigned int SCORE_INCREMENT = 100;
static const unsigned int MAX_SCORE_INCREMENT = 500;
static const unsigned int COMBO_INCREMENT = 10;
static const unsigned int LIFE_GAIN_SCORE_TRESHOLD = 50000;
const float COMBO_DURATION = 1.0f;

static const unsigned int BULLET_COUNT = 12;
static const unsigned int BULLET_ID = 1;
const float BULLET_SPEED = 1200.0f;
const float BULLET_RECOIL = 1.0f / 6.0f;

static const unsigned int BLAST_COUNT = 6;
static const unsigned int BLAST_ID = 0;
const float BLAST_SPEED = 800.0f;
const float BLAST_RECOIL = 1.0f / 3.0f;

const float POWERUP_DURATION = 15.0f;
const float POWERUP_CHANCE = 0.04f;
static const unsigned int BOOST_COUNT = 6;
static const unsigned int NUKE_COUNT = 6;
static const unsigned int BOOST_ID = 0;
static const unsigned int NUKE_ID = 1;

static const unsigned int MUSIC_COUNT = 3;
