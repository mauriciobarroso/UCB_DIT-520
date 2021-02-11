/*
 * data_logger.h
 *
 * Created on: Jan 30, 2021
 * Author: Mauricio Barroso Benavides
 */

#ifndef _DATA_LOGGER_H_
#define _DATA_LOGGER_H_

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
  uint8_t hours;
  uint8_t minutes;
} rtc_time_t;

typedef struct
{
  uint8_t date;
  uint8_t month;
} rtc_date_t;

typedef struct
{
  rtc_time_t time;
  rtc_date_t date;
} rtc_t;

/**/
typedef struct
{
  rtc_t rtc;
  uint16_t index;
  uint8_t data[2];
} data_logger_t;

/* external data declaration -------------------------------------------------*/

/* external functions declaration --------------------------------------------*/

void data_logger_init(data_logger_t * const me);
data_logger_t data_logger_read(data_logger_t * const me, uint16_t index);
void data_logger_write(data_logger_t * const me, uint8_t temp, uint8_t hum);
void data_logger_print(data_logger_t * const me);

/* cplusplus -----------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/** @} doxygen end group definition */

/* end of file ---------------------------------------------------------------*/

#endif /* #ifndef _DATA_LOGGER_H_ */
