
// FOR MULITHREADING
#define WIN32_LEAN_AND_MEAN
#if defined(_WIN32)
#define WIN32
#endif
#if defined(_WIN64)
#define WIN64
#define _AMD64_
#undef _X86_
#else
#undef _AMD64_
#define _X86_
#endif

#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <Windows.h> // or any library that uses Windows.h

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif
// END MULTITHREADING




#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include <math.h>

#include <raylib.h>

#define NUM_ENTRIES 1024

// Prototypes
DWORD WINAPI run_sort(LPVOID lpParameter);
_Bool is_thread_running(HANDLE hThread);
int sort(char type[32]);

// Proto
int randomize(uint16_t *data);
int shuffle(uint16_t* data);
int bubble_sort(uint16_t* data);
int bogo_sort(uint16_t* data);

uint16_t* data;

uint16_t *aux1, *aux2;
char *current_sort[32];


int main() {

	// Raylib
	const window_x = 1280, window_y = 720;
	InitWindow(window_x, window_y, "Sort");
	SetTargetFPS(144);

	// Settings
	const float bar_scale = 0.01f;
	const int bar_width = 1;
	const Color bar_color = BLACK;

	// Sorting
	data = malloc(NUM_ENTRIES * sizeof(uint16_t));
	aux1 = malloc(NUM_ENTRIES * sizeof(uint16_t));
	aux2 = malloc(NUM_ENTRIES * sizeof(uint16_t));

	randomize(data);

	// Threading
	*current_sort = "none";
	printf("Current sort: %s", *current_sort);

	HANDLE sort_h_thread = NULL; // Multithreading for sort
	DWORD sort_dw_thread_id;
	_Bool sort_thread_status = false; // fasle: open, true: running

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

		if (IsKeyPressed(KEY_S)) {
			*current_sort = "shuffle";
			sort_h_thread = CreateThread(NULL, 0, run_sort, 0, 0, &sort_dw_thread_id);
		}
			

		if (IsKeyPressed(KEY_R)) {
			*current_sort = "random";
			sort_h_thread = CreateThread(NULL, 0, run_sort, 0, 0, &sort_dw_thread_id);
		}

		if (IsKeyPressed(KEY_B)) {
			*current_sort = "bubble";
			sort_h_thread = CreateThread(NULL, 0, run_sort, 0, 0, &sort_dw_thread_id);
		}

		EndDrawing();
	}

	free(data);
	CloseWindow();
}

int randomize() {
	srand(GetTime() * 100.0f);
	for (uint32_t i = 0; i < NUM_ENTRIES; ++i) {
		data[i] = rand();
		WaitTime(0.0001f);
	}

	return 0;
}

int shuffle() {
	for (uint32_t i = 0; i < NUM_ENTRIES; ++i) {
		const uint32_t pick = rand() % (NUM_ENTRIES - 1);
		const uint16_t p =  data[pick];

		data[pick] = data[i];
		data[i] = p;

		WaitTime(0.0001f);
	}

	return 0;
}

int quick_sort(uint32_t start, uint32_t end) {
	uint32_t length = end - start;
	uint32_t length1 = length * 0.5f;
	uint32_t length2 = length1;

	if (length == 2) {
		const uint16_t a = data[start];
		const uint16_t b = data[start + 1];

		if (a > b) {
			data[start] = b;
			data[start + 1] = a;
			return 0;
		}
	}

	if (length & 0x0001 == 0) {
		// It is not odd we are goood buddy
		//memcpy(&aux1, &data, length1);
		//memcpy(&aux2, &data, length1);

		quick_sort(start, start + length1);
		quick_sort(start + length1, start + length1 + length2);
	}

	return 0;
}

int bogo_sort() {
	_Bool sorted = false;

	while (!sorted) {
		shuffle();
		sorted = true;

		for (uint32_t i = 0; i < NUM_ENTRIES; ++i) {
			if (data[i] > data[i + 1]) {
				sorted = false;
				break;
			}
		}
	}

	return 0;
}

int bubble_sort() {
	for (uint32_t i = 1; i < NUM_ENTRIES; ++i) {

		// Iteration
		uint32_t swaps = 0;
		for (uint32_t j = 0; j < NUM_ENTRIES - i; ++j) {
			const uint16_t c = data[j];
			const uint16_t n = data[j + 1];

			if (c > n) {
				data[j] = n;
				data[j + 1] = c;
				swaps++;

				WaitTime(0.00001f);
			}
		}

		if (swaps == 0)
			break;
	}

	return 0;
}






DWORD WINAPI run_sort(LPVOID lpParameter) {

	// Load chunk data
	printf("\nSorting using %s sort.\n", *current_sort);
	double start_time = GetTime();

	if (*current_sort == "random")
		randomize();
	else if (*current_sort == "shuffle")
		shuffle();
	else if (*current_sort == "bubble")
		bubble_sort();
	else if (*current_sort == "bogo")
		bogo_sort();
	else if (*current_sort == "quick")
		quick_sort(0, NUM_ENTRIES);

	printf("Sort finished in %f seconds.\n", GetTime() - start_time);

	return 0;
}

_Bool is_thread_running(HANDLE hThread) {
	DWORD result = WaitForSingleObject(hThread, 0);
	return !result == WAIT_OBJECT_0;
}