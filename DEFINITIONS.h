#pragma once

namespace game {
	enum class Direction { LEFT, RIGHT, UP, DOWN };
}

// Window options
#define INIT_SCREEN_WIDTH 1280
#define INIT_SCREEN_HEIGHT 720
#define GAME_TITLE "2048"

#define SCREEN_WIDTH this->_data->window.getSize().x
#define SCREEN_HEIGHT this->_data->window.getSize().y

// Splash state options
#define SPLASH_STATE_SHOW_TIME 4
#define LOGO_FILEPATH "res/img/logo.png"
#define INTRO_FILEPATH "res/audio/intro.wav"

// Main Menu options
#define MAIN_MENU_TITLE_FILEPATH "res/img/title.png"
#define GEAR_FILEPATH "res/img/gear.png"
#define PLAY_BUTTON "res/img/playbutton.png"
#define EXIT_BUTTON "res/img/exitbutton.png"
#define MENU_THEME "res/audio/menu.wav"

// Game state options
#define STATE_PLAYING 100
#define STATE_PAUSED 101
#define STATE_RESTART 102
#define STATE_HOME 103
#define PAUSE_BUTTON "res/img/pause_button.png"
#define PREV_BUTTON "res/img/prev_button.png"
#define GAME_MUSIC "res/audio/game_music.wav"

// Pause state options
#define RESUME_BUTTON "res/img/resume_button.png"
#define RESTART_BUTTON "res/img/restart_button.png"
#define HOME_BUTTON "res/img/home_button.png"

// Grid defs
#define GRID_BACK "res/img/grid/grid.png"
#define TILE_0 "res/img/grid/0_tile.png"
#define TILE_2 "res/img/grid/2_tile.png"
#define TILE_4 "res/img/grid/4_tile.png"
#define TILE_8 "res/img/grid/8_tile.png"
#define TILE_16 "res/img/grid/16_tile.png"
#define TILE_32 "res/img/grid/32_tile.png"
#define TILE_64 "res/img/grid/64_tile.png"
#define TILE_128 "res/img/grid/128_tile.png"
#define TILE_256 "res/img/grid/256_tile.png"
#define TILE_512 "res/img/grid/512_tile.png"
#define TILE_1024 "res/img/grid/1024_tile.png"
#define TILE_2048 "res/img/grid/2048_tile.png"

// Fonts
#define CALM_FONT "res/fonts/KeepCalm-Medium.ttf"

// Text boxes
#define TEXT_BOX "res/img/text_box.png"

// Button boxes
#define WIN_BOX "res/img/win_box.png"
#define LOSE_BOX "res/img/lost_box.png"