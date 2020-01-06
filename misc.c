/*
 * misc.c
 *
 *  Created on: Aug 17, 2019
 *      Author: root
 */


void governInt(int * val, int val_min, int val_max)
{
  if (*val>val_max) *val = val_max;
  else if (*val<val_min) *val = val_min;
}
