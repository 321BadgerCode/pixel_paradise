#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int GRID_WIDTH = 32;
int GRID_HEIGHT = 32;

const int CANVAS_WIDTH = SCREEN_WIDTH - 150;
const int CANVAS_HEIGHT = SCREEN_HEIGHT;

const int BUTTON_WIDTH = 60;
const int BUTTON_HEIGHT = 30;
const int BUTTON_SPACING = 10;
const int BUTTON_X_OFFSET = SCREEN_WIDTH - BUTTON_WIDTH - BUTTON_SPACING;

std::vector<std::vector<std::vector<float>>> pixelGrid(GRID_HEIGHT, std::vector<std::vector<float>>(GRID_WIDTH, {1.0f, 1.0f, 1.0f}));

float currentColor[3] = {1.0f, 1.0f, 1.0f};

void initOpenGL() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 1.0);
}

void drawPixels() {
	const int pixelWidth = CANVAS_WIDTH / GRID_WIDTH;
	const int pixelHeight = CANVAS_HEIGHT / GRID_HEIGHT;

	for (int y = 0; y < GRID_HEIGHT; ++y) {
		for (int x = 0; x < GRID_WIDTH; ++x) {
			glColor3f(pixelGrid[y][x][0], pixelGrid[y][x][1], pixelGrid[y][x][2]);
			glBegin(GL_QUADS);
			glVertex2f(x * pixelWidth, y * pixelHeight);
			glVertex2f((x + 1) * pixelWidth, y * pixelHeight);
			glVertex2f((x + 1) * pixelWidth, (y + 1) * pixelHeight);
			glVertex2f(x * pixelWidth, (y + 1) * pixelHeight);
			glEnd();
		}
	}
}

void drawColorButtons() {
	const float colors[9][3] = {
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f},
		{1.0f, 0.5f, 0.0f},
		{1.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{0.29f, 0.0f, 0.51f},
		{0.93f, 0.51f, 0.93f}
	};

	for (int i = 0; i < 9; ++i) {
		float yPos = BUTTON_SPACING + i * (BUTTON_HEIGHT + BUTTON_SPACING);

		glColor3f(colors[i][0], colors[i][1], colors[i][2]);
		glBegin(GL_QUADS);
		glVertex2f(BUTTON_X_OFFSET, yPos);
		glVertex2f(BUTTON_X_OFFSET + BUTTON_WIDTH, yPos);
		glVertex2f(BUTTON_X_OFFSET + BUTTON_WIDTH, yPos + BUTTON_HEIGHT);
		glVertex2f(BUTTON_X_OFFSET, yPos + BUTTON_HEIGHT);
		glEnd();
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (xpos < CANVAS_WIDTH) {
			int gridX = static_cast<int>((xpos) / (CANVAS_WIDTH / GRID_WIDTH));
			int gridY = static_cast<int>((ypos) / (CANVAS_HEIGHT / GRID_HEIGHT));

			if (gridX < GRID_WIDTH && gridY < GRID_HEIGHT) {
				pixelGrid[gridY][gridX] = {currentColor[0], currentColor[1], currentColor[2]};
			}
		}
	}
}

void handleColorButtonClick(GLFWwindow* window) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (xpos > BUTTON_X_OFFSET && xpos < BUTTON_X_OFFSET + BUTTON_WIDTH) {
		int buttonIdx = static_cast<int>((ypos - BUTTON_SPACING) / (BUTTON_HEIGHT + BUTTON_SPACING));
		if (buttonIdx >= 0 && buttonIdx < 9) {
			const float colors[9][3] = {
				{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f},
				{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.29f, 0.0f, 0.51f}, {0.93f, 0.51f, 0.93f}
			};
			currentColor[0] = colors[buttonIdx][0];
			currentColor[1] = colors[buttonIdx][1];
			currentColor[2] = colors[buttonIdx][2];
		}
	}
}

