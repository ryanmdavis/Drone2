/*
 * state.c
 *
 *  Created on: Aug 17, 2019
 *      Author: root
 */


void getState(float * p_roll,float * p_pitch,float * p_yaw,float * p_d_roll,float * p_d_pitch,float * p_d_yaw,float * p_dd_roll,float * p_dd_pitch,float * p_dd_yaw,int * p_t)
{
  double prev_d_roll, prev_d_pitch, prev_d_yaw;
  int new_time,elapsed_time;

  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  // get euler angles
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  *p_roll=euler.z();
  *p_pitch=-euler.y();
  *p_yaw=euler.x();

  // Save angular velocities from last interrupt call
  prev_d_roll=*p_d_roll;
  prev_d_pitch=*p_d_pitch;
  prev_d_yaw=*p_d_yaw;

  // get angluar velocities
  imu::Vector<3> vel = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  *p_d_roll=-vel.x();
  *p_d_pitch=vel.y();
  *p_d_yaw=-vel.z();

  // get time since last velocity measurement
  new_time=micros();
  elapsed_time=*p_t-new_time;
  *p_t=new_time;

  // get angular acceleration
  *p_dd_roll=(prev_d_roll-*p_d_roll)/elapsed_time;
  *p_dd_pitch=(prev_d_pitch-*p_d_pitch)/elapsed_time;
  *p_dd_yaw=(prev_d_yaw-*p_d_yaw)/elapsed_time;
}

void printPositionSerial(float * p_roll,float * p_pitch,float * p_yaw,float * p_d_roll,float * p_d_pitch,float * p_d_yaw,float * p_dd_roll,float * p_dd_pitch,float * p_dd_yaw,int * p_t)
{
  Serial.print("R: ");
  Serial.print(*p_roll, 4);
  Serial.print("\tP: ");
  Serial.print(*p_pitch, 4);
  Serial.print("\tY: ");
  Serial.print(*p_yaw, 4);

  Serial.print("\tdR: ");
  Serial.print(*p_d_roll, 4);
  Serial.print("\tdP: ");
  Serial.print(*p_d_pitch, 4);
  Serial.print("\tdY: ");
  Serial.print(*p_d_yaw, 4);

  Serial.print("\tddR: ");
  Serial.print(*p_dd_roll, 4);
  Serial.print("\tddP: ");
  Serial.print(*p_dd_pitch, 4);
  Serial.print("\tddY: ");
  Serial.print(*p_dd_yaw, 4);
  Serial.println("");
}

void printPositionXBee(float * p_roll,float * p_pitch,float * p_yaw,float * p_d_roll,float * p_d_pitch,float * p_d_yaw,float * p_dd_roll,float * p_dd_pitch,float * p_dd_yaw,int * p_t)
{
  XBee.print(*p_roll,4);
  XBee.print(",");
  XBee.print(*p_pitch,4);
  XBee.print(",");
  XBee.print(*p_yaw,4);
  XBee.print(",");

  XBee.print(*p_d_roll,4);
  XBee.print(",");
  XBee.print(*p_d_pitch,4);
  XBee.print(",");
  XBee.print(*p_d_yaw,4);
  XBee.print(",");

  XBee.print(*p_dd_roll,4);
  XBee.print(",");
  XBee.print(*p_dd_pitch,4);
  XBee.print(",");
  XBee.print(*p_dd_yaw,4);
  XBee.print("\n");
}
