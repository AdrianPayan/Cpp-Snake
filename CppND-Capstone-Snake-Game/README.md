# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

## New Features
1. Added two new tipes of cubes:
* Extra: Grants 2 points to the player and doubles the speed compared to regular food, but only increases the snake's length by 1 unit. Displayed on screen in yellow color.
* Mine: Ends the game immediately when touched by the snake. Displayed on screen in red color.
2. Highscore board that saves the highest 5 scores.
3. Added key to close the game (ESC). Also saves the score and update the highscore if necesary.

## Rubric
1. README
* [DONE] A README with instructions is included with the project
* [DONE] The README indicates the new features you added to the game.
* [DONE] The README includes information about each rubric point addressed.
2. Compiling and Testing
* [DONE] The submission must compile and run without errors on the Udacity project workspace.
3. Loops, functions, I/O
* [DONE] The project demonstrates an understanding of C++ functions and control structures.
* [DONE] The project reads data from a file and process the data, or the program writes data to a file.
* [DONE] The project accepts user input and processes the input.
* [DONE] The project uses data structures and immutable variables.
4. Object Oriented Programming
* [DONE] One or more classes are added to the project with appropriate access specifiers for class members.
* [DONE] Class constructors utilize member initialization lists.
* [DONE] Classes abstract implementation details from their interfaces.
5. Memory Management
* [DONE] The project makes use of references in function declarations.
* [DONE] The project uses destructors appropriately.
* [DONE] The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
6. Concurrency
* [DONE] The project uses multithreading.
* [DONE] A mutex or lock is used in the project.