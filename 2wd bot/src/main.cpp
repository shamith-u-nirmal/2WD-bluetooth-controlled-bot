#include <Arduino.h>
#include <SoftwareSerial.h>

//==================================================
// Bluetooth
//==================================================
SoftwareSerial BT(4, 7); // Arduino RX, TX

//==================================================
// L298N Motor Pins
//==================================================
const int ENA = 5;
const int ENB = 6;

const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

//==================================================
// Encoder Pins
//==================================================
const int LEFT_ENCODER = 2;
const int RIGHT_ENCODER = 3;

volatile long leftTicks = 0;
volatile long rightTicks = 0;

//==================================================
// Encoder Calibration
//==================================================
const float WHEEL_CIRCUMFERENCE_CM = 19.6; // pi * ~6.25cm diameter
const int TICKS_PER_REV = 100;             // calibrated value, re-check per wheel

long lastLeftTicks = 0;
long lastRightTicks = 0;

//==================================================
// Speed
//==================================================
int speedValue = 120;

//==================================================
// Encoder Interrupts
//==================================================
void leftEncoderISR()
{
    leftTicks++;
}

void rightEncoderISR()
{
    rightTicks++;
}

//==================================================
// Motor Functions
//==================================================
void setSpeed(int speed)
{
    speedValue = constrain(speed, 0, 255);
    analogWrite(ENA, speedValue);
    analogWrite(ENB, speedValue);
}

void stopMotors()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void moveForward()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void moveBackward()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnLeft()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void turnRight()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

//==================================================
// Setup
//==================================================
void setup()
{
    Serial.begin(9600);
    BT.begin(9600);

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(LEFT_ENCODER, INPUT_PULLUP);
    pinMode(RIGHT_ENCODER, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER), leftEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER), rightEncoderISR, RISING);

    setSpeed(speedValue);
    stopMotors();

    Serial.println("Robot Ready!");
}

//==================================================
// Loop
//==================================================
void loop()
{
    if (BT.available())
    {
        char cmd = BT.read();

        switch (cmd)
        {
        case 'F':
            setSpeed(speedValue);
            moveForward();
            break;

        case 'B':
            setSpeed(speedValue);
            moveBackward();
            break;

        case 'L':
            setSpeed(speedValue);
            turnLeft();
            break;

        case 'R':
            setSpeed(speedValue);
            turnRight();
            break;

        case 'G':
            analogWrite(ENA, speedValue / 2);
            analogWrite(ENB, speedValue);
            moveForward();
            break;

        case 'H':
            analogWrite(ENA, speedValue);
            analogWrite(ENB, speedValue / 2);
            moveForward();
            break;

        case 'I':
            analogWrite(ENA, speedValue / 2);
            analogWrite(ENB, speedValue);
            moveBackward();
            break;

        case 'J':
            analogWrite(ENA, speedValue);
            analogWrite(ENB, speedValue / 2);
            moveBackward();
            break;

        case 'S':
            stopMotors();
            break;

        case '0': setSpeed(0); break;
        case '1': setSpeed(30); break;
        case '2': setSpeed(60); break;
        case '3': setSpeed(90); break;
        case '4': setSpeed(120); break;
        case '5': setSpeed(150); break;
        case '6': setSpeed(180); break;
        case '7': setSpeed(210); break;
        case '8': setSpeed(235); break;
        case '9': setSpeed(255); break;
        }
    }

    // Print encoder counts + RPM + distance every 500 ms
    static unsigned long lastPrint = 0;

    if (millis() - lastPrint >= 500)
    {
        unsigned long dt = millis() - lastPrint; // actual elapsed, not assumed 500
        lastPrint = millis();

        noInterrupts();
        long left = leftTicks;
        long right = rightTicks;
        interrupts();

        long leftDelta = left - lastLeftTicks;
        long rightDelta = right - lastRightTicks;
        lastLeftTicks = left;
        lastRightTicks = right;

        float leftRPM  = (leftDelta  / (float)TICKS_PER_REV) * (60000.0 / dt);
        float rightRPM = (rightDelta / (float)TICKS_PER_REV) * (60000.0 / dt);

        float leftDistCm  = (leftDelta  / (float)TICKS_PER_REV) * WHEEL_CIRCUMFERENCE_CM;
        float rightDistCm = (rightDelta / (float)TICKS_PER_REV) * WHEEL_CIRCUMFERENCE_CM;

        Serial.print("L ticks:"); Serial.print(left);
        Serial.print(" R ticks:"); Serial.print(right);
        Serial.print(" | L RPM:"); Serial.print(leftRPM, 1);
        Serial.print(" R RPM:"); Serial.print(rightRPM, 1);
        Serial.print(" | L cm/interval:"); Serial.print(leftDistCm, 2);
        Serial.print(" R cm/interval:"); Serial.println(rightDistCm, 2);
    }
}