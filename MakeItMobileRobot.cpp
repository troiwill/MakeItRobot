#include "MakeItMobileRobot.h"

const unsigned int MakeItMobileRobot::kLeftMotorPort = 1;
const unsigned int MakeItMobileRobot::kRightMotorPort = 3;
const unsigned int MakeItMobileRobot::kMotorSpeed = 140;
const unsigned int MakeItMobileRobot::kMillimetersPerSec = 260;
const float MakeItMobileRobot::kPi = 3.14159f;
const float MakeItMobileRobot::kTwoRad = 2.0f * kPi;

MakeItMobileRobot::MakeItMobileRobot()
  : shield(),
    cmd(),
    lastReq(),
    stopTime(0ul)
{
    shield.activatePort(kLeftMotorPort);
    shield.activatePort(kRightMotorPort);
    lastReq.currState = STOPPED;
    lastReq.disOrAngle = 0.0f;
}

MakeItMobileRobot::~MakeItMobileRobot() {
    // no explicit implementation
}

void MakeItMobileRobot::move(float distMM) {
    // if the distance is zero, do nothing.
    if (distMM == 0.0f) {
        return;
    }

    if (lastReq.currState != MOVING || lastReq.disOrAngle != distMM) {
        // stop the motors if they are rotating
        if (lastReq.currState != STOPPED) {
            stop();
        }
        // move backwards if distance is less than zero
        if (distMM < 0) {
            cmd.addCommand(kLeftMotorPort, -0.9 * kMotorSpeed);
            cmd.addCommand(kRightMotorPort, -1 * kMotorSpeed);
        }
        // else move forwards
        else {
            cmd.addCommand(kLeftMotorPort, 0.9 * kMotorSpeed);
            cmd.addCommand(kRightMotorPort, kMotorSpeed);
        }
        shield.runCommand(cmd);
        lastReq.currState = MOVING;
        lastReq.disOrAngle = distMM;
    }
    distMM = abs(distMM);
    unsigned long motionRunTimeMS = (unsigned long)((distMM / kMillimetersPerSec) * 1000ul);
    stopTime = millis() + motionRunTimeMS;
}

void MakeItMobileRobot::turn(float angleRAD) {
    // if the radians variable is zero, do nothing.
    if (angleRAD == 0.0f) {
        return;
    }

    if (lastReq.currState != TURNING || lastReq.disOrAngle != angleRAD) {
        // stop the motors if they are rotating
        if (lastReq.currState != STOPPED) {
            stop();
        }
        // turn left if radians is less than zero
        if (angleRAD < 0) {
            cmd.addCommand(kLeftMotorPort, -1 * 0.9 * kMotorSpeed);
            cmd.addCommand(kRightMotorPort, kMotorSpeed);
        }
        // else turn right
        else {
            cmd.addCommand(kLeftMotorPort, 0.9 * kMotorSpeed);
            cmd.addCommand(kRightMotorPort, -1 * kMotorSpeed);
        }
        shield.runCommand(cmd);
        lastReq.currState = TURNING;
        lastReq.disOrAngle = angleRAD;
    }
    angleRAD = abs(angleRAD);
    unsigned long motionRunTimeMS = (unsigned long)((angleRAD / kTwoRad) * 1300ul);
    stopTime = millis() + motionRunTimeMS;
}

void MakeItMobileRobot::stop() {
    if (lastReq.currState != STOPPED) {
        cmd.addCommand(kLeftMotorPort, 0);
        cmd.addCommand(kRightMotorPort, 0);
        shield.runCommand(cmd);
        lastReq.currState = STOPPED;
        lastReq.disOrAngle = 0.0f;
    }
}

bool MakeItMobileRobot::isInMotion() const {
    return (lastReq.currState != STOPPED);
}

MakeItMobileRobot::MovingState_t MakeItMobileRobot::getState() const {
    return lastReq.currState;
}

void MakeItMobileRobot::checkTime() {
    if (millis() >= stopTime && lastReq.currState != STOPPED) {
        stop();
    }
}

float MakeItMobileRobot::degsToRads(float degrees) {
    return (degrees * kPi / 180.0f);
}