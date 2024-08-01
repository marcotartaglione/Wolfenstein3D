Wolfenstein 3D
==================
#### A 3D game made in C using raycasting

## Table of Contents
- [Description](#description)
- [Technologies](#technologies)
- [How to play](#how-to-play)
- [How to compile](#how-to-compile)
- [License](#license)
- [Development Status](#development-status)

## Description
This repository contains the source code of a 3D game made in C using raycasting. The game is inspired by the game Wolfenstein 3D, released in 1992 by id Software. The game is a first-person shooter where the player must escape from a maze while fighting enemies and collecting treasures.

## Technologies
The game was made using the C programming language with the support, for graphics, of the [WIN32]() library by Microsoft. The game was developed and intended to be played on Windows.

## How to play
The game is not yet finished and is not playable. The game is still in development and there are some features to be implemented and bugs to be fixed.

## How to compile
To compile the game, you must have the Cmake installed on your computer. After that, you can run the following commands on the root directory of the project:

```cmake CMakeLists.txt```

The command above will generate the executable file of the game. To run the game, you can use the following command:

```Wolfenstein3D.exe``` or just double-click on the executable file.

## License
GNU General Public License v3.0

## Development Status
The project is currently in development and is not yet finished. The game is __not__ playable and there are some features to be implemented and bugs to be fixed.

## Editor
The ```Wolfenstein3D.exe``` also contains a map editor that can be used to modify the maps in the game.

### How to start the editor
In order to start the editor, you must run the following command: ```Wolfenstein3D.exe -editor```

### Choose a map and a floor
Once the editor is started, you can choose an apisode to edit by pressing the `ARROW UP` and `ARROW DOWN` keys and then pressing `ENTER`. 
You can repeat this process to choose a floor to edit.

In every moment, pressing the `ESC` will bring you back to the previous menu.

### Edit the map
The editor will show you a tile grid with some frame colors, according to this legend:

- `WHITE` - Empty tile or Wall tile
- `RED` - Current selected tile
- `BROWN` - Enemy start position
- `BLUE` - Player start position

Once you have chosen a map and a floor, you can move the map by using the following keys:

- `ARROW UP` - Move the selected tile up
- `ARROW DOWN` - Move the selected tile down
- `ARROW LEFT` - Move the selected tile left
- `ARROW RIGHT` - Move the selected tile right

You can also move to a specified row and column by pressing the following keys:

- `Q` - Open navigation menu
- `[1...9]` - Set current number
- `TAB` - Multiply current number by 10
- `SHIFT` - Divide current number by 10
- `=` - Change from row to column and vice-versa
- `ENTER` - Move to the specified row and column
- `ESC` - Close the navigation menu

When you have selected the tile you want to edit, you can change its type by pressing the following:

- `ENTER` - Change the tile to the selected wall type
- `P` - Player start position
- `E` - Enemy start position
- `WORK IN PROGRESS` - Place food
- `WORK IN PROGRESS` - Place treasure
- `WORK IN PROGRESS` - Place weapon
- `WORK IN PROGRESS` - Place ammo
- `DELETE` - Change the tile to be empty

In order to change the selected wall type, you can press the following keys:

- `TAB` - Open the wall type menu
- `ARROW UP` - Move the selected wall type up
- `ARROW DOWN` - Move the selected wall type down
- `ENTER` - Select the wall type
- `ESC` - Close the wall type menu

### Save the map
Once you have finished editing the map, you can press `ESC` to open the quit menu and choose to save before going back to the editor menu:

- `Y` - Save the map and quit the editor
- `N` - Quit the editor without saving the map
- `ESC` - Back to the editor
