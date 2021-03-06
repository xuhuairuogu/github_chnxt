/* File name: ultrasonicsensor.ch
 *
 * Demonstrate the Ch Mindstorms Control Package's ability 
 * to control the NXT Mindstorm to use the ultrasonic sensor.*/
  
#include <nxt.h>

ChNXT nxt;
int status;
int ultraValue;
double speedRatio;

/* Connect to NXT */
if (nxt.connect()) {
    printf("Error: Cannot connect to Lego Mindstorm NXT.\n");
    exit(-1);
}

/* Set sensor types */
status = nxt.setSensor(NXT_SENSORPORT4, 
        NXT_SENSORTYPE_ULTRASONIC, NXT_SENSORMODE_RAWMODE);
if (status) {
    printf("Fail to setup sensors\n");
    exit(-1);
}

/* Commands: */
while (1) {
    /* get ultrasonic sensor data */
    nxt.getSensor(NXT_SENSORPORT4, ultraValue);

    /* If obstacle is really close */
    if (ultraValue < 20 && ultraValue >0) {
        speedRatio = 0.25;
        /* Move backward */
        nxt.moveJointContinuousNB(NXT_JOINT2, NXT_BACKWARD);
        nxt.moveJointContinuousNB(NXT_JOINT3, NXT_BACKWARD);
        sleep(3);
        /* Quit the while loop */
        break;
    }/* Else if the obstacle is close */
    else if (ultraValue < 40) {
        speedRatio = .5;
    }/* Else if the obstacle is not close */
    else if (ultraValue < 80) {
        speedRatio = .75;
    }/* Else if there is no obstacle in sight */
    else {
        speedRatio = 1.0;
    }
    /* Move forward (constantly)*/
    nxt.setJointSpeedRatios(0, speedRatio, speedRatio);
    nxt.moveJointContinuousNB(NXT_JOINT2, NXT_FORWARD);
    nxt.moveJointContinuousNB(NXT_JOINT3, NXT_FORWARD);
}

/* Stop the motors */
nxt.stopTwoJoints(NXT_JOINT2, NXT_JOINT3);
