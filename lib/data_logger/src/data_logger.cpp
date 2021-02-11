/*
 * data_logger.c
 *
 * Created on: Jan 30, 2021
 * Author: Mauricio Barroso Benavides
 */

/* inclusions ----------------------------------------------------------------*/

#include "data_logger.h"

/* macros --------------------------------------------------------------------*/

#define INDEX_ADDR          0x0 /*!< RTC hours address register */
#define DATA_SIZE           6   /*!< Data size in bytes */
#define BASE_INDEX          0x2 /*!< Data size in bytes */

/* typedef -------------------------------------------------------------------*/

/* internal data declaration -------------------------------------------------*/

/* external data declaration -------------------------------------------------*/

/* internal functions declaration --------------------------------------------*/

static void eeprom_write16(uint16_t address, uint16_t data);
static uint16_t eeprom_read16(uint16_t address);

/* external functions definition ---------------------------------------------*/

void data_logger_init(data_logger_t * const me)
{
    EEPROM.begin();

    // EEPROM.clear();

    /* Get index from EEPROM */
    uint16_t data = eeprom_read16(INDEX_ADDR);
    if(data == 0xFFFF || data < BASE_INDEX)
    {
        eeprom_write16(INDEX_ADDR, BASE_INDEX);
        me->index = eeprom_read16(INDEX_ADDR);
    }
    else
        me->index = data;
}

data_logger_t data_logger_read(data_logger_t * const me, uint16_t index)
{
    data_logger_t data;

    data.rtc.date.date = EEPROM.read(index);
    data.rtc.date.month = EEPROM.read(index + 1);
    data.rtc.time.hours = EEPROM.read(index + 2);
    data.rtc.time.minutes = EEPROM.read(index + 3);
    data.data[0] = EEPROM.read(index + 4);
    data.data[1] = EEPROM.read(index + 5);

    return data;
}

void data_logger_write(data_logger_t * const me, uint8_t temp, uint8_t hum)
{
    EEPROM.write(me->index, (uint8_t)Time.day());
    EEPROM.write(me->index + 1, (uint8_t)Time.month());
    EEPROM.write(me->index + 2, (uint8_t)Time.hour());
    EEPROM.write(me->index + 3, (uint8_t)Time.minute());
    EEPROM.write(me->index + 4, temp);
    EEPROM.write(me->index + 5, hum);

    me->index += DATA_SIZE;
    // EEPROM.write(INDEX_ADDR, me->index);
    eeprom_write16(INDEX_ADDR, me->index);

    // me->index = EEPROM.read(INDEX_ADDR);
    me->index = eeprom_read16(INDEX_ADDR);
}

void data_logger_print(data_logger_t * const me)
{
    char string[64];

    Serial.println("*******************************");
    Serial.println("Log:");
    sprintf(string, "date, time, temperature,humidity");
    Serial.println(string);

    for(uint8_t i = BASE_INDEX; i < me->index; i += DATA_SIZE)
    {
        data_logger_t values = data_logger_read(me, i);

        sprintf(string, "%02d/%02d,%02d:%02d,      %02d,      %02d", values.rtc.date.date, values.rtc.date.month, values.rtc.time.hours, values.rtc.time.minutes, values.data[0], values.data[1]);

        Serial.println(string);
    }

    Serial.println("*******************************");
}

/* internal functions definition ---------------------------------------------*/

static void eeprom_write16(uint16_t address, uint16_t data)
{
    EEPROM.write(address, (uint8_t)(data & 0xFF00) >> 8);
    EEPROM.write(address + 1, (uint8_t)(data & 0xFF));
}

static uint16_t eeprom_read16(uint16_t address)
{
    uint8_t data[2] = {0};

    data[0] = EEPROM.read(address);
    data[1] = EEPROM.read(address + 1);

    return (uint16_t)(data[0] << 8) | (uint16_t )data[1];
}

/* end of file ---------------------------------------------------------------*/