void saveImage(const std::string& filename) {
	std::ofstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Error saving image!" << std::endl;
		return;
	}

	uint16_t bfType = 0x4D42;
	uint32_t bfSize = 54 + 3 * GRID_WIDTH * GRID_HEIGHT;
	uint16_t bfReserved1 = 0, bfReserved2 = 0;
	uint32_t bfOffBits = 54;
	uint32_t biSize = 40;
	int32_t biWidth = GRID_WIDTH;
	int32_t biHeight = GRID_HEIGHT;
	uint16_t biPlanes = 1;
	uint16_t biBitCount = 24;
	uint32_t biCompression = 0;
	uint32_t biSizeImage = 3 * GRID_WIDTH * GRID_HEIGHT;
	int32_t biXPelsPerMeter = 0, biYPelsPerMeter = 0;
	uint32_t biClrUsed = 0, biClrImportant = 0;

	file.write(reinterpret_cast<const char*>(&bfType), sizeof(bfType));
	file.write(reinterpret_cast<const char*>(&bfSize), sizeof(bfSize));
	file.write(reinterpret_cast<const char*>(&bfReserved1), sizeof(bfReserved1));
	file.write(reinterpret_cast<const char*>(&bfReserved2), sizeof(bfReserved2));
	file.write(reinterpret_cast<const char*>(&bfOffBits), sizeof(bfOffBits));

	file.write(reinterpret_cast<const char*>(&biSize), sizeof(biSize));
	file.write(reinterpret_cast<const char*>(&biWidth), sizeof(biWidth));
	file.write(reinterpret_cast<const char*>(&biHeight), sizeof(biHeight));
	file.write(reinterpret_cast<const char*>(&biPlanes), sizeof(biPlanes));
	file.write(reinterpret_cast<const char*>(&biBitCount), sizeof(biBitCount));
	file.write(reinterpret_cast<const char*>(&biCompression), sizeof(biCompression));
	file.write(reinterpret_cast<const char*>(&biSizeImage), sizeof(biSizeImage));
	file.write(reinterpret_cast<const char*>(&biXPelsPerMeter), sizeof(biXPelsPerMeter));
	file.write(reinterpret_cast<const char*>(&biYPelsPerMeter), sizeof(biYPelsPerMeter));
	file.write(reinterpret_cast<const char*>(&biClrUsed), sizeof(biClrUsed));
	file.write(reinterpret_cast<const char*>(&biClrImportant), sizeof(biClrImportant));

	for (int y = GRID_HEIGHT - 1; y >= 0; --y) {
		for (int x = 0; x < GRID_WIDTH; ++x) {
			file.put(static_cast<char>(pixelGrid[y][x][2] * 255));
			file.put(static_cast<char>(pixelGrid[y][x][1] * 255));
			file.put(static_cast<char>(pixelGrid[y][x][0] * 255));
		}
	}

	file.close();
	std::cout << "Image saved!" << std::endl;
}

void loadImage(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Error loading image!" << std::endl;
		return;
	}

	file.seekg(18);
	int width, height;
	file.read(reinterpret_cast<char*>(&width), 4);
	file.read(reinterpret_cast<char*>(&height), 4);

	pixelGrid = std::vector<std::vector<std::vector<float>>>(height, std::vector<std::vector<float>>(width, {1.0f, 1.0f, 1.0f}));

	// TODO: Fix padding for 8 bit bitmap images
	// int padding = 4 - (width % 4);
	int padding = 0;

	file.seekg(54);

	for (int y = height - 1; y >= 0; --y) {
		for (int x = 0; x < width; ++x) {
			unsigned char pixel[3];
			file.read(reinterpret_cast<char*>(pixel), 3);
			pixelGrid[y][x] = {pixel[2] / 255.0f, pixel[1] / 255.0f, pixel[0] / 255.0f};
		}

		file.seekg(padding, std::ios::cur);
	}

	GRID_WIDTH = width;
	GRID_HEIGHT = height;
	std::cout << "Image loaded!" << std::endl;
}

