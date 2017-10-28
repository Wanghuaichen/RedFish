#ifndef HMCH
#define HMCH
#include "Fish.h"

#define MAG_ADDRESS			0x3C
#define MAG_DATA_REGISTER	0x03
#define ConfigRegA           0x00
#define ConfigRegB           0x01
#define magGain              0x40
#define PositiveBiasConfig   0x11
#define NegativeBiasConfig   0x12
#define NormalOperation      0x10
#define ModeRegister         0x02
#define ContinuousConversion 0x00
#define SingleConversion     0x01

// ConfigRegA valid sample averaging for 5883L
#define SampleAveraging_1    0x00
#define SampleAveraging_2    0x01
#define SampleAveraging_4    0x02
#define SampleAveraging_8    0x03

// ConfigRegA valid data output rates for 5883L
#define DataOutputRate_0_75HZ 0x00
#define DataOutputRate_1_5HZ  0x01
#define DataOutputRate_3HZ    0x02
#define DataOutputRate_7_5HZ  0x03
#define DataOutputRate_15HZ   0x04
#define DataOutputRate_30HZ   0x05
#define DataOutputRate_75HZ   0x06

#define OFFSET_X -12.6226
#define OFFSET_Y -48.4424
#define OFFSET_Z 0.0
#define Ellipse_To_Round_XY 1.046241736
#define Ellipse_To_Round_YZ 0.53283767

/*º¯ÊýÉêÃ÷*/
extern void HMCInitialize(void);
extern void HMCReadData(void);

extern u8 HMCData[6];

#endif
