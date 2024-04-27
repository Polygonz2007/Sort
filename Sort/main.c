#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <math.h>

#include <raylib.h>

#define NUM_ENTRIES 1000


// Proto
int randomize(uint16_t *data);
int shuffle(uint16_t* data);
int bubble_sort(uint16_t* data);
int bogo_sort(uint16_t* data);


int main() {

	// Raylib
	const window_x = 1280, window_y = 720;
	InitWindow(window_x, window_y, "Sort");
	SetTargetFPS(144);

	// Settings
	const float bar_scale = 0.01f;
	const int bar_width = 1;
	const Color bar_color = RED;

	// Sorting
	uint16_t *data;
	data = malloc(NUM_ENTRIES * sizeof(uint16_t));

	randomize(data);

	// Loop
	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(WHITE);
		DrawText("Sorter", 12, 12, 32, BLACK);

		// Lines
		for (uint32_t i = 0; i < NUM_ENTRIES; ++i) {
			uint32_t n = (int)(data[i] * bar_scale);
			DrawRectangle(i * bar_width, window_y - n, bar_width, n, bar_color);
		}

		if (IsKeyPressed(KEY_S))
			shuffle(data);

		if (IsKeyPressed(KEY_R))
			randomize(data);

		if (IsKeyPressed(KEY_B))
			bubble_sort(data);

		if (IsKeyPressed(KEY_E))
			bogo_sort(data);

		EndDrawing();
	}

	free(data);
	CloseWindow();
}








int randomize(uint16_t *data) {
	printf("\nRandomizing...\n");

	for (uint32_t i = 0; i < NUM_ENTRIES; ++i) {
		data[i] = rand();
	}

	return 0;
}

int shuffle(uint16_t *data) {
	printf("\nShufflin'\n");

	for (uint32_t i = 0; i < NUM_ENTRIES; ++i) {
		const uint32_t pick = rand() % (NUM_ENTRIES - 1);
		const uint16_t p =  data[pick];

		data[pick] = data[i];
		data[i] = p;
	}

	return 0;
}

int bubble_sort(uint16_t *data) {
	printf("\nBubble sorting\n");
	const double start = GetTime();

	for (uint32_t i = 1; i < NUM_ENTRIES; ++i) {

		// Iteration
		for (uint32_t j = 0; j < NUM_ENTRIES - i; ++j) {
			const uint16_t c = data[j];
			const uint16_t n = data[j + 1];

			if (c > n) {
				data[j] = n;
				data[j + 1] = c;
			}
		}
	}

	printf("Time spent: %fs\n", GetTime() - start);

	return 0;
}

int bogo_sort(uint16_t* data) {
	printf("Bogosort...\n");
	const start = GetTime();
	_Bool finished = false;
	uint32_t correct;

	while (!finished) {
		correct = 1;
		shuffle(data);

		// check if its sorted
		for (uint32_t i = 0; i < NUM_ENTRIES; ++i) {
			if (data[i] > data[i + 1])
				correct++;
		}

		printf("%d\n", correct);

		if (correct == NUM_ENTRIES)
			finished = true;
	}
	printf("Finished in %f seconds\n", GetTime() - start);
	return 0;

}