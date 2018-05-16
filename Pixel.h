#ifndef Pixel_h
#define Pixel_h

#include "Arduino.h"
#include <RGBmatrixPanel.h> // Hardware-specific library

class Pixel {
  
  public:
    Pixel(int row, int col, RGBmatrixPanel *matrix, void *board, 
          int dir, int r, int g, int b);
    void move(int dir);
    
  private:
    int _row;
    int _col;
    int _r;
    int _g;
    int _b;
    int _dir;
    RGBmatrixPanel *_matrix;
    char (*_board)[32];
    int *Pixel::collision_with_moves(int dir);
    int no_collisions(int dir);
    int is_in_bounds(int drow, int dcol);
    int _directions[9][2] = { {},  {+1, +0}, {+1, -1}, {+0, -1}, 
                              {+1, -1},           {-1, +0}, 
                              {-1, +1}, {+0, +1}, {+1, +1}  };
};

#endif