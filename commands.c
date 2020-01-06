/*
 * commands.c
 *
 *  Created on: Aug 10, 2019
 *      Author: root
 */

// functions for measuring the throttle value provided by RC
#include "/home/ryan/Dropbox/Code/Eclipse Workspaces/TowDrone_C/throttle.c"

void sa(String val) // took 864 us to execute command "sa1500/n"
{
  int servo_angle = val.toInt();
  if (servo_angle) //if it's zero, then there was an error converting to int
  {
    governInt(&servo_angle, SA_90, SA_0);
    //XBee.print(String("SA set to: ") + String(servo_angle)+String("\n"));
    MotorServo.writeMicroseconds(servo_angle);
  }
  else XBee.print("Err conv SA to int\n");

  //t0=micros();
  //XBee.write("test test");
  //t1=micros();
  //XBee.write(String("Time to exc write: " + String(t1-t0) + String("us\n")).c_str());
}

void parseCmd(String * p_cmd, String * p_val)
{
  int char_num=2;

  *p_cmd=String(xbee_input_str.charAt(0))+String(xbee_input_str.charAt(1));

  while (xbee_input_str.charAt(char_num) != '\r')
  {
    *p_val = *p_val + String(xbee_input_str.charAt(char_num));
    char_num=char_num+1;
  }
  //XBee.print(String("command executed! \ncmd: "+*p_cmd+"\nval: "+*p_val+"\n").c_str());

}

void interpretCmd(String cmd, String val)
{
  if (!(cmd.compareTo("sa"))) sa(val);
  else if (!(cmd.compareTo("xs"))) printPositionXBee(&roll, &pitch, &yaw, &d_roll, &d_pitch, &d_yaw, &dd_roll, &dd_pitch, &dd_yaw, &t_last_read);
  else if (!(cmd.compareTo("xt"))) printThrottle(throttle_val, wheel_val);
  else if (!(cmd.compareTo("xm"))) printMotorSpeed();

}

void excCmd(void)
{
  String cmd=String("");
  String val=String("");


  parseCmd(&cmd, &val);
  interpretCmd(cmd, val);
  //XBee.print(String("command executed! \ncmd: "+cmd+"\nval: "+val+"\n").c_str());
}
