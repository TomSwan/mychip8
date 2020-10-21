// config.h - (c) 2020 by Tom Swan - GPLv3 (see LICENSE.txt)

#ifndef CONFIG_H
#define CONFIG_H

#define TITLE "Chip-8 Emulator"
#define VERSION "0.1"
#define DESCRIPTION "Emulates the Cosmac VIP Chip-8 programming language\nby Tom Swan"
#define CAPTION "<< F1:Help Esc:Quit >>"
#define BACKGROUND_COLOR Qt::black
#define PIXEL_COLOR Qt::cyan
#define BLOCK_SIZE 18
#define SCREEN_WIDTH 64   // do not change
#define SCREEN_HEIGHT 32  // do not change
#define DISPLAY_WIDTH ((SCREEN_WIDTH+2)*BLOCK_SIZE)
#define DISPLAY_HEIGHT ((SCREEN_HEIGHT+2)*BLOCK_SIZE)
#define DELAY_TIMER_RATE_MS 17
#define SOUND_TIMER_RATE_MS 17
#define REFRESH_RATE_MS 18
#define CYCLE_RATE_MS 18
#define STEPS_PER_CYCLE 13
//#define DEBUGGING false
#define DEBUGGING true
#define SILENT_RUNNING false
#define ALT_KEYS false

#endif // CONFIG_H
