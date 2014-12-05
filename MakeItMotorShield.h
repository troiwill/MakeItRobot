#ifndef MAKE_IT_MOTOR_SHIELD_H
#define MAKE_IT_MOTOR_SHIELD_H

#include "Arduino.h"
//#include <map>

#define DC_CMD_IR_TX1    0x70
#define DC_CMD_IR_TX2    0x71
#define DC_CMD_IR_TX3    0x72
#define DC_CMD_IR_TX4    0x73

#define SW_ON            0xff
#define DC_SEND_HEADER   0x56
#define DC_RECV_HEADER   0x76

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
    int speeds[4];
};

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
    unsigned int activePorts[4];
    unsigned int numActivePorts;

    // 
    static const int kMotorDir1;
    static const int kMotorDir2;
    static const int kMotorDir3;
    static const int kMotorDir4;

    // 
    static const int kCW;
    static const int kCCW;

    static const int kMotorPwm1;
    static const int kMotorPwm2;
    static const int kMotorPwm3;
    static const int kMotorPwm4;

    static int instanceCount;
};

#endif // end of MAKE_IT_MOTOR_SHIELD_H