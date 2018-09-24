#ifndef __AirFlowSensor__
#define __AirFlowSensor__
/*****************************************************************************

DESCRIPTION: <title>

<text>

--------------------------------------------------------------------------

© Copyright 2015 

*****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

/*--------------------  I n c l u d e   F i l e s  -------------------------*/

/*--------------------  C o n s t a n t s  ---------------------------------*/

#define SCALE 1000

/*--------------------  T y p e s  -----------------------------------------*/

/*--------------------  V a r i a b l e s  ---------------------------------*/

/*--------------------  F u n c t i o n   P r o t o t y p e s  -------------*/
int SensorNoise(void);

int SetOutputRefValue( int val );
int SetSensorGain(int val);
int SetSensorOffset(int val);
int ReadSensor(void);

#ifdef __cplusplus
}
#endif

#endif


