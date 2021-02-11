/*
 * sensors.h
 *
 * Created on: Jan 30, 2021
 * Author: Mauricio Barroso Benavides
 */

#ifndef _ACTUATORS_H_
#define _ACTUATORS_H_

/* inclusions ----------------------------------------------------------------*/

#include "Particle.h"

/* cplusplus -----------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* macros --------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/

/**/
typedef struct
{
  uint16_t servo;
  uint16_t motor;
  uint16_t relay;
} actuators_pins_t;

typedef struct
{
  bool servo;
  uint8_t motor;
  bool relay;
} actuators_values_t;


typedef struct
{
  Servo servo;                /*!<  */
  actuators_pins_t pins;      /*!<  */
  actuators_values_t values;  /*!<  */
} actuators_t;

/* external data declaration -------------------------------------------------*/

/* external functions declaration --------------------------------------------*/

bool actuators_init(actuators_t * const me, actuators_pins_t * pins);
void actuators_set_values(actuators_t * const me, actuators_values_t * values);

/* cplusplus -----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/** @} doxygen end group definition */

/* end of file ---------------------------------------------------------------*/

#endif /* #ifndef _ACTUATORS_H_ */
