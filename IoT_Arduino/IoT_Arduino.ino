#include "CurieIMU.h"
#include "CuriePME.h"
#include <CurieBLE.h>
//#include "MemoryFree.h"
#include <avr/pgmspace.h>

const unsigned int sampleRateHZ = 100;

const unsigned int vectorNumBytes = 128;

const unsigned int samplesPerVector = (vectorNumBytes / 3);


const unsigned int sensorBufSize = 2048;
const int IMULow = -32768;
const int IMUHigh = 32767;


BLEPeripheral blePeripheral;
BLEService fitnessService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEUnsignedCharCharacteristic fitnessTypeChar("19B10001-E8F2-537E-4F6C-D104768A1214",
    BLERead | BLENotify);

void setup()
{
  Serial.begin(9600);
  //while (!Serial);
  Serial.println("INIT COMPL");

  CurieIMU.begin();
  CuriePME.begin();

  CurieIMU.setAccelerometerRate(sampleRateHZ);
  CurieIMU.setAccelerometerRange(1);

  //CuriePME.setClassifierMode(CuriePME.KNN_Mode);

  /*category 1 = still*/
  static byte subject102_1_0[] PROGMEM = {60, 60, 60, 60, 60, 60, 59, 59, 59, 59, 59, 60, 60, 59, 60, 59, 59, 60, 59, 60, 60, 59, 60, 60, 59, 60, 59, 60, 60, 59, 60, 60, 59, 60, 60, 60, 59, 59, 59, 59, 59, 60, 60, 59, 59, 59, 60, 60, 60, 59, 59, 59, 60, 60, 59, 59, 59, 59, 59, 60, 60, 59, 60, 60, 60, 59, 59, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 59, 60, 60, 60, 60, 60, 60, 60, 60, 59, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 59, 60, 60, 60, 60, 59, 59, 59, 59, 58, 60, 60, 59, 59, 59, 59, 59, 59, 59, 60};
  CuriePME.learn(subject102_1_0, 120, 1);
  static byte subject102_1_1[] PROGMEM = {59, 59, 60, 60, 59, 60, 60, 59, 59, 60, 60, 60, 59, 60, 60, 60, 60, 61, 60, 56, 61, 61, 60, 59, 59, 59, 60, 60, 60, 59, 59, 60, 60, 60, 60, 60, 60, 59, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 59, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 59, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 59, 59, 60, 60, 60, 60, 59, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 59, 60, 60, 60, 60, 60, 59, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 59, 60, 60, 60};
  CuriePME.learn(subject102_1_1, 120, 1);

  /*category = 3  walk*/
  static byte subject102_3_0[] PROGMEM = {64, 68, 70, 69, 68, 69, 70, 69, 64, 59, 59, 61, 64, 50, 33, 24, 28, 27, 25, 43, 57, 51, 36, 22, 6, 54, 90, 85, 88, 92, 85, 83, 87, 89, 91, 93, 94, 91, 84, 74, 57, 23, 1, 64, 123, 73, 2, 0, 139, 156, 44, 1, 50, 82, 79, 71, 59, 60, 71, 79, 69, 69, 61, 69, 69, 67, 64, 59, 70, 71, 60, 31, 23, 22, 25, 29, 43, 54, 44, 33, 38, 24, 54, 76, 80, 75, 93, 86, 84, 82, 89, 90, 94, 98, 98, 88, 62, 29, 0, 20, 118, 142, 57, 0, 9, 131, 114, 34, 38, 57, 78, 75, 68, 65, 63, 70, 67, 73, 68, 67};
  CuriePME.learn(subject102_3_0, 120, 3);
  static byte subject102_3_1[] PROGMEM = {62, 46, 34, 29, 31, 26, 30, 45, 55, 50, 44, 34, 0, 46, 78, 80, 87, 93, 97, 88, 95, 101, 102, 104, 100, 91, 65, 11, 0, 50, 126, 135, 45, 0, 27, 126, 110, 34, 40, 66, 76, 69, 72, 68, 60, 67, 75, 73, 69, 65, 69, 64, 69, 62, 51, 61, 61, 45, 33, 32, 41, 32, 31, 40, 48, 46, 38, 29, 27, 38, 77, 70, 83, 95, 95, 90, 91, 96, 99, 100, 100, 91, 62, 6, 0, 62, 148, 148, 56, 0, 4, 142, 129, 37, 31, 44, 72, 78, 74, 66, 59, 65, 67, 75, 69, 67, 71, 71, 64, 60, 60, 68, 65, 49, 32, 24, 26, 32, 30, 41};
  CuriePME.learn(subject102_3_1, 120, 3);
  static byte subject102_3_2[] PROGMEM = {77, 77, 80, 80, 83, 86, 93, 91, 94, 94, 96, 97, 88, 58, 14, 0, 46, 130, 148, 59, 0, 4, 143, 132, 38, 33, 53, 69, 78, 71, 62, 62, 72, 77, 74, 60, 63, 70, 68, 64, 58, 62, 69, 56, 38, 30, 30, 28, 21, 31, 49, 53, 44, 37, 18, 26, 83, 83, 83, 89, 85, 88, 86, 96, 100, 103, 103, 101, 85, 49, 5, 0, 85, 146, 86, 0, 0, 142, 129, 33, 24, 53, 76, 78, 76, 66, 60, 64, 71, 76, 71, 67, 70, 68, 65, 62, 54, 58, 65, 60, 44, 32, 27, 31, 35, 39, 47, 49, 42, 40, 24, 33, 73, 69, 80, 81, 82, 83, 88, 93};
  CuriePME.learn(subject102_3_2, 120, 3);
  static byte subject102_3_3[] PROGMEM = {34, 0, 25, 106, 132, 73, 0, 1, 126, 145, 43, 26, 47, 72, 85, 70, 64, 62, 70, 69, 75, 70, 66, 66, 61, 59, 62, 66, 66, 61, 42, 31, 27, 29, 28, 36, 48, 49, 36, 30, 27, 43, 76, 78, 92, 86, 93, 90, 94, 97, 102, 102, 101, 96, 69, 33, 0, 14, 110, 129, 57, 0, 34, 125, 96, 25, 44, 64, 70, 76, 72, 72, 63, 74, 67, 68, 66, 66, 69, 68, 64, 56, 59, 65, 61, 41, 26, 21, 28, 29, 41, 53, 49, 42, 24, 30, 43, 72, 72, 82, 83, 93, 81, 90, 95, 99, 102, 104, 98, 75, 41, 3, 3, 75, 135, 114, 35, 0, 43, 148};
  CuriePME.learn(subject102_3_3, 120, 3);
  static byte subject102_3_4[] PROGMEM = {70, 65, 60, 67, 74, 71, 62, 66, 68, 69, 67, 63, 55, 65, 62, 53, 34, 31, 28, 26, 30, 43, 52, 51, 44, 22, 19, 63, 77, 74, 84, 82, 83, 89, 90, 98, 100, 103, 103, 99, 67, 21, 0, 26, 129, 159, 87, 2, 0, 101, 134, 68, 38, 47, 67, 87, 72, 70, 59, 68, 68, 74, 69, 67, 69, 70, 68, 63, 58, 65, 62, 52, 36, 35, 37, 28, 27, 32, 47, 44, 38, 33, 13, 47, 72, 76, 79, 91, 86, 87, 91, 92, 100, 103, 97, 111, 86, 49, 0, 0, 80, 147, 123, 45, 0, 40, 138, 103, 38, 44, 60, 72, 77, 67, 69, 58, 73, 74, 72, 67};
  CuriePME.learn(subject102_3_4, 120, 3);
  static byte subject102_3_5[] PROGMEM = {60, 61, 50, 19, 13, 12, 29, 35, 59, 62, 58, 56, 24, 35, 70, 79, 72, 70, 79, 87, 76, 98, 94, 98, 102, 103, 100, 85, 57, 0, 2, 82, 126, 71, 5, 10, 121, 104, 26, 40, 66, 81, 67, 72, 70, 67, 67, 70, 76, 75, 68, 71, 69, 70, 62, 59, 56, 64, 59, 47, 33, 31, 31, 26, 21, 42, 57, 56, 48, 26, 10, 48, 73, 78, 85, 90, 85, 86, 87, 84, 109, 102, 104, 99, 77, 46, 0, 2, 89, 144, 97, 14, 0, 109, 141, 54, 18, 45, 69, 81, 72, 71, 61, 58, 79, 73, 70, 64, 72, 70, 65, 64, 66, 68, 61, 50, 33, 38, 28, 30};
  CuriePME.learn(subject102_3_5, 120, 3);
  static byte subject102_3_6[] PROGMEM = {31, 53, 67, 68, 86, 91, 95, 87, 84, 87, 91, 90, 85, 68, 51, 52, 57, 72, 66, 33, 0, 31, 125, 59, 49, 25, 56, 70, 68, 57, 67, 69, 69, 69, 68, 68, 69, 70, 70, 69, 70, 66, 65, 63, 64, 69, 66, 58, 56, 56, 43, 17, 16, 34, 49, 55, 63, 35, 35, 52, 76, 73, 73, 83, 77, 76, 76, 81, 87, 90, 89, 83, 71, 52, 31, 18, 58, 108, 137, 73, 0, 0, 108, 131, 43, 28, 53, 74, 77, 70, 63, 58, 70, 68, 69, 69, 68, 68, 68, 68, 66, 61, 60, 58, 61, 55, 41, 43, 39, 32, 26, 30, 47, 52, 45, 48, 42, 19, 45, 67};
  CuriePME.learn(subject102_3_6, 120, 3);
  static byte subject102_3_7[] PROGMEM = {61, 32, 16, 44, 116, 123, 68, 0, 0, 61, 160, 110, 17, 6, 58, 70, 58, 50, 65, 58, 61, 64, 69, 76, 73, 70, 75, 66, 52, 64, 73, 72, 51, 40, 47, 47, 37, 48, 60, 60, 62, 65, 74, 81, 79, 74, 64, 43, 35, 43, 60, 68, 66, 58, 63, 70, 72, 68, 68, 66, 68, 69, 69, 70, 73, 77, 71, 60, 34, 40, 69, 78, 60, 52, 57, 60, 60, 68, 75, 68, 58, 63, 70, 70, 65, 63, 62, 63, 63, 65, 64, 58, 56, 61, 62, 52, 42, 29, 25, 25, 30, 44, 57, 53, 55, 50, 31, 47, 74, 75, 80, 66, 85, 83, 84, 71, 96, 84, 85, 87};
  CuriePME.learn(subject102_3_7, 120, 3);
  static byte subject102_3_8[] PROGMEM = {88, 130, 98, 42, 0, 15, 165, 139, 30, 15, 37, 77, 83, 79, 60, 63, 64, 83, 75, 72, 64, 68, 72, 68, 65, 57, 57, 64, 63, 45, 34, 34, 42, 34, 24, 39, 45, 41, 33, 26, 24, 53, 72, 71, 80, 90, 79, 91, 85, 89, 95, 93, 96, 99, 91, 70, 44, 0, 2, 105, 157, 117, 31, 0, 53, 157, 100, 15, 34, 60, 78, 81, 75, 59, 59, 65, 78, 72, 64, 68, 71, 67, 69, 63, 57, 63, 64, 48, 31, 32, 36, 34, 33, 40, 44, 42, 30, 37, 4, 49, 66, 80, 101, 91, 90, 87, 85, 101, 95, 100, 100, 96, 82, 61, 4, 0, 101, 137, 78, 0};
  CuriePME.learn(subject102_3_8, 120, 3);
  static byte subject102_3_9[] PROGMEM = {79, 83, 79, 67, 64, 67, 79, 70, 72, 71, 71, 72, 70, 62, 58, 54, 64, 61, 53, 27, 14, 19, 21, 31, 51, 49, 45, 47, 12, 38, 85, 85, 88, 87, 86, 89, 103, 94, 99, 95, 98, 93, 80, 54, 21, 0, 59, 107, 95, 25, 0, 59, 114, 74, 47, 55, 72, 78, 77, 67, 64, 55, 76, 73, 77, 66, 66, 70, 71, 73, 69, 64, 59, 60, 58, 47, 33, 25, 27, 26, 25, 40, 62, 58, 50, 25, 30, 24, 77, 89, 76, 82, 84, 87, 107, 90, 99, 99, 103, 100, 86, 56, 3, 0, 89, 140, 116, 31, 0, 32, 138, 113, 35, 41, 63, 78, 81, 75, 60, 56};
  CuriePME.learn(subject102_3_9, 120, 3);
  static byte subject102_3_10[] PROGMEM = {69, 66, 66, 70, 58, 56, 68, 73, 72, 68, 66, 65, 64, 68, 68, 62, 63, 59, 59, 58, 53, 41, 26, 27, 27, 37, 50, 67, 77, 74, 67, 42, 26, 25, 61, 70, 70, 67, 88, 88, 106, 96, 99, 96, 91, 82, 66, 38, 17, 26, 77, 107, 94, 45, 0, 31, 104, 100, 47, 44, 73, 77, 67, 63, 68, 56, 65, 67, 74, 71, 66, 65, 66, 67, 65, 61, 62, 61, 62, 65, 56, 48, 42, 40, 40, 35, 33, 45, 60, 61, 57, 49, 45, 5, 50, 65, 67, 77, 79, 81, 92, 89, 98, 100, 94, 94, 82, 61, 37, 15, 11, 78, 124, 107, 61, 0, 1, 97, 132, 67};
  CuriePME.learn(subject102_3_10, 120, 3);
  static byte subject102_3_11[] PROGMEM = {54, 60, 72, 73, 72, 66, 62, 64, 68, 67, 65, 63, 61, 63, 63, 56, 51, 49, 49, 50, 44, 38, 41, 43, 51, 53, 50, 35, 37, 22, 59, 65, 74, 85, 81, 86, 86, 88, 93, 93, 88, 86, 82, 70, 52, 59, 36, 41, 86, 107, 69, 5, 0, 83, 141, 96, 24, 42, 67, 78, 70, 66, 58, 55, 72, 74, 72, 65, 64, 68, 70, 68, 69, 65, 59, 64, 65, 56, 50, 46, 51, 50, 41, 39, 37, 39, 50, 51, 44, 46, 41, 6, 73, 57, 76, 87, 90, 79, 82, 90, 95, 99, 96, 87, 71, 57, 49, 30, 43, 95, 77, 18, 0, 87, 132, 77, 35, 39, 60, 77};
  CuriePME.learn(subject102_3_11, 120, 3);
  static byte subject102_3_12[] PROGMEM = {51, 50, 46, 32, 43, 63, 58, 68, 73, 74, 76, 86, 94, 97, 94, 96, 96, 92, 84, 65, 36, 0, 24, 98, 118, 63, 0, 15, 123, 124, 31, 45, 59, 75, 82, 69, 65, 55, 66, 78, 68, 71, 68, 74, 70, 68, 65, 61, 64, 64, 51, 47, 41, 35, 34, 26, 28, 42, 50, 52, 50, 43, 28, 55, 62, 72, 75, 80, 95, 88, 91, 98, 103, 103, 101, 91, 66, 30, 0, 33, 108, 113, 54, 0, 21, 119, 101, 33, 47, 59, 72, 73, 69, 65, 62, 70, 71, 68, 70, 73, 69, 69, 69, 61, 60, 60, 62, 57, 55, 49, 41, 37, 39, 38, 40, 42, 41, 44, 44, 35};
  CuriePME.learn(subject102_3_12, 120, 3);
  static byte subject102_3_13[] PROGMEM = {59, 56, 42, 36, 8, 72, 59, 59, 74, 88, 97, 96, 94, 99, 99, 96, 91, 86, 71, 56, 19, 0, 48, 92, 72, 16, 18, 111, 120, 37, 39, 48, 70, 77, 65, 63, 67, 66, 70, 65, 62, 62, 66, 69, 65, 60, 58, 61, 52, 45, 41, 32, 23, 32, 30, 46, 55, 40, 17, 33, 28, 78, 84, 82, 92, 98, 96, 91, 91, 92, 92, 87, 79, 64, 48, 22, 11, 77, 137, 126, 62, 0, 0, 151, 172, 51, 7, 27, 80, 87, 72, 61, 57, 63, 73, 71, 64, 60, 63, 64, 64, 55, 56, 59, 55, 42, 39, 35, 32, 38, 35, 40, 48, 42, 25, 25, 35, 78, 80, 83};
  CuriePME.learn(subject102_3_13, 120, 3);
  static byte subject102_3_14[] PROGMEM = {0, 57, 160, 112, 22, 11, 53, 77, 85, 63, 65, 58, 75, 70, 65, 63, 62, 64, 65, 62, 66, 59, 54, 57, 57, 52, 41, 30, 20, 27, 30, 46, 45, 36, 30, 14, 44, 55, 66, 79, 85, 100, 90, 86, 92, 95, 94, 96, 92, 86, 68, 51, 22, 2, 62, 119, 104, 26, 0, 61, 144, 83, 28, 31, 65, 72, 69, 69, 60, 62, 75, 69, 69, 61, 62, 64, 62, 60, 62, 59, 66, 58, 48, 44, 35, 30, 30, 26, 38, 54, 54, 49, 30, 10, 33, 61, 75, 81, 89, 95, 90, 89, 87, 96, 100, 96, 93, 81, 59, 33, 1, 21, 94, 123, 83, 10, 0, 83, 123, 56};
  CuriePME.learn(subject102_3_14, 120, 3);
  static byte subject102_3_15[] PROGMEM = {62, 77, 73, 77, 71, 58, 60, 55, 58, 54, 58, 58, 44, 44, 30, 34, 50, 54, 29, 25, 25, 19, 39, 59, 87, 92, 89, 90, 93, 92, 105, 109, 109, 113, 98, 68, 4, 0, 51, 118, 108, 44, 1, 43, 100, 98, 42, 37, 62, 66, 60, 67, 74, 82, 70, 74, 73, 71, 66, 59, 55, 54, 50, 55, 50, 52, 50, 32, 34, 46, 34, 31, 13, 25, 29, 45, 79, 73, 85, 95, 102, 100, 101, 100, 101, 105, 108, 94, 50, 0, 1, 76, 118, 89, 25, 25, 78, 109, 69, 20, 54, 74, 65, 58, 70, 74, 76, 75, 75, 72, 59, 51, 59, 63, 52, 59, 50, 45, 52, 35};
  CuriePME.learn(subject102_3_15, 120, 3);
  static byte subject102_3_16[] PROGMEM = {95, 93, 96, 103, 107, 107, 77, 30, 0, 18, 85, 67, 49, 82, 93, 78, 38, 25, 55, 81, 76, 72, 59, 58, 71, 82, 74, 69, 65, 57, 54, 52, 59, 59, 59, 47, 37, 24, 31, 42, 58, 25, 15, 26, 30, 41, 50, 89, 91, 97, 97, 97, 93, 98, 103, 107, 109, 95, 57, 0, 0, 70, 109, 76, 38, 51, 78, 93, 46, 35, 63, 76, 73, 68, 63, 65, 75, 82, 72, 69, 65, 50, 56, 61, 54, 52, 50, 21, 16, 22, 45, 59, 34, 41, 38, 35, 28, 44, 71, 98, 106, 96, 94, 95, 103, 109, 110, 110, 102, 72, 1, 4, 61, 73, 24, 29, 86, 103, 91, 15};
  CuriePME.learn(subject102_3_16, 120, 3);

  /*category 2 =  sit*/
  static byte subject102_2_0[] PROGMEM = {128, 130, 129, 129, 129, 129, 130, 130, 130, 129, 128, 127, 127, 128, 129, 130, 131, 132, 132, 131, 131, 131, 131, 132, 131, 131, 131, 131, 131, 131, 132, 131, 132, 132, 132, 132, 131, 131, 131, 130, 131, 132, 132, 132, 131, 130, 130, 130, 131, 131, 131, 132, 131, 131, 131, 131, 132, 130, 131, 130, 131, 131, 131, 132, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 132, 131, 131, 131, 132, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131, 131};
  CuriePME.learn(subject102_2_0, 120, 2);

  /*category 4 = run*/
  static byte subject102_4_0[] PROGMEM = {19, 16, 50, 40, 79, 102, 126, 131, 127, 92, 75, 46, 57, 133, 212, 0, 0, 101, 87, 16, 0, 12, 58, 18, 5, 49, 64, 98, 120, 110, 113, 121, 119, 66, 0, 0, 4, 10, 66, 77, 93, 111, 130, 130, 123, 96, 73, 53, 109, 186, 175, 0, 5, 255, 144, 0, 1, 14, 67, 12, 18, 65, 84, 116, 133, 110, 106, 112, 114, 32, 0, 6, 15, 16, 53, 84, 114, 140, 142, 133, 105, 79, 66, 70, 132, 128, 0, 36, 79, 33, 0, 0, 32, 13, 36, 65, 66, 95, 120, 116, 111, 114, 117, 114, 0, 1, 1, 49, 49, 71, 74, 126, 147, 150, 127, 92, 75, 25, 63, 108};
  CuriePME.learn(subject102_4_0, 120, 4);
  static byte subject102_4_1[] PROGMEM = {1, 9, 22, 64, 78, 87, 112, 115, 126, 113, 113, 98, 8, 0, 0, 0, 22, 61, 89, 116, 144, 146, 142, 116, 85, 58, 31, 102, 133, 45, 0, 93, 70, 0, 0, 0, 8, 32, 74, 94, 96, 114, 123, 107, 113, 109, 119, 69, 0, 0, 0, 0, 30, 54, 96, 124, 145, 151, 138, 110, 82, 38, 68, 135, 67, 0, 64, 70, 7, 0, 3, 8, 26, 68, 76, 81, 107, 114, 125, 124, 105, 108, 33, 0, 0, 11, 0, 37, 58, 108, 125, 134, 150, 141, 104, 70, 28, 42, 66, 105, 117, 58, 0, 0, 1, 9, 0, 34, 72, 81, 94, 114, 114, 114, 117, 110, 114, 37, 0, 0};
  CuriePME.learn(subject102_4_1, 120, 4);
  static byte subject102_4_2[] PROGMEM = {145, 156, 145, 123, 84, 43, 40, 67, 71, 74, 56, 0, 11, 7, 0, 7, 28, 75, 86, 90, 114, 118, 114, 119, 114, 107, 98, 0, 0, 0, 0, 1, 63, 62, 101, 122, 152, 162, 130, 102, 63, 33, 58, 163, 24, 1, 115, 98, 4, 0, 0, 25, 33, 41, 72, 88, 101, 119, 110, 122, 125, 109, 108, 66, 0, 0, 0, 0, 44, 53, 50, 100, 112, 131, 155, 142, 99, 71, 30, 55, 66, 0, 94, 84, 32, 17, 19, 23, 47, 81, 62, 61, 70, 87, 94, 98, 97, 90, 68, 54, 38, 7, 29, 31, 21, 0, 1, 0, 67, 54, 81, 99, 109, 118, 121, 122, 123, 112, 84, 38};
  CuriePME.learn(subject102_4_2, 120, 4);
  static byte subject102_4_3[] PROGMEM = {22, 0, 108, 103, 0, 0, 22, 44, 3, 13, 52, 69, 90, 114, 115, 91, 66, 108, 128, 44, 0, 0, 6, 13, 71, 82, 110, 133, 139, 134, 105, 88, 70, 80, 143, 59, 9, 50, 62, 0, 0, 0, 9, 28, 47, 68, 81, 104, 119, 113, 111, 115, 108, 98, 58, 0, 0, 0, 1, 72, 69, 93, 102, 139, 166, 146, 103, 91, 62, 41, 119, 179, 0, 0, 138, 106, 0, 0, 0, 0, 13, 59, 64, 80, 99, 112, 108, 121, 128, 101, 75, 27, 0, 0, 0, 9, 76, 81, 113, 137, 158, 149, 120, 81, 68, 57, 113, 129, 2, 6, 111, 22, 3, 0, 0, 0, 31, 58, 74, 79};
  CuriePME.learn(subject102_4_3, 120, 4);
  static byte subject102_4_4[] PROGMEM = {69, 4, 0, 0, 0, 26, 45, 95, 131, 148, 154, 165, 140, 89, 73, 48, 52, 167, 14, 0, 99, 68, 0, 0, 0, 3, 35, 57, 66, 75, 98, 118, 108, 119, 123, 92, 31, 0, 0, 0, 0, 13, 88, 109, 150, 160, 154, 135, 102, 103, 56, 57, 152, 0, 1, 140, 39, 0, 0, 0, 0, 36, 59, 80, 82, 106, 116, 109, 133, 126, 84, 12, 0, 0, 0, 0, 10, 64, 104, 136, 155, 168, 146, 107, 94, 49, 0, 11, 248, 105, 0, 0, 0, 43, 0, 12, 61, 59, 77, 99, 118, 113, 123, 114, 103, 66, 19, 0, 0, 0, 0, 52, 90, 120, 146, 159, 159, 146, 83, 68};
  CuriePME.learn(subject102_4_4, 120, 4);
  static byte subject102_4_5[] PROGMEM = {204, 0, 1, 0, 0, 33, 72, 89, 75, 95, 110, 115, 127, 132, 99, 74, 27, 0, 0, 0, 0, 18, 66, 114, 131, 151, 168, 155, 112, 91, 70, 54, 68, 184, 0, 12, 171, 7, 1, 0, 1, 12, 38, 89, 92, 105, 116, 112, 109, 135, 118, 86, 53, 0, 0, 0, 0, 0, 45, 52, 113, 143, 168, 176, 135, 70, 83, 66, 51, 188, 9, 3, 95, 150, 13, 0, 0, 17, 50, 44, 67, 91, 88, 105, 112, 115, 122, 127, 101, 39, 0, 16, 50, 0, 14, 44, 63, 109, 118, 133, 146, 134, 105, 77, 53, 65, 62, 0, 68, 145, 35, 0, 5, 50, 47, 65, 64, 70, 94, 86};
  CuriePME.learn(subject102_4_5, 120, 4);
  static byte subject102_4_6[] PROGMEM = {32, 72, 99, 124, 135, 153, 140, 97, 81, 60, 103, 166, 0, 38, 138, 34, 0, 10, 3, 4, 4, 23, 63, 81, 85, 90, 105, 114, 116, 111, 104, 77, 27, 0, 0, 5, 36, 26, 83, 105, 138, 137, 141, 142, 115, 72, 52, 94, 92, 70, 78, 48, 0, 3, 57, 0, 0, 11, 56, 82, 84, 95, 105, 107, 134, 124, 111, 115, 81, 13, 0, 0, 7, 88, 42, 74, 101, 127, 126, 146, 150, 118, 89, 46, 68, 136, 181, 0, 13, 234, 55, 0, 0, 16, 24, 0, 21, 84, 81, 103, 114, 103, 120, 123, 120, 107, 107, 78, 0, 0, 0, 6, 52, 69, 90, 110, 135, 152, 162, 143};
  CuriePME.learn(subject102_4_6, 120, 4);
  static byte subject102_4_7[] PROGMEM = {172, 153, 155, 126, 89, 76, 38, 25, 12, 24, 67, 35, 0, 0, 2, 56, 109, 93, 117, 126, 116, 117, 124, 100, 69, 0, 0, 0, 0, 0, 59, 51, 93, 130, 147, 140, 172, 162, 136, 81, 43, 3, 76, 45, 46, 27, 10, 0, 0, 0, 83, 94, 105, 136, 127, 115, 114, 125, 99, 78, 0, 0, 0, 0, 7, 53, 89, 100, 130, 144, 150, 148, 163, 138, 101, 84, 42, 0, 3, 73, 86, 36, 0, 0, 3, 21, 100, 119, 129, 115, 122, 115, 113, 103, 90, 8, 0, 0, 0, 0, 57, 67, 103, 118, 144, 139, 160, 159, 144, 114, 96, 56, 0, 0, 104, 117, 47, 0, 0, 12};
  CuriePME.learn(subject102_4_7, 120, 4);
  static byte subject102_4_8[] PROGMEM = {126, 109, 105, 100, 46, 0, 0, 0, 0, 0, 50, 48, 92, 114, 136, 157, 142, 130, 105, 96, 14, 3, 92, 82, 34, 2, 7, 0, 12, 58, 65, 94, 100, 117, 98, 107, 106, 102, 64, 24, 0, 0, 3, 13, 27, 37, 59, 55, 63, 90, 117, 129, 140, 130, 113, 76, 11, 0, 6, 54, 96, 61, 32, 3, 3, 35, 79, 83, 82, 75, 83, 83, 77, 72, 72, 68, 59, 55, 47, 49, 59, 70, 46, 20, 1, 22, 31, 53, 57, 62, 73, 81, 86, 89, 78, 77, 68, 64, 58, 65, 75, 65, 56, 28, 9, 37, 42, 61, 61, 64, 64, 63, 65, 69, 70, 67, 61, 61, 60, 65};
  CuriePME.learn(subject102_4_8, 120, 4);
  static byte subject102_4_9[] PROGMEM = {53, 54, 54, 52, 54, 58, 58, 57, 62, 58, 44, 45, 41, 56, 64, 75, 70, 74, 81, 85, 93, 90, 82, 72, 62, 68, 77, 82, 84, 86, 109, 80, 26, 17, 66, 81, 74, 68, 61, 60, 49, 33, 17, 9, 15, 26, 43, 68, 81, 91, 109, 130, 116, 114, 106, 68, 4, 3, 0, 0, 0, 24, 47, 44, 81, 94, 122, 146, 151, 142, 123, 107, 90, 100, 64, 15, 0, 67, 64, 9, 0, 0, 0, 3, 62, 78, 108, 123, 127, 136, 115, 114, 99, 74, 6, 0, 0, 0, 25, 64, 70, 89, 123, 139, 154, 156, 147, 119, 110, 59, 85, 97, 35, 0, 1, 28, 11, 0, 0, 41};
  CuriePME.learn(subject102_4_9, 120, 4);

  // Serial.println(freeMemory());

  blePeripheral.setLocalName("Activity");
  blePeripheral.setAdvertisedServiceUuid(fitnessService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(fitnessService);
  blePeripheral.addAttribute(fitnessTypeChar);

  // assign event handlers for connected, disconnected to peripheral
  fitnessTypeChar.setValue(20);

  // advertise the service
  blePeripheral.begin();

  Serial.println("Bluetooth device active, waiting for connections...");

}



void loop ()
{
  blePeripheral.poll();
  BLECentral central = blePeripheral.central();

  byte vector[120];
  unsigned int category;

  if (central) {
    Serial.print(F("Connected to central: "));
    Serial.println(central.address());

    while (central.connected()) {
      readVectorFromIMU(vector);
      category = CuriePME.classify(vector, 120);

      switch (category) {
        case 1:
          Serial.println(F("Still"));
          break;
        case 2:
          Serial.println(F("Sit"));
          break;
        case 3:
          Serial.println(F("Walk"));
          break;
        case 4:
          Serial.println(F("Run"));
          break;
        default:
          Serial.println(F("Unknown"));
      }

      char bigstring[370];
      bigstring[0] = 0;
      int i;
      for (i = 0; i < 120; i++) {
        Serial.print(vector[i]);
        Serial.print(", ");
      }
      Serial.println();

      fitnessTypeChar.setValue((byte) category);
      Serial.println(fitnessTypeChar.value());
      
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}

void undersample(byte samples[], int numSamples, byte vector[])
{
  int i, j;
  for (i = 0; i < 120; i++) {
    j = i * 2;
    vector[i] = samples[j];
  }
}

void readVectorFromIMU(byte vector[])
{
  byte accel[250];
  int raw[3];

  unsigned int samples = 0;
  unsigned int i = 0;

  while (samples < 250) {
    if (CurieIMU.dataReady()) {

      CurieIMU.readAccelerometer(raw[0], raw[1], raw[2]);

      /* Map raw values to 0-255 */
      accel[i] = (byte) map(raw[0], IMULow, IMUHigh, 0, 255);
      // accel[i + 1] = (byte) map(raw[1], IMULow, IMUHigh, 0, 255);
      // accel[i + 2] = (byte) map(raw[2], IMULow, IMUHigh, 0, 255);

      i += 1;
      ++samples;
    }
  }
  undersample(accel, samples, vector);
}