#ifndef MAKE_IT_MOTOR_SHIELD_H
#define MAKE_IT_MOTOR_SHIELD_H

#include "Arduino.h"

#define DC_CMD_IR_TX1    0x70
#define DC_CMD_IR_TX2    0x71
#define DC_CMD_IR_TX3    0x72
#define DC_CMD_IR_TX4    0x73

#define SW_ON            0xff
#define DC_SEND_HEADER   0x56
#define DC_RECV_HEADER   0x76

/**
 * The MotorShieldCommand class stores a revolution per
 * minute request for each motor shield port.
**/
class MotorShieldCommand {
public:
    // constructor
    MotorShieldCommand();

    // deconstructor
    ~MotorShieldCommand();

    // adds a motor
    void addCommand(unsigned int shieldPort, int rpm);

    friend class MakeItMotorShield;
private:
    /**
     * Stores a list of rpms. An rpm "r" for a particular
     * shield port "sp" is stored as "speeds[sp - 1] = r"
    **/
    int speeds[4];
};

/**
 * The MakeItMotorShield class provides an interface for
 * operating the motor shield using an Arduino Uno v3
**/
class MakeItMotorShield {
public:
    // constructor
    MakeItMotorShield();

    // deconstructor
    ~MakeItMotorShield();

    // activate a particular port on the motor shield
    void activatePort(unsigned int shieldPort);

    // runs motor command
    void runCommand(const MotorShieldCommand& kCmd);

    // halts all motors attached to this motor shield
    void stopAllMotors();

private:
    // sends the motor commands
    void dcWrite(int type, int value);

public:
    // max number of motor ports on this shield
    static const unsigned int kMaxNumMotors;

private:
    // stores the list of active shield ports
    unsigned int activePorts[4];

    // the number of active shield ports
    unsigned int numActivePorts;

    /**
     * Hexidecimal header values that are used in
     * conjunction with a hexidecimal direction value
     * for specifying the direction of a motor on a
     * particular shield port.
    **/
    static const int kMotorDir1;
    static const int kMotorDir2;
    static const int kMotorDir3;
    static const int kMotorDir4;

    /**
     * Hexidecimal values for specifying the
     * motor direction.
    **/
    static const int kCW;
    static const int kCCW;

    /**
     * Hexidecimal header values that are used 
     * in conjunction with a rpm value to specify
     * motor speed.
    **/
    static const int kMotorPwm1;
    static const int kMotorPwm2;
    static const int kMotorPwm3;
    static const int kMotorPwm4;

    // a count of the number instances of this class
    static int instanceCount;
};

#endif // end of MAKE_IT_MOTOR_SHIELD_H