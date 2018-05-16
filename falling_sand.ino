// FALLING SAND

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <avr/pgmspace.h> 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>
#include <math.h>
#include "Pixel.h"
  
Adafruit_LSM9DS0 accelerometer = Adafruit_LSM9DS0(1000);  // Use I2C, ID #1000

/* Direction Matrix */
#define N 1
#define NE 2
#define E 3
#define SE 4
#define S 5
#define SW 6
#define W 7
#define NW 8

int directions[9][2] = { {},  {+1, +0}, {+1, -1}, {+0, -1}, 
                              {+1, -1},           {-1, +0}, 
                              {-1, +1}, {+0, +1}, {+1, +1}  };
int curr_dir;

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

#define pixel_count 64
char board[32][32];
Pixel *pixels[pixel_count];

/* Button Pin */
int button = 3;
volatile int shapeNum = 0;

void setup_board() {
  for (int i = 0; i < pixel_count; i++) {
    Pixel *px = malloc(sizeof(Pixel));
    Serial.println(sizeof(Pixel));
    int row = i / 32;
    int col = i - row * 32;
    *px = Pixel(row, col, &matrix, (void*)board, curr_dir, 7,0,0);
    pixels[i] = px;
    px->move(curr_dir); 
  }

  if (shapeNum == 0) drawSquare();
  else if (shapeNum == 1) drawCircle();
  else if (shapeNum == 2) drawTriangle();
  else if (shapeNum == 3) drawV();
  else if (shapeNum == 4) drawX();
  else if (shapeNum == 5) drawTree();
  draw_board();
}

void setup() {
  Serial.begin(9600);
  matrix.begin();
  /* Button Initialization */
  pinMode(button, INPUT);
  attachInterrupt(digitalPinToInterrupt(button), resetBoard, FALLING);

  if(!accelerometer.begin()) {
      /* There was a problem detecting the LSM9DS0 ... check your connections */
    Serial.print(F("Ooops, no LSM9DS0 detected ... Check your wiring or I2C ADDR!"));
    while(!accelerometer.begin());
  }
  Serial.println(F("Found LSM9DS0 9DOF"));
  
  /* Setup the sensor gain and integration time */
  accelerometer.setupAccel(accelerometer.LSM9DS0_ACCELRANGE_2G);

  setup_board();
}

int map_angle(float angle) {

  if (22.5 <= angle && angle < 67.5) return SW;
  else if (67.5 <= angle && angle < 112.5) return S;
  else if (112.5 <= angle && angle < 157.5) return SE;
  else if (157.5<= angle && angle < 202.5) return E;
  else if (202.5 <= angle && angle < 247.5) return SE;
  else if (247.5 <= angle && angle < 292.5) return N;
  else if (292.5 <= angle && angle < 337.5) return NW;
  else return W;
}

void check_sensor() {
  /* Get a new sensor event */ 
  sensors_event_t accel, mag, gyro, temp;

  accelerometer.readAccel(); 
  float x_accel = -accelerometer.accelData.x;
  float y_accel = -accelerometer.accelData.y;
  float z_accel = -accelerometer.accelData.z;
  
  float angle = atan2(x_accel, y_accel) * 180.0/M_PI + 180.0;
  curr_dir = map_angle(angle);
  

  // Serial.print("  Direction: ");
  // Serial.println(curr_dir);
  // Serial.println("**********************\n");
}

void move_pixels() {
  check_sensor();
  for (int i = 0; i < pixel_count; i++) {
    Pixel *px = pixels[i];
    px->move(curr_dir);
  }
}

void drawSquare() {
  for (int r = 11; r <= 20; r++) {
    for (int c = 11; c <= 20; c++) {
      board[r][c] = 1;  
    }
  } 
}

void drawCircle() {
  Serial.println(shapeNum);
  int r = 8;
  int cx = 31/2;
  int cy = 31/2;
  for (int row = 0; row < 32; row++) {
    for (int col = 0; col < 32; col++) {
      if (((row - cx) * (row - cx)) + ((col - cy) * (col - cy)) <= r * r) board[row][col] = 1;
    }
  }
}


void drawTriangle() {
  Serial.println(shapeNum);
  int c = 15;
  int upperBound = 16;
  for (int r = 8; r <= 24; r++) {
    for (c; c <= upperBound; c++) {
      board[r][c] = 1;
    }
    c = c - 1;
    upperBound = upperBound + 1;
  }
}

void drawV() {
  int diff = 16;
  for (int r = 0; r < 16; r++) {
    if (8 > r) continue;
    else {
      board[r][r] = 1;
      board[r][r+diff] = 1;
    }
    diff = diff - 2;
  }
}

void drawX() {
  int diff = 16;
  for (int r = 8; r <= 16; r++) {
    board[r][r] = 1;
    board[r+diff][r] = 1;
    board[r][r+diff] = 1;
    board[r+diff][r+diff] = 1;
    diff = diff - 2;
  }
}

void drawTree() {
  int col = 15;
  for (int r = 0; r < 32; r++) {
    if (8 > r || r > 24) continue;
    else {
      board[r][col] = 1;
      board[r][col - r%9] = 1;
      board[r+1][col - r%9] = 1;
      board[r][col + r%9] = 1;
      board[r+1][col + r%9] = 1;
    }
  }
}

void draw_board() {
  for (int row = 0; row < 32; row++) {
    for (int col = 0; col < 32; col++) {
      if (board[row][col] == 1) matrix.drawPixel(col, row, matrix.Color333(0,0,7));
    }
  }
}

void loop() {
  move_pixels();
  delay(50);
}

void resetBoard() {
  delay(500);
  for (int r = 0; r < 32; r++) {
    for (int c = 0; c < 32; c++) {
      board[r][c] = 0;
    }
  }
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  shapeNum = shapeNum + 1;
  if (shapeNum > 5) shapeNum = 0;
  for (int i = 0; i < pixel_count; i++) {
    free(pixels[i]);
  }
  setup_board();
}

