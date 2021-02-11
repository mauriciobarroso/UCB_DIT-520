/*
 * sensors.h
 *
 * Created on: Jan 30, 2021
 * Author: Mauricio Barroso Benavides
 */

#ifndef _SENSORS_H_
#define _SENSORS_H_

/* inclusions ----------------------------------------------------------------*/

#include "Particle.h"
#include "Grove_Temperature_And_Humidity_Sensor.h"

/* cplusplus -----------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* macros --------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/

typedef struct
{
  uint16_t dht; /*!< DHT pin */
  uint16_t ldr; /*!< LDR pìn */
  uint16_t pir; /*!< PIR pin */ 
} sensors_pins_t;

typedef struct
{
  float hum;      /*!< Humidity variable */
	float temp;     /*!< Temperature variable */
  uint16_t light; /*!< Incident light variable */
	bool presence;  /*!< Presence variable */
} sensors_values_t;

typedef struct
{
  sensors_pins_t pins;      /*!<  */ 
  sensors_values_t values;  /*!<  */ 
} sensors_t;

/* external data declaration -------------------------------------------------*/

/* external functions declaration --------------------------------------------*/


bool sensors_init(sensors_t * const me, sensors_pins_t * pins);
void sensors_get_values(sensors_t * const me, sensors_values_t * values);

/* cplusplus -----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/** @} doxygen end group definition */

/* end of file ---------------------------------------------------------------*/

#endif /* #ifndef _SENSORS_H_ */
