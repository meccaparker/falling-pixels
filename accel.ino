#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>
#include <math.h>


#define MECBRISAM 
/* Assign a unique base ID for this sensor */   
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(MECBRISAM);  // Use I2C, ID #MECBRISAM


/* Direction Matrix */

#define N 1
#define NE 2
#define E 3
#define SE 4
#define S 5
#define SW 6
#define W 7
#define NW 8

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void) {
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  Serial.println(F("LSM9DS0 9DOF Sensor Test")); Serial.println("");
  
  /* Initialise the sensor */
  if(!lsm.begin()) {
    Serial.print("brooo");
    /* There was a problem detecting the LSM9DS0 ... check your connections */
    Serial.print(F("Ooops, no LSM9DS0 detected ... Check your wiring or I2C ADDR!"));
    while(!lsm.begin());
  }
  Serial.println(F("Found LSM9DS0 9DOF"));
  
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);

  Serial.println("");
}


/* Map accelerometer angle to direction in the direction matrix */
int map_angle(float angle) {
  if (0.0 <= angle && angle < 45.0) return NE;
  else if (45.0 <= angle && angle < 90.0) return N;
  else if (90.0 <= angle && angle < 135.0) return NW;
  else if (135.0 <= angle && angle < 180.0) return W;
  else if (180.0 <= angle && angle < 225.0) return SW;
  else if (225.0 <= angle && angle < 270.0) return S;
  else if (270.0 <= angle && angle < 315.0) return SE;
  else return E;
}

void check_sensor() {
  /* Get a new accelerometer event */ 
  sensors_event_t accel, mag, gyro, temp;

  lsm.getEvent(&accel, &mag, &gyro, &temp); 
  float x_accel = -accel.acceleration.x;
  float y_accel = -accel.acceleration.y;
  float z_accel = -accel.acceleration.z;

  float angle = atan2(x_accel, y_accel) * 180.0/M_PI;
  int dir = map_angle(angle);

  Serial.print("Direction: ");
  Serial.println(dir);
  Serial.println("**********************\n");
}

void loop(void) {  
  check_sensor();
  delay(500);
}