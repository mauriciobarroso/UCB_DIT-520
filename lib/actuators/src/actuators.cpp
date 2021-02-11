/*
 * mcp401x.c
 *
 * Created on: Jan 13, 2021
 * Author: Mauricio Barroso Benavides
 */

/* inclusions ----------------------------------------------------------------*/

#include "actuators.h"

/* macros --------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/

/* internal data declaration -------------------------------------------------*/

/* external data declaration -------------------------------------------------*/

/* internal functions declaration --------------------------------------------*/

/* external functions definition ---------------------------------------------*/


bool actuators_init(actuators_t * const me, actuators_pins_t * pins)
{
  /* Initialise servomotor*/
  me->pins.servo = pins->servo;
  me->servo.attach(me->pins.servo);
  me->servo.write(SERVO_DEFAULT_MIN_ANGLE);

  /* Initialise DC motor*/
  me->pins.motor = pins->motor;
  pinMode(me->pins.motor, OUTPUT);
  analogWrite(me->pins.motor, 0);

  /* Initialise relay*/
  me->pins.relay = pins->relay;
  pinMode(me->pins.relay, OUTPUT);
  digitalWrite(me->pins.relay, LOW);

  return true;
}

void actuators_set_values(actuators_t * const me, actuators_values_t * values)
{
  /* Set servomotor value */
  if(values->servo != me->values.servo)
  {
    me->values.servo = values->servo;

    if(me->values.servo)
      me->servo.write(SERVO_DEFAULT_MAX_ANGLE);
    else
      me->servo.write(SERVO_DEFAULT_MIN_ANGLE);
  }

  /* Set DC motor value */
  if(values->motor >= 0 && values->motor <= 255 && values->motor != me->values.motor)
  {
    me->values.motor = values->motor;
    analogWrite(me->pins.motor, me->values.motor);
  }

  /* Set relay value */
  if(values->relay != me->values.relay)
  {
    me->values.relay =  values->relay;
    digitalWrite(me->pins.relay, me->values.relay);
  }
}

/* internal functions definition ---------------------------------------------*/

/* end of file ---------------------------------------------------------------*/
