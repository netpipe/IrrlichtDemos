#include "../src/x11/X11InputController.h"
#include "../src/x11/X11Keyboard.h"

#include <X11/Xutil.h>
#include <cstdio>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	Display* display = XOpenDisplay(NULL);
	int screen = DefaultScreen(display);
	Window main_window;
	XSetWindowAttributes window_attrs;
	window_attrs.background_pixel = XWhitePixel(display, screen);
	window_attrs.border_pixel = XBlackPixel(display, screen);
	window_attrs.win_gravity = SouthWestGravity;
	main_window = XCreateWindow(display, RootWindow(display, screen), 1, 1, 600, 400,
	                             0, CopyFromParent, InputOutput, CopyFromParent,
	                             CWBackPixel|CWBorderPixel, &window_attrs
							   );

	printf("Created main window: %d\n", main_window);

	XMapWindow(display, main_window);
	XFlush(display);

	X11InputController input_controller(display, &main_window);
	if (!input_controller.initialize()) {
		fprintf(stderr, "Failed to initialize X11 input controller\n");
		return -1;
	}

	input_controller.enumerate();
	auto keyboards = input_controller.getKeyboards();

	for (const auto& keyboard : keyboards) {
		printf("Found keyboard %s with device id: %d\n", keyboard->name(), keyboard->deviceId());
	}

	auto mice = input_controller.getMice();
	for (const auto mouse : mice) {
		printf("Found a mouse %s with device id: %d\n", mouse->name(), mouse->deviceId());
	}


	bool input_valid = false;
	char choice;
	int device_id;

	KeyboardPtr keyboard;
	MousePtr mouse;

	do {
		printf("Select the input device:\n");
		printf("1. Mouse\n");
		printf("2. Keyboard\n");

		//cin >> choice;
		//if (choice == '1') {
			printf("Enter the mouse id\n");
			cin >> device_id;
			mouse = input_controller.getMouseById(device_id);
			if (mouse) {
				printf("Selected mouse with device id: %d\n", mouse->deviceId());
				input_valid = true;
			} else {
				printf("Mouse with device id: %d not found!\n", device_id);
			}
		//} else if (choice == '2') {
			printf("Enter the keyboard id\n");
			cin >> device_id;
			keyboard = input_controller.getKeyboardById(device_id);
			if (keyboard) {
				printf("Selected keyboard with device id: %d\n", keyboard->deviceId());
				input_valid = true;
			} else {
				printf("Keyboard with device id: %d not found!\n", device_id);
			}
		//} else {
		//	input_valid = false;
		//}
	} while(!input_valid);


	printf("Press Enter to continue...");
	getchar();

	while(1) {
		//if (mouse) {
			auto state = mouse->getMouseState();
			if (!state)
				continue;

			//system("clear");
			printf("Mouse state: x: %d, y: %d, btn1: %d, btn2: %d, btn3: %d\n",
					state->x, state->y, state->btn1_pressed,
					state->btn2_pressed, state->btn3_pressed);

		//}

		//if (keyboard) {
			auto kb_state = keyboard->getKeyboardState();
			bool* keys_state = kb_state->keys_pressed;
			//system("clear");
			printf("Keys pressed: ");
			for (int i = 0; i < 256; i++) {
				if (keys_state[i]) {
					printf("%d, ", i);
				}
			}
			printf("\n");
		//}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

//	int ret = input_controller.detachDeviceById(14);
//	printf("Detached ret: %d\n", ret);

//	input_controller.enumerate();
}
