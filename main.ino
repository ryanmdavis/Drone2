//Note that now it takes 10ms to send just 9 characters, need to increase baud rate!

//Pins:
#define MOTOR_FRONT_PIN     9
#define MOTOR_REAR_PIN      10
#define SERVO_PIN           11

//Throttle definitions
#define CHANNEL_1_PIN       3
#define TRIGGER_MS_MIN      970
#define TRIGGER_MS_MID      1460
#define TRIGGER_MS_MAX      1860
#define WHEEL_MS_MIN        730
#define WHEEL_MS_MID        1570
#define WHEEL_MS_MAX        1860
#define THROTTLE_MS_MIN     1000
#define THROTTLE_MS_MAX     1300
#define D_THROTTLE_MS_MIN   0
#define D_THROTTLE_MS_MAX   100
#define MOTOR_MS_MIN        1000
#define MOTOR_MS_MAX        1400

// Servo Angle
#define SA_MIN          670
#define SA_MAX          1420
#define SA_90           670
#define SA_0            2200

// XBee comm
#define MAX_CMD_LEN     8
#define XBEE_BAUD       115200

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
//#include "TimerOne.h"
#include <SoftwareSerial.h> //For communicating with XBee
#include <Servo.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
//micros when the pin goes HIGH
volatile unsigned long timer_start;
volatile int last_interrupt_time; //readThrottle is the interrupt handler
volatile int pulse_time;

int ch1;

//For Atmega328P's
// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX

//variables to hold euler angles, angle changes, and angle accelerations 
float roll, pitch, yaw;
float d_roll, d_pitch, d_yaw;
float dd_roll, dd_pitch, dd_yaw;

// throttle
int throttle_val,prev_throttle_val,wheel_val,prev_wheel_val;

// motor PWM vals
int front_motor_ms,rear_motor_ms;

// PWMs
Servo MotorServo;
Servo MotorFront;
Servo MotorRear;

// Include commands

// xbee input
String xbee_input_str = String("");
String command = String("");

// timer
int t_last_read,t0,t1;

// prototypes
#include "/home/ryan/Dropbox/Code/Eclipse Workspaces/TowDrone_C/prototypes.h"

// miscilations operations
#include "/home/ryan/Dropbox/Code/Eclipse Workspaces/TowDrone_C/misc.c"

// file for giving text commands via XBee and XCTU
#include "/home/ryan/Dropbox/Code/Eclipse Workspaces/TowDrone_C/commands.c"

// functions for measuring the physical state of the system
#include "/home/ryan/Dropbox/Code/Eclipse Workspaces/TowDrone_C/state.c"

// functions for measuring the throttle value provided by RC
#include "/home/ryan/Dropbox/Code/Eclipse Workspaces/TowDrone_C/throttle.c"

void setup() {

  //pinMode(CHANNEL_1_PIN, INPUT); // For measuring PWM length on pin CHANNEL_1_PIN
  pinMode(12, OUTPUT);
  timer_start = 0;
  //attachInterrupt(digitalPinToInterrupt(CHANNEL_1_PIN), readThrottle, CHANGE); // For measuring PWM length on pin CHANNEL_1_PIN

  // open up com channels
  XBee.begin(XBEE_BAUD);
  Serial.begin(9600); // Pour a bowl of Serial

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("No BNO055 detected ... Check your wiring or I2C ADDR!");
    //while(1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);

  // set pin 5 as input to read in PWM from throttle
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  // Set up PWM outputs (motor servo and two motors)
  //pinMode(SERVO_PIN, OUTPUT);
  //  pinMode(MOTOR_P_PIN,OUTPUT);
  //  pinMode(MOTOR_M_PIN,OUTPUT);
  //  pinMode(SERVO_PIN,OUTPUT);
  MotorRear.attach(MOTOR_FRONT_PIN);
  MotorFront.attach(MOTOR_REAR_PIN);
  MotorServo.attach(SERVO_PIN);
  MotorServo.writeMicroseconds(SA_0);
  MotorFront.writeMicroseconds(MOTOR_MS_MIN);
  MotorRear.writeMicroseconds(MOTOR_MS_MIN);

  // put an initial value in the timer
  t_last_read=micros();
}

void loop() {
 
  //digitalWrite(12, LOW);
  delay(100);
  //digitalWrite(12, LOW);

  /* Display the floating point data */
  getState(&roll,&pitch,&yaw,&d_roll,&d_pitch,&d_yaw,&dd_roll,&dd_pitch,&dd_yaw,&t_last_read);
  //printPositionXBee(&roll,&pitch,&yaw,&d_roll,&d_pitch,&d_yaw,&dd_roll,&dd_pitch,&dd_yaw,&t_last_read);

  // Does the work of reading wheel andtrigger pos from receicer, converting to 
  // thrust and felta thrust (b/w motors), and governing motor speeds
  readSetThrottle(&throttle_val, &prev_throttle_val, &wheel_val, &prev_wheel_val);

  // Check XBee buffer
  while (XBee.available())
  {
    xbee_input_str=String(xbee_input_str+char(XBee.read()));
  }

  // if we lost track of command and it's gotten too long
  if (xbee_input_str.length()>MAX_CMD_LEN)
  {
    xbee_input_str=String("");
  }

  if (xbee_input_str.length()>0) 
  {
    //XBee.print("test");
    //XBee.write(xbee_input_str.charAt(xbee_input_str.length()-1));
    if (xbee_input_str.charAt(xbee_input_str.length()-1)=='\r')
      {
        //t0=micros();

        excCmd();
        xbee_input_str=String("");
        //t1=micros();
        //XBee.write(String("Time to exc write: " + String(t1-t0) + String("us\n")).c_str());

      }
    
  }

  //XBee.write(xbee_input_str.c_str());
  //XBee.write(String(xbee_input_str.length()).c_str());
  //XBee.write(xbee_input_str.charAt(xbee_input_str.length()-1));
  } 
