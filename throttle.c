/*
 * throttle.c
 *
 *  Created on: Aug 17, 2019
 *      Author: root
 */


#ifndef THROTTLE_C_
#define THROTTLE_C_

// This function measures the PWM signal on the pin defined by CHANNEL_1_PIN
void readThrottle()
{
    digitalWrite(12, HIGH);

    //record the interrupt time so that we can tell if the receiver has a signal from the transmitter
    last_interrupt_time = micros();
    //Serial.print(last_interrupt_time);
    //if the pin has gone HIGH, record the microseconds since the Arduino started up
    if(digitalRead(CHANNEL_1_PIN) == HIGH)
    {
        timer_start = micros();
    }
    //otherwise, the pin has gone LOW
    else
    {
        //only worry about this if the timer has actually started
        if(timer_start != 0)
        {
            //record the pulse time
            pulse_time = ((volatile int)micros() - timer_start);
            //restart the timer
            timer_start = 0;
        }
    }
}

void triggerRange(int * thr, int * prev_thr)
{
  // sometimes remote gives us a 0, in which case
  // use the previous value
  if ((*thr)==0) *thr=*prev_thr;

  // else if it's a new value, make sure it falls
  // within the range. Also, update previous value
  else{
    if (*thr<TRIGGER_MS_MIN) *thr = TRIGGER_MS_MIN;
    if (*thr>TRIGGER_MS_MAX) *thr = TRIGGER_MS_MAX;
    *prev_thr=*thr;
  }
}

void wheelRange(int * thr, int * prev_thr)
{
  // sometimes remote gives us a 0, in which case
  // use the previous value
  if ((*thr)==0) *thr=*prev_thr;

  // else if it's a new value, make sure it falls
  // within the range. Also, update previous value
  else{
    if (*thr<WHEEL_MS_MIN) *thr = WHEEL_MS_MIN;
    if (*thr>WHEEL_MS_MAX) *thr = WHEEL_MS_MAX;
    *prev_thr=*thr;
  }
}

int governMotorRange(int thr)
{

  if (thr<MOTOR_MS_MIN) return MOTOR_MS_MIN;
  else if (thr>MOTOR_MS_MAX) return MOTOR_MS_MAX;
  else return thr;

}

void printThrottle(int throttle, int wheel)
{
  //Serial.print(throttle);
  //Serial.print(" ");
  //Serial.println(wheel);

  XBee.print("trigger: ");
  XBee.print(throttle);
  XBee.print("\t wheel: ");
  XBee.print(wheel);
  XBee.print("\n");

}

void printMotorSpeed(void)
{
	  XBee.print("front: ");
	  XBee.print(front_motor_ms);
	  XBee.print("\t rear: ");
	  XBee.print(rear_motor_ms);
	  XBee.print("\n");
}

void readSetThrottle(int * p_trigger, int * p_prev_trigger, int * p_wheel, int * p_prev_wheel)
{
	int thrust,delta_thrust;

	*p_trigger = pulseIn(4, HIGH, 25000); // Read the pulse width of
	*p_wheel = pulseIn(5, HIGH, 25000); // Read the pulse width of
	triggerRange(p_trigger,p_prev_trigger);
	wheelRange(p_wheel,p_prev_wheel);

	thrust = map(*p_trigger,TRIGGER_MS_MID,TRIGGER_MS_MAX,THROTTLE_MS_MIN,THROTTLE_MS_MAX);
	if (*p_wheel>=WHEEL_MS_MID) delta_thrust = map(*p_wheel,WHEEL_MS_MID,WHEEL_MS_MAX, D_THROTTLE_MS_MIN, D_THROTTLE_MS_MAX);
	else delta_thrust = -1*map(*p_wheel,WHEEL_MS_MID,WHEEL_MS_MIN, D_THROTTLE_MS_MIN, D_THROTTLE_MS_MAX);

	front_motor_ms=governMotorRange(thrust+delta_thrust);
	rear_motor_ms=governMotorRange(thrust-delta_thrust);

//	MotorFront.writeMicroseconds(front_motor_ms);
//	MotorRear.writeMicroseconds(rear_motor_ms);
	MotorFront.writeMicroseconds(front_motor_ms);
	MotorRear.writeMicroseconds(rear_motor_ms);

//	XBee.print("Trigger: ");
//	XBee.print(*p_trigger);
//	XBee.print("\tWheel: ");
//	XBee.print(*p_wheel);
//	XBee.print("\tThrust: ");
//	XBee.print(thrust);
//	XBee.print("\tDelta: ");
//	XBee.print(delta_thrust);
//	XBee.print("\tFront: ");
//	XBee.print(front_motor_ms);
//	XBee.print("\tRear: ");
//	XBee.print(rear_motor_ms);
//	XBee.print("\n");
}

#endif /* THROTTLE_C_ */