void resizeImage(int newWidth, int newHeight) {
	std::vector<std::vector<std::vector<float>>> newPixelGrid(newHeight, std::vector<std::vector<float>>(newWidth, {1.0f, 1.0f, 1.0f}));

	float xRatio = static_cast<float>(GRID_WIDTH) / newWidth;
	float yRatio = static_cast<float>(GRID_HEIGHT) / newHeight;

	for (int y = 0; y < newHeight; ++y) {
		for (int x = 0; x < newWidth; ++x) {
			int oldX = static_cast<int>(x * xRatio);
			int oldY = static_cast<int>(y * yRatio);
			newPixelGrid[y][x] = pixelGrid[oldY][oldX];
		}
	}

	pixelGrid = newPixelGrid;
	GRID_WIDTH = newWidth;
	GRID_HEIGHT = newHeight;
	std::cout << "Image resized!" << std::endl;
}

void drawButtons() {
	const int colors = 9;
	const int buttonYOffset = BUTTON_HEIGHT + (colors * (BUTTON_HEIGHT + BUTTON_SPACING)) + BUTTON_SPACING;

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(BUTTON_X_OFFSET, buttonYOffset);
	glVertex2f(BUTTON_X_OFFSET + BUTTON_WIDTH, buttonYOffset);
	glVertex2f(BUTTON_X_OFFSET + BUTTON_WIDTH, buttonYOffset + BUTTON_HEIGHT);
	glVertex2f(BUTTON_X_OFFSET, buttonYOffset + BUTTON_HEIGHT);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(BUTTON_X_OFFSET, buttonYOffset + BUTTON_SPACING + BUTTON_HEIGHT);
	glVertex2f(BUTTON_X_OFFSET + BUTTON_WIDTH, buttonYOffset + BUTTON_SPACING + BUTTON_HEIGHT);
	glVertex2f(BUTTON_X_OFFSET + BUTTON_WIDTH, buttonYOffset + 2 * BUTTON_HEIGHT + BUTTON_SPACING);
	glVertex2f(BUTTON_X_OFFSET, buttonYOffset + 2 * BUTTON_HEIGHT + BUTTON_SPACING);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(BUTTON_X_OFFSET, buttonYOffset + 2 * BUTTON_SPACING + 2 * BUTTON_HEIGHT);
	glVertex2f(BUTTON_X_OFFSET + BUTTON_WIDTH, buttonYOffset + 2 * BUTTON_SPACING + 2 * BUTTON_HEIGHT);
	glVertex2f(BUTTON_X_OFFSET + BUTTON_WIDTH, buttonYOffset + 3 * BUTTON_HEIGHT + 2 * BUTTON_SPACING);
	glVertex2f(BUTTON_X_OFFSET, buttonYOffset + 3 * BUTTON_HEIGHT + 2 * BUTTON_SPACING);
	glEnd();
}

void handleButtonClick(GLFWwindow* window) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		const int colors = 9;
		const int buttonYOffset = BUTTON_HEIGHT + (colors * (BUTTON_HEIGHT + BUTTON_SPACING)) + BUTTON_SPACING;

		if (xpos > BUTTON_X_OFFSET && xpos < BUTTON_X_OFFSET + BUTTON_WIDTH && ypos > buttonYOffset && ypos < buttonYOffset + BUTTON_HEIGHT) {
			saveImage("image.bmp");
		}

		if (xpos > BUTTON_X_OFFSET && xpos < BUTTON_X_OFFSET + BUTTON_WIDTH && ypos > buttonYOffset + BUTTON_SPACING + BUTTON_HEIGHT && ypos < buttonYOffset + 2 * BUTTON_HEIGHT + BUTTON_SPACING) {
			loadImage("image.bmp");
		}

		if (xpos > BUTTON_X_OFFSET && xpos < BUTTON_X_OFFSET + BUTTON_WIDTH && ypos > buttonYOffset + 2 * BUTTON_SPACING + 2 * BUTTON_HEIGHT && ypos < buttonYOffset + 3 * BUTTON_HEIGHT + 2 * BUTTON_SPACING) {
			int newWidth, newHeight;
			std::cout << "Width: ";
			std::cin >> newWidth;
			std::cout << "Height: ";
			std::cin >> newHeight;
			resizeImage(newWidth, newHeight);
		}
	}
}

int main() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pixel Art Editor", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		std::cerr << "Failed to create GLFW window!" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	initOpenGL();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		drawPixels();

		drawColorButtons();

		handleColorButtonClick(window);

		drawButtons();

		handleButtonClick(window);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}