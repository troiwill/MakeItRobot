#include "MakeItMotorShield.h"

const unsigned int MakeItMotorShield::kMaxNumMotors = 4;

const int MakeItMotorShield::kMotorDir1 = 0x73;
const int MakeItMotorShield::kMotorDir2 = 0x74;
const int MakeItMotorShield::kMotorDir3 = 0x75;
const int MakeItMotorShield::kMotorDir4 = 0x76;

const int MakeItMotorShield::kCW = 0xff;
const int MakeItMotorShield::kCCW = 0x00;

const int MakeItMotorShield::kMotorPwm1 = 0x80;
const int MakeItMotorShield::kMotorPwm2 = 0x81;
const int MakeItMotorShield::kMotorPwm3 = 0x82;
const int MakeItMotorShield::kMotorPwm4 = 0x83;

int MakeItMotorShield::instanceCount = 0;

MotorShieldCommand::MotorShieldCommand()
  : speeds()
{ }

MotorShieldCommand::~MotorShieldCommand() {
    // no explicit implementation
}

void MotorShieldCommand::addCommand(unsigned int shieldPort, int rpm) {
    speeds[shieldPort - 1] = rpm;
}


MakeItMotorShield::MakeItMotorShield()
  : numActivePorts(0)
{
    if (instanceCount++ == 0) {
        // setup communication with the PCB
        Serial.begin(10420);
        delay(250);
    }
}

MakeItMotorShield::~MakeItMotorShield() {
    if (--instanceCount == 0) { // <--- I do not know if this is necessary
        // stops all motors
        stopAllMotors();
    }
}

void MakeItMotorShield::activatePort(unsigned int shieldPort) {
    // check if the specified port is within the range of ports
    // on the motor shield
    if (0 < shieldPort && shieldPort <= kMaxNumMotors) {
        for (unsigned int i = 0; i < numActivePorts; i++) {
            // the shield port was previously added
            if (activePorts[i] == shieldPort)
                return;
        }
        // add the "shieldPort" to the list of active ports
        activePorts[numActivePorts] = shieldPort;

        // activate the port
        switch(activePorts[numActivePorts]) {
            case 1:
                dcWrite(DC_CMD_IR_TX1, SW_ON);
                break;

            case 2:
                dcWrite(DC_CMD_IR_TX2, SW_ON);
                break;

            case 3:
                dcWrite(DC_CMD_IR_TX3, SW_ON);
                break;

            case 4:
                dcWrite(DC_CMD_IR_TX4, SW_ON);
                break;

            default:
                break;
        }
        // increase the number of active ports
        numActivePorts++;
        delay(200);
    }
}

void MakeItMotorShield::runCommand(const MotorShieldCommand& kCmd) {
    int motorDirCmd;
    int motorPwmCmd[kMaxNumMotors];
    int motorPwn[kMaxNumMotors];

    // iterate over the active shield ports to determine
    // motor direction and speed from the "command" argument
    for (unsigned int i = 0; i < numActivePorts; i++) {
        unsigned int shieldPort = activePorts[i];

        switch(shieldPort) {
            case 1:
                motorDirCmd = kMotorDir1;
                motorPwmCmd[i] = kMotorPwm1;
                break;

            case 2:
                motorDirCmd = kMotorDir2;
                motorPwmCmd[i] = kMotorPwm2;
                break;

            case 3:
                motorDirCmd = kMotorDir3;
                motorPwmCmd[i] = kMotorPwm3;
                break;

            case 4:
                motorDirCmd = kMotorDir4;
                motorPwmCmd[i] = kMotorPwm4;
                break;

            default:
                return;
        }

        // set motor direction and determine the
        // motor's speed (rpm)
        int rpm = kCmd.speeds[shieldPort - 1];
        if (rpm < 0) {
            dcWrite(motorDirCmd, kCCW);
            rpm *= -1;
        } else if (rpm > 0) {
            dcWrite(motorDirCmd, kCW);
        }
        motorPwn[i] = rpm;
    }

    // set the motor directions and speeds
    for (unsigned int i = 0; i < numActivePorts; i++) {
        dcWrite(motorPwmCmd[i], motorPwn[i]);
    }
}

void MakeItMotorShield::stopAllMotors() {
    // create a command to stop all the motors
    MotorShieldCommand cmd;
    for (unsigned int i = 0; i < numActivePorts; i++) {
        cmd.addCommand(activePorts[i], 0);
    }
    runCommand(cmd);
}

void MakeItMotorShield::dcWrite(int type, int value) {
    Serial.write(DC_SEND_HEADER);
    Serial.write(type);
    Serial.write(value);
    delay(40);
}