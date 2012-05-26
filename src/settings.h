#ifndef PICKLE_SETTINGS_H_
#define PICKLE_SETTINGS_H_

#define PI 3.14159265

// Window
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_MARGIN 20

// Gameplay
#define JAR_CAPACITY 10
#define MAX_LIVES    3

#define UPDATE_INTERVAL  1000.0/60
#define DROP_INTERVAL    1000
#define SPEEDUP_INTERVAL 10000

#define PICKLE_PROB_REDUCTION 0.95
#define PICKLE_PROB_MIN       0.1

#define DROP_INTERVAL_REDUCTION 0.95
#define DROP_INTERVAL_MIN       200

#define ADDITIONAL_DROP_PROB 0.01

#define THING_GRAVITY   -100 // Pixels/sec^2
#define THING_MIN_SCALE 0.7
#define THING_FADE_RATE 5

#define LOSE_JAR_SPREAD 400

// Cursor
#define CURSOR_X      WINDOW_WIDTH/2
#define CURSOR_Y      80
#define CURSOR_WIDTH  70
#define CURSOR_HEIGHT 20
#define CURSOR_MIN_X  CURSOR_WIDTH/2
#define CURSOR_MAX_X  WINDOW_WIDTH - (CURSOR_WIDTH/2)

// Background
#define BACKGROUND_INITIAL_OFFSET 0.5 // Radians
#define BACKGROUND_SPEED          0.02 // Radians per second
#define BACKGROUND_SCORE_SPEED    5 // Radians per second
#define BACKGROUND_INTERVAL       (2*PI)/20 // Radians
#define BACKGROUND_RAY_LENGTH     WINDOW_WIDTH
#define BACKGROUND_ORIGIN_X       320
#define BACKGROUND_ORIGIN_Y       -50

// Timers
#define UPDATE_TIMER  1
#define UNHURT_TIMER  2
#define UNSCORE_TIMER 3

// Other
#define SPLAT_SPEED 100 // Pixels/sec

#define IMAGE_DIR "/images/"

#endif
