# Raylib RPG Monsters Battle

Tutorial https://www.youtube.com/watch?v=fo4e3njyGy0

### Dependencies

- Rygame
  Created by Bruno Cabral, this library adds Raylib functionalities in Pygame style  
  https://github.com/brccabral/rygame  
  Itself has below dependencies  
- Raylib  
  Main library to manage window, inputs, screen  
  Compile and install raylib https://github.com/raysan5/raylib  
  On Windows, compile with `cmake *** -DCUSTOMIZE_BUILD=ON -DSUPPORT_WINMM_HIGHRES_TIMER=OFF`  
- TMX  
  Parse `.tmx` files (**Tiled**)  
  Compile and install TMX https://github.com/baylej/tmx  
- File `raylib-tmx.h` is from https://github.com/RobLoach/raylib-tmx  
  It contains helper functions to integrate TMX with Raylib  

### Resources

- Get game assets from tutorial project code https://github.com/clear-code-projects/Python-Monsters  
