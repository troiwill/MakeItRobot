#ifndef MAKE_IT_MOBILE_ROBOT_H
#define MAKE_IT_MOBILE_ROBOT_H

// forward declaration
#include "MakeItMotorShield.h"

class MakeItMobileRobot {
public:
    enum MovingState_t {
        STOPPED = 1 << 0,
        MOVING = 1 << 1,
        TURNING = 1 << 2
    };

    // constructor
    MakeItMobileRobot();

    // deconstructor
    ~MakeItMobileRobot();

    /**
     * Moves the robot forward or backward by specifying
     * the number of millimeters.
     * If millimeters is negative (-), the robot moves backward.
     * If millimeters is positive (+), the robot moves forward.
    **/
    void move(float distMM = 0.0f);

    /**
     * Turns the robot left or right by specifying the angle
     * amount in degrees.
     * If radians is negative (-), the robot turns left.
     * If radians is positive (+), the robot moves right.
    **/
    void turn(float angleRAD = 0.0f);

    /**
     * Halts all the motors (the robot stops moving/turning).
    **/
    void stop();

    /**
     * States whether or not the robot is in motion.
    **/
    bool isInMotion() const;

    /**
     * Returns the current state of the robot.
    **/
    MovingState_t getState() const;

    //
    void checkTime();

    /**
     * Converts a "degree" argument to radians.
    **/
    static float degsToRads(float degrees);

private:
    struct LastRequest {
        MovingState_t currState;
        float disOrAngle;
    };

    MakeItMotorShield shield;
    MotorShieldCommand cmd;
    LastRequest lastReq;
    unsigned long stopTime;

    static const unsigned int kLeftMotorPort;
    static const unsigned int kRightMotorPort;
    static const unsigned int kMotorSpeed;
    static const unsigned int kMillimetersPerSec;
    static const float kPi;
    static const float kTwoRad;
};

#endif // end of MAKE_IT_MOBILE_ROBOT_H