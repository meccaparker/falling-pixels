#include "Pixel.h"
#include "Arduino.h"


Pixel::Pixel(int row, int col, RGBmatrixPanel *matrix, void *board, int dir, 
            int r, int g, int b) {
  _row = row;
  _col = col;
  _r = r;
  _g = g;
  _b = b;
  _board = board;
  _matrix = matrix;
  _dir = dir;
}

void Pixel::move(int dir) {
  int drow = _directions[dir][0];
  int dcol = _directions[dir][1];
  if (is_in_bounds(drow, dcol)) {
    if (no_collisions(dir)) {
      _matrix->drawPixel(_col, _row, _matrix->Color333(0, 0, 0));
      _board[_row][_col] = 0;
      _row = _row + drow;
      _col = _col + dcol;
      _board[_row][_col] = 1;
      _matrix->drawPixel(_col, _row, _matrix->Color333(_r, _g, _b));
    } else {
      int *move = collision_with_moves(dir);
      drow = move[0];
      dcol = move[1];
      _matrix->drawPixel(_col, _row, _matrix->Color333(0, 0, 0));
      _board[_row][_col] = 0;
      _row = _row + drow;
      _col = _col + dcol;
      _board[_row][_col] = 1;
      _matrix->drawPixel(_col, _row, _matrix->Color333(_r, _g, _b));
    }
  }
}

int *Pixel::collision_with_moves(int dir) {
  int move[2];
  int drow1; int dcol1; 
  int drow2; int dcol2;
  move[0] = 0; move[1] = 0;
  switch (dir) {
    case 1:
      drow1 = 1; dcol1 = -1;
      drow2 = 1; dcol2 = 1;
      if (_board[_row + drow1][_col + dcol1] == 0 
          && is_in_bounds(drow1, dcol1)) {
            move[0] = drow1;
            move[1] = dcol1;
            break;
      } else if (_board[_row + drow2][_col + dcol2] == 0 
          && is_in_bounds(drow2, dcol2)) {
            move[0] = drow2;
            move[1] = dcol2;
            break;
      } break;
    case 2:
        drow1 = 0; dcol1 = -1;
        drow2 = 1; dcol2 = 0;
        if (_board[_row + drow1][_col + dcol1] == 0 
            && is_in_bounds(drow1, dcol1)) {
              move[0] = drow1;
              move[1] = dcol1;
              break;
        } else if (_board[_row + drow2][_col + dcol2] == 0 
            && is_in_bounds(drow2, dcol2)) {
              move[0] = drow2;
              move[1] = dcol2;
              break;
        } break;
    case 3:
        drow1 = -1; dcol1 = -1;
        drow2 = 1; dcol2 = -1;
        if (_board[_row + drow1][_col + dcol1] == 0 
            && is_in_bounds(drow1, dcol1)) {
              move[0] = drow1;
              move[1] = dcol1;
              break;
        } else if (_board[_row + drow2][_col + dcol2] == 0 
            && is_in_bounds(drow2, dcol2)) {
              move[0] = drow2;
              move[1] = dcol2;
              break;
        } break;
    case 4:
        drow1 = 0; dcol1 = -1;
        drow2 = 1; dcol2 = 0;
        if (_board[_row + drow1][_col + dcol1] == 0 
            && is_in_bounds(drow1, dcol1)) {
              move[0] = drow1;
              move[1] = dcol1;
              break;
        } else if (_board[_row + drow2][_col + dcol2] == 0 
            && is_in_bounds(drow2, dcol2)) {
              move[0] = drow2;
              move[1] = dcol2;
              break;
        } break;
    case 5:
        drow1 = -1; dcol1 = 1;
        drow2 = -1; dcol2 = -1;
        if (_board[_row + drow1][_col + dcol1] == 0 
            && is_in_bounds(drow1, dcol1)) {
              move[0] = drow1;
              move[1] = dcol1;
              break;
        } else if (_board[_row + drow2][_col + dcol2] == 0 
            && is_in_bounds(drow2, dcol2)) {
              move[0] = drow2;
              move[1] = dcol2;
              break;
        } break;
    case 6:
        drow1 = 0; dcol1 = 1;
        drow2 = -1; dcol2 = 0;
        if (_board[_row + drow1][_col + dcol1] == 0 
            && is_in_bounds(drow1, dcol1)) {
              move[0] = drow1;
              move[1] = dcol1;
              break;
        } else if (_board[_row + drow2][_col + dcol2] == 0 
            && is_in_bounds(drow2, dcol2)) {
              move[0] = drow2;
              move[1] = dcol2;
              break;
        } break;
    case 7:
        drow1 = 1; dcol1 = 1;
        drow2 = -1; dcol2 = 1;
        if (_board[_row + drow1][_col + dcol1] == 0 
            && is_in_bounds(drow1, dcol1)) {
              move[0] = drow1;
              move[1] = dcol1;
              break;
        } else if (_board[_row + drow2][_col + dcol2] == 0 
            && is_in_bounds(drow2, dcol2)) {
              move[0] = drow2;
              move[1] = dcol2;
              break;
        } break;
    case 8:
        drow1 = 1; dcol1 = 0;
        drow2 = 0; dcol2 = 1;
        if (_board[_row + drow1][_col + dcol1] == 0 
            && is_in_bounds(drow1, dcol1)) {
              move[0] = drow1;
              move[1] = dcol1;
              break;
        } else if (_board[_row + drow2][_col + dcol2] == 0 
            && is_in_bounds(drow2, dcol2)) {
              move[0] = drow2;
              move[1] = dcol2;
              break;
        } break;
    default:
      return 42;
  }
  Serial.print(" ");
  return move;
}

int Pixel::no_collisions(int dir) {
  int drow = _directions[dir][0];
  int dcol = _directions[dir][1];
  return !(_board[_row + drow][_col + dcol]);
}


int Pixel::is_in_bounds(int drow, int dcol) {
  return  0 <= _row + drow && _row + drow < _matrix->height()
          && 0 <= _col + dcol && _col + dcol < _matrix->height();
}

