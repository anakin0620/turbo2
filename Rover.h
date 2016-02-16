/**
 * rover
 */

#ifndef __ROVER_OBJECT_H
#define __ROVER_OBJECT_H


#include "MotorController.h"
#include "UsbManager.h"
#include "v4l2Camera.h"
#include "evdevController.h"
#include "rpLIDAR.h"
#include "rovernet.h"


/**
 * Rover primary object.
 */
class Rover
{
public:
	/**
	 * Create
	 */
	static Rover* Create();

	/**
	 * destructor
	 */
	~Rover();

	/**
 	 * Run the next frame / main loop.
	 */
	bool NextEpoch();


protected:

	Rover();

	bool init();
	bool initMotors();
	bool initBtController();

	static const uint32_t NumMotorCon = 2;	/**< number of motor controllers */

	MotorController* mMotorCon[NumMotorCon];
	UsbManager*      mUsbManager;	
	v4l2Camera*	  mCamera;
	evdevController* mBtController;		/**< Bluetooth /dev/event controller */
	rpLIDAR*		  mLidar;
	roverNet*		  mRoverNet;
};



#endif