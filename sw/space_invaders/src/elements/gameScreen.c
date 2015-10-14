#include "gameScreen.h"

// We need these to know what to erase the next time
static uint32_t currentScore = 0;
static uint8_t currentScoreArray[SCORE_VAL_MAX_LEN] = { 0 };
static uint8_t currentScoreLength = 0;

static uint8_t currentLives = 0;

void drawFloor();
void drawScore();
void drawLives();
void getScoreAsArray(uint32_t score, uint8_t *scoreArray, uint8_t *scoreLength);
uint32_t setScore(uint32_t score);
uint8_t setLives(uint8_t lives);

// ----------------------------------------------------------------------------

void gameScreen_init() {
	drawFloor();
	drawScore();
	drawLives();

	// draw a '0' next to "SCORE"
	setScore(0);

	// draw 3 tanks next to "LIVES"
	setLives(LIVES_MAX);
}

// ----------------------------------------------------------------------------

uint32_t gameScreen_increaseScore(int32_t score) {
	uint32_t newScore = currentScore;

	if ((currentScore + score) < 0) newScore = 0;
	else if ((currentScore + score) > SCORE_VAL_MAX) newScore = SCORE_VAL_MAX;
	else newScore = currentScore + score;

	return setScore(newScore);
}

// ----------------------------------------------------------------------------

uint8_t gameScreen_increaseLives(int8_t lives) {
	uint8_t newLives = currentLives;

	if ((currentLives + lives) < 0) newLives = 0;
	else if ((currentLives + lives) > LIVES_MAX) newLives = LIVES_MAX;
	else newLives = currentLives + lives;

	return setLives(newLives);
}

// ----------------------------------------------------------------------------
// Private Helper Methods
// ----------------------------------------------------------------------------

void drawFloor() {
	point_t origin = { .x = FLOOR_ORIGIN_X, .y = FLOOR_ORIGIN_Y };
	symbolsize_t size = { .w = FLOOR_WIDTH, .h = FLOOR_HEIGHT };

	uint8_t i = 0;
	for (i=0; i<(SCREEN_WIDTH/FLOOR_WIDTH); i++) {
		screen_drawSymbol(floor_32x2, origin, size, FLOOR_SCALE, FLOOR_COLOR);

		// shift the origin over a bit
		origin.x += FLOOR_WIDTH*FLOOR_SCALE;
	}

}

// ----------------------------------------------------------------------------

void drawScore() {
	point_t origin = { .x = SCORE_TXT_ORIGIN_X, .y = SCORE_TXT_ORIGIN_Y };
	symbolsize_t size = { .w = SCORE_TXT_WIDTH, .h = SCORE_TXT_HEIGHT };

	// Draw the text "SCORE"
	screen_drawSymbol(word_score_30x5, origin, size, SCORE_TXT_SCALE, SCORE_TXT_COLOR);
}

// ----------------------------------------------------------------------------

void drawLives() {
	point_t origin = { .x = LIVES_TXT_ORIGIN_X, .y = LIVES_TXT_ORIGIN_Y };
	symbolsize_t size = { .w = LIVES_TXT_WIDTH, .h = LIVES_TXT_HEIGHT };

	// Draw the text "SCORE"
	screen_drawSymbol(word_lives_24x5, origin, size, LIVES_TXT_SCALE, LIVES_TXT_COLOR);
}

// ----------------------------------------------------------------------------

uint8_t setLives(uint8_t lives) {
	point_t origin = { .x = LIVES_TANK_ORIGIN_X, .y = LIVES_TANK_ORIGIN_Y };
	symbolsize_t size = { .w = LIVES_TANK_WIDTH, .h = LIVES_TANK_HEIGHT };
	uint8_t i = 0;

	for (i=0; i<LIVES_MAX; i++) {
		// Find out if this tank live should be drawn or not
		uint32_t color = (i<lives) ? LIVES_TANK_COLOR : SCREEN_BG_COLOR;

		// Draw a tank at the given origin
		screen_drawSymbol(tank_15x8, origin, size, LIVES_TANK_SCALE, color);

		// Shift the origin over to the next tank
		origin.x += (LIVES_TANK_WIDTH+LIVES_TANK_PADDING)*LIVES_TANK_SCALE;
	}

	// save lives for next time
	currentLives = lives;

	return currentLives;
}

// ----------------------------------------------------------------------------

uint32_t setScore(uint32_t score) {
	point_t origin = { .x = SCORE_VAL_ORIGIN_X, .y = SCORE_VAL_ORIGIN_Y };
	symbolsize_t size = { .w = SCORE_VAL_WIDTH, .h = SCORE_VAL_HEIGHT };
	uint8_t i = 0;

	// Get score info as an array so we can loop through each digit
	getScoreAsArray(score, currentScoreArray, &currentScoreLength);

	for (i=0; i<SCORE_VAL_MAX_LEN; i++) {

		// Find out if this digit should be drawn or not
		uint32_t color = (i<currentScoreLength) ? SCORE_VAL_COLOR : SCREEN_BG_COLOR;

		uint32_t digit = currentScoreArray[i];
		screen_drawSymbol(numbers_5x5[digit], origin, size, SCORE_VAL_SCALE, color);

		// Shift the origin over to the next spot
		origin.x += (SCORE_VAL_WIDTH+SCORE_VAL_PADDING)*SCORE_VAL_SCALE;
	}

	// save score for next time
	currentScore = score;

	return currentScore;
}

// ----------------------------------------------------------------------------

void getScoreAsArray(uint32_t score, uint8_t *scoreArray, uint8_t *scoreLength) {

	uint8_t tmpArray[SCORE_VAL_MAX_LEN] = { 0 };
	uint8_t count = 0;
	int8_t i = (SCORE_VAL_MAX_LEN-1); // A backward counter to tell us which digit

	// If you passed in a 0 score, make the appropriate array/length
	if (score == 0) {
		scoreArray[0] = 0;
		*scoreLength = 1;
		return;
	}

	// while there's still a score to explode out into an array
	while(score) {
		// Grab the digit
		tmpArray[i] = (score % 10);

		// Divide by 10 to get the next digit in the one's spot
		score /= 10;

		i--;
		count++;
	}

	uint8_t j = 0;
	// Push the tmpArray to the left, so the 0's are at the end
	for (j=0; j<count; j++) scoreArray[j] = tmpArray[++i];

	// Let the caller now how many digits there are
	*scoreLength = count;
}