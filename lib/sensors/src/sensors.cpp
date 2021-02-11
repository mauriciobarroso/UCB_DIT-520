/*
 * mcp401x.c
 *
 * Created on: Jan 13, 2021
 * Author: Mauricio Barroso Benavides
 */

/* inclusions ----------------------------------------------------------------*/

#include "sensors.h"

/* macros --------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/

/* internal data declaration -------------------------------------------------*/

/* external data declaration -------------------------------------------------*/

/* internal functions declaration --------------------------------------------*/

/* external functions definition ---------------------------------------------*/


bool sensors_init(sensors_t * const me, sensors_pins_t * pins)
{
  /* Initialise DHT */
  me->pins.dht = pins->dht;
  me->values.hum = 0;
  me->values.temp = 0;

  /* Initialise LDR */
  me->pins.ldr = pins->ldr;
  me->values.light = 0;

  /* Initialise PIR */
  me->pins.pir = pins->pir;
  pinMode(me->pins.pir, INPUT);
  me->values.presence = 0;

  return true;
}

void sensors_get_values(sensors_t * const me, sensors_values_t * values)
{
  /* Get DHT values */
  DHT dht(me->pins.dht);

  float temp_tmp = dht.getTempCelcius();
  if(!isnan(temp_tmp) && temp_tmp >= 0 && temp_tmp <= 80)
  {
    me->values.temp = temp_tmp;
    values->temp = me->values.temp;
  }

  float hum_tmp = dht.getHumidity();
  if(!isnan(hum_tmp) && hum_tmp >= 0 && hum_tmp <= 95)
  {
    me->values.hum = hum_tmp;
    values->hum = me->values.hum;
  }

  /* Get LDR values */
  uint16_t light_tmp = analogRead(me->pins.ldr);
  if(light_tmp >= 0 && light_tmp <= 4095)
  {
    me->values.light = light_tmp;
    values->light = me->values.light;
  }

  /* Get PIR values */
  me->values.presence = digitalRead(me->pins.pir);
  values->presence = me->values.presence;
}

/* internal functions definition ---------------------------------------------*/

/* end of file ---------------------------------------------------------------*/
