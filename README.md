# Impossible Pacman
Pacman game implementation in C++ and Qt with artificial intelligence mode using Floyd-Warshall algorithm for shortest path.

<img src="./images/Example.gif" width="50%"> <br>

# Requirements
- Qt library ([Download link](https://www.qt.io/download-dev))

# How to run it
Build the project using CMake ([Download link](https://cmake.org/download/)):
1. Clone the repository and select it as source code.
2. Select the destination folder that will contain the built project.
3. Click Configure.
4. Select the desired generator:
   - For Unix Makefiles: Choose **Unix Makefiles** and ensure **Use default native compilers** is selected.
   - For IDEs supported by CMake (e.g. Xcode, Visual Studio, Eclipse), select the appropriate generator and ensure **Use default native compilers** is selected.
5. If Qt is not found automatically:
   - Add the Qt paths manually by clicking the ellipsis button next to the error message.
   - Alternatively, update the `CMakeLists.txt` file to specify the paths of your Qt installation. For example in my case is:
     ```cmake
     # Example for Qt5Widgets and Qt5Multimedia
     set(CMAKE_PREFIX_PATH "/Applications/Qt/5.15.2/clang_64/lib/cmake")
     ```
6. Click Generate.

    ### For Xcode Users
    1. Navigate to the destination folder and open the generated Xcode project.
    2. In Xcode, select the executable target from the top menu.
    3. Click the Play button in Xcode to build and run the project.

    Note:
    - You may need to include the following lines in the `CMakeLists.txt` file to specify the compiler paths:
        ```cmake
        set(CMAKE_C_COMPILER /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc)
        set(CMAKE_CXX_COMPILER /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++)
        ```

    - If you encounter errors, such as an unrecognized compiler during the Configure step, you can run the following command to set Xcode tools correctly:
        ```bash
        sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
        ```

    ### For Unix Makefiles
    1. Navigate to the build folder using a terminal.
    2. Run the `make` command to compile the project:
        ```bash
        make
        ```
    3. After compiling, launch the game with:
        ```bash
        ./impossiblepacman
        ```

## Important Notes
- Do not rename or move the source or destination folders after generating the project; otherwise, the build may fail.
- Ensure you are using the correct compilers. 
- Avoid using folder names with special characters or spaces, as they may cause errors in the build process.

Note: No matter which compiling solution you use. You can build the project also using one of the IDE project that can be created with CMake (e.g. Xcode, Eclipse etc.).

# Game logic
The classic pacman rules are implemented. We have 4 ghosts:

1. **Blinky** (red)
2. **Pinky** (pink)
3. **Inky** (cyan)
4. **Clyde** (orange)

Ghosts have different modes:
- **Chase**: ghosts actively pursue Pacman based on their individual targeting logic:

    - Blinky directly targets Pacman’s current position.
    - Pinky targets a position 4 tiles ahead of Pacman. If Pacman is moving up it adjust with other 4 tiles on the left.
    - Inky targets a tile that is 2 tiles directly ahead of Pacman in his current direction. If Pacman is moving up, Inky targets a tile that is 2 tiles ahead of Pacman in his current direction and then 2 tiles to the left of that position. Once Inky has determined Pacman's target tile (as described above), he then takes Blinky's position into account. Imagine a line drawn from Blinky's current position to the target tile calculated in the first step. Extend this line an equal distance beyond the target tile, effectively rotating the target by 180 degrees around the target tile. The end of this extended line (the point beyond the original target tile) becomes Inky's final target. This targeting method often causes Inky to coordinate with Blinky to trap Pacman between them.
    - Clyde targets in the lower left corner of the map. If it is at a distance less than 8 tiles from Pacman, it points directly to it as Blinky.

- **Scatter**: ghosts temporarily stop chasing Pacman and instead move to their respective corners of the map:

    - Blinky moves to the top-right corner.
    - Pinky moves to the top-left corner.
    - Inky moves to the bottom-right corner.
    - Clyde moves to the bottom-left corner.

- **Frightened**: ghosts can be eaten by Pacman.

- **Eaten**: ghosts eaten by Pacman. They turn into a pair of eyes and come back to the starting gate.

The different modes change following this sequence:
1. Scatter for 7 seconds, Chase for 20 seconds.
2. Scatter for 7 seconds, Chase for 20 seconds.
3. Scatter for 5 seconds, Chase for 20 seconds.
4. Scatter for 5 seconds, Chase until the end of the level.

Ghosts can change their direction only when they have arrived on a node of the map.

If Pacman dies or the level is completed, the cycle resets. Additionally, when only 20 pieces of food remain, Blinky permanently enters Scatter mode. Ghosts are slightly faster than Pacman.

When Pacman eats a power pellet, all ghosts enter Frightened mode. In this mode, ghosts turn blue, move more slowly, and can be eaten by Pacman for extra points. Their movement logic also changes—they reverse their current direction and start moving randomly.

If Pacman eats a ghost in Frightened mode, the ghost enters Eaten mode. The ghost’s appearance changes to just a pair of eyes, and it heads directly back to the starting gate. Once there, it returns to its normal form and resumes either Scatter or Chase mode, depending on the current cycle.

# How to play it
The game open with a welcome window: <br>

<img src="./images/Welcome-window.png" width="50%">

<br>

You can click on:

- **Play**: to play the normal pacman game mode. <br>
<img src="./Sprites/Play.png"> <br>
Pacman is controlled using the arrow keys on your keyboard.

If you want to change the game mode, on the top of the screen there are two buttons:
- **Normal**: to play in the current normal mode. <br>
<img src="./Sprites/normal.png">  <br>

- **AI**: to play the AI version where Blinky and Clyde are powered with the Floyd-Warshall algorithm for shortest path, while the other ghosts are in permanent Chase mode.
After you selected your game mode, you will be redirected to the welcome window, and clicking again on "Play" you will play in the selected mode. You can recognize it because the ghosts will have an angry face and the map will blink in RGB colors<br>
<img src="./Sprites/buttonAi.png">  <br>

- **Ranking**: to see the ranking scores saved. <br>
<img src="./Sprites/Ranking.png"> <br>
The 10 best saved scores are shown: <br>
<img src="./images/Ranking-show.png" width="50%"> <br>
Click on the yellow arrow to go back to the welcome window.



Once your lives are finished or you finished all the food, you will see a "Game over" or "You Win!" message. 

<img src="./Sprites/gameover.png"> <br>
<img src="./Sprites/win.png"> <br>

The ranking window will appear to save your score. Insert your name into the text box and click "Ok" to save it, or click "Go back" to come back to the welcome window and not save your score.

<img src="./images/Ranking-save.png" width="50%">

# Shortcuts
- P: paused the game. Click it again to continue to play. <br>
    <img src="./images/Pause.png" width="50%"> <br>

- T: show the ghost's target.

    Normal mode: <br>
    <img src="./images/Target-show-normal.png" width="50%">

    AI mode: <br>
    <img src="./images/Target-show-ai.png" width="50%">
