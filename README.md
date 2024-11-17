<p align="center">
	<img src="./asset/logo.png" alt="Pixel Paradise logo" width="200" height="200">
</p>

<h1 align="center">Pixel Paradise</h1>

<p align="center">
	<strong>Generate pixel images!</strong>
</p>

> [!NOTE]
> Padding for the reading of the 8 bit bitmaps is not implemented yet, so images are currently shifted along the x-axis when loaded into the pixel grid.

## 🚀 Overview

Welcome to **Pixel Paradise**! This program allows you to generate pixel images using a simple and intuitive interface. The program is written in C++ and uses OpenGL for rendering.

## 🎨 Features

- **Variety of Colors:** ROYGBIV colors.
- **Save Image:** Save the pixel grid to a bitmap image file.
- **Load Image:** Load a bitmap image file to the pixel grid.
- **Customizable Size:** Change the size of pixel grid.

## 🛠️ Installation

To get started with the program, follow the steps below:

1. **Clone the Repository**
```sh
git clone https://github.com/321BadgerCode/pixel_paradise.git
cd ./pixel_paradise/
```

2. **Compile the Program**
```sh
g++ ./main.cpp -lglfw -lGL -o pixel_paradise
```

<details>

<summary>📦 Dependencies</summary>

- **OpenGL:** `sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev`
- **GLFW:** `sudo apt-get install libglfw3`

</details>

## 📈 Usage

To use the program, there is only **one** step!

1. **Run the program**
```sh
./pixel_paradise
```

## 📜 License

[LICENSE](./LICENSE)