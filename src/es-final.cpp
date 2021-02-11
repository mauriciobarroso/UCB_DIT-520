/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "/home/mauricio/iot-ucb/dit-es/src/es-final.ino"
/*
 * app.ino
 *
 * Created on: Jan 27, 2021
 * Author: Mauricio Barroso Benavides
 * Link video: ...
 */

/* includes ------------------------------------------------------------------*/

#include "Particle.h"

/* Components */
#include "sensors.h"
#include "actuators.h"
#include "data_logger.h"

/* macros --------------------------------------------------------------------*/

/* Pins definition */
void setup();
void loop();
#line 21 "/home/mauricio/iot-ucb/dit-es/src/es-final.ino"
#define DHT_PIN	        D2		      /*!< DHT sensor input pin */
#define LDR_PIN	        A5		      /*!< LDR sensor input pin */
#define PIR_PIN	        D8		      /*!< PIR sensor input pin */
#define RELAY_PIN	      D7		      /*!< Relay output pin */
#define SERVO_PIN       D3          /*!< Servo motor output pin */
#define MOTOR_PIN       D5          /*!< DC motor output pin */

/* Times definition */
#define POLLING_TIME    5000                        /*!< Time to get sensors values samples in ms */
#define PUBLISH_TIME    300000                      /*!< Time to publish sensors values in ms */
#define SAMPLES_NUMBER  PUBLISH_TIME / POLLING_TIME /*!< Number of samples before publish */
#define MAX_DELAY       0xFFFFFFFF                  /*!< Maximum delay value */

/* typedef -------------------------------------------------------------------*/

/* Application struct */
typedef enum
{
  SENSORS_DATA = 0, /*!< Sensors data type */
  DATA_LOGGER_DATA  /*!< Data logger data type */
} data_type_e;

typedef struct
{
  data_type_e data_type;  /*!< Data type */
  void * data;            /*!< Pointer to data */
} app_t;


/* function declaration ------------------------------------------------------*/

/* Application functions */
static bool app_init(void);
static void app_sensors(void * arg);
static void app_actuators(void * arg);
static void app_data_logger(void * arg);
static void app_manager(void * arg);

/* Utilities */
static void process_sensors_data(sensors_values_t * sensors_values, actuators_values_t * actuators_values);
static void pir_isr(void);
int get_log(String val);

/* data declaration ----------------------------------------------------------*/

/* Components instances */
sensors_t sensors;
actuators_t actuators;
data_logger_t data_logger;

/* RTOS variables */
os_queue_t app_rx_queue;
os_queue_t app_rx_actuatos_queue;
os_queue_t app_rx_data_logger_queue;
os_queue_t app_tx_data_logger_queue;

/* setup ---------------------------------------------------------------------*/

void setup() 
{
  /* Enable thread functions */
  SYSTEM_THREAD(ENABLED);

  /* Initialise app */
  app_init();

  /* Create RTOS tasks */
  os_thread_create(NULL, "Sensors Task", OS_THREAD_PRIORITY_DEFAULT + 1, app_sensors, NULL, OS_THREAD_STACK_SIZE_DEFAULT);
  os_thread_create(NULL, "Actuators Task", OS_THREAD_PRIORITY_DEFAULT + 2, app_actuators, NULL, OS_THREAD_STACK_SIZE_DEFAULT);
  os_thread_create(NULL, "Data Logger Task", OS_THREAD_PRIORITY_DEFAULT + 3, app_data_logger, NULL, OS_THREAD_STACK_SIZE_DEFAULT);
  os_thread_create(NULL, "Manager Task", OS_THREAD_PRIORITY_DEFAULT + 4, app_manager, NULL, OS_THREAD_STACK_SIZE_DEFAULT);
}

/* infinite Loop -------------------------------------------------------------*/

void loop() 
{

}

/* function definition -------------------------------------------------------*/

static bool app_init(void)
{
  /* Initialise serial port */
  Serial.begin(9600);

  /* Create queues */
  os_queue_create(&app_rx_queue, sizeof(app_t), 3, NULL);
  os_queue_create(&app_rx_actuatos_queue, sizeof(actuators_values_t *), 2, NULL);
  os_queue_create(&app_rx_data_logger_queue, sizeof(sensors_values_t *), 2, NULL);

  /* Initilise sensors */
  sensors_pins_t sensors_pins =
  {
    .dht = DHT_PIN,
    .ldr = LDR_PIN,
    .pir = PIR_PIN,
  };

  if(!sensors_init(&sensors, &sensors_pins))
    return false;

  /* Initilise actuators */
  actuators_pins_t actuators_pins =
  {
    .servo = SERVO_PIN,
    .motor = MOTOR_PIN,
    .relay = RELAY_PIN,
  };

  if(!actuators_init(&actuators, &actuators_pins))
    return false;

  /* Initialize data logger */
  data_logger.rtc.date.date = Time.day();
  data_logger.rtc.date.month = Time.month();
  data_logger.rtc.time.hours = Time.hour();
  data_logger.rtc.time.minutes = Time.minute();
  data_logger_init(&data_logger);

  Serial.print("Index: ");
  Serial.println(data_logger.index);

  /* Configure interrupts */
  attachInterrupt(PIR_PIN, pir_isr, CHANGE);

  /* Cloud functions */
  Particle.function("log", get_log);

  /* Return true if evertything is ok */
  return true;
}

static void app_sensors(void * arg)
{
  app_t queue;
  sensors_values_t sensors_values;
  system_tick_t last_wake_time = 0;

  /* Initialize app structure */
  queue.data_type = SENSORS_DATA;
  queue.data = (void *)&sensors_values;

  for(;;)
  {
    /* Wait for 5000 ms*/
    os_thread_delay_until(&last_wake_time, POLLING_TIME);
      
    /* Get sensors values */
    sensors_get_values(&sensors, &sensors_values);

    /* Print data */
    // Serial.println("*********************");
    // Serial.println("DHT values:");
    // Serial.print("hum:");
    // Serial.println(sensors_values.hum);
    // Serial.print("temp:");
    // Serial.println(sensors_values.temp);
    // Serial.println("*********************");

    /* Send sensors values to Manager task */
    os_queue_put(app_rx_queue, &queue, 0, NULL);
  }
}

static void app_actuators(void * arg)
{
  actuators_values_t actuators_values;

  for(;;)
  {
    if(!os_queue_take(app_rx_actuatos_queue, &actuators_values, 0xFFFFFFFF, NULL))
    {
      /* Print actuators values */
      // Serial.println("*********************");
      // Serial.println("Actuators values:");
      // Serial.print("servo:");
      // Serial.println(actuators_values.servo);
      // Serial.print("motor:");
      // Serial.println(actuators_values.motor);
      // Serial.println("*********************");

      /* Set actuators values */
      actuators_set_values(&actuators, &actuators_values);
    }
  }
}

static void app_data_logger(void * arg)
{
  sensors_values_t * sensors_values;

  for(;;)
  {
    if(!os_queue_take(app_rx_data_logger_queue, &sensors_values, MAX_DELAY, NULL))
    {
      /* Print incoming data */
      // Serial.println("*********************");
      // Serial.println("Data Logger:");
      // Serial.print("data_1:");
      // Serial.println(sensors_values->hum);
      // Serial.print("data_2:");
      // Serial.println(sensors_values->temp);
      // Serial.println("*********************");
      
      /* Store values in EEPROM */
      data_logger_write(&data_logger, (uint8_t)sensors_values->temp, (uint8_t)sensors_values->hum);
    }
  }
}

static void app_manager(void * arg)
{
  app_t rx_queue;
  actuators_values_t actuators_values;
  uint8_t data_counter = 0;
  sensors_values_t array[SAMPLES_NUMBER];  /* TODO: obtain the number with macros */

  for(;;)
  {
    /* Wait for messages */
    if(!os_queue_take(app_rx_queue, &rx_queue, MAX_DELAY, NULL))
    {
      switch(rx_queue.data_type)
      {
        case SENSORS_DATA:
        {
          sensors_values_t * sensors_values = (sensors_values_t *)rx_queue.data;

          /* Average sensors values samples */
          array[data_counter++] = * sensors_values;
          
          float temp_sum = 0;
          float hum_sum = 0;
          for(uint8_t i = 0; i < data_counter; i++)
          {
            temp_sum += array[i].temp;
            hum_sum += array[i].hum;
          }

          /* Publish every 5 minutes */
          if(data_counter == SAMPLES_NUMBER)  /* TODO: obtain the number with macros */
          {
            /* Publish variables */
            char buff[128];
            snprintf(buff, sizeof(buff), "[%f, %f]", (temp_sum / data_counter), (hum_sum / data_counter));
            Particle.publish("environment", buff, PRIVATE);

            /* Reset data counter */
            data_counter = 0;

            /* Send data to data logger queue */
            os_queue_put(app_rx_data_logger_queue, &sensors_values, 0, NULL);
          }
                
          /* Process data */       
          process_sensors_data(sensors_values, &actuators_values);

          /* Send data to actuators queue */
          os_queue_put(app_rx_actuatos_queue, &actuators_values, 0, NULL);

          break;
        }

        case DATA_LOGGER_DATA:
        {
          data_logger_t * data_logger_values = (data_logger_t *)rx_queue.data;

          /* Print values in EEPROM */
          data_logger_print(data_logger_values);

          break;
        }
        
        default:
          Serial.println("Unknow");

          break;
      }
    }
    else  
      Serial.println("Error receiving data");
  }
}

static void process_sensors_data(sensors_values_t * sensors_values, actuators_values_t * actuators_values)
{
  if(sensors_values->temp <= 23)
  {
    actuators_values->servo = SERVO_DEFAULT_MIN_ANGLE;
    actuators_values->motor = 0;
  }
  else if(sensors_values->temp > 25)
  {
    actuators_values->servo = SERVO_DEFAULT_MAX_ANGLE;
    actuators_values->motor = 255;
  }
}

static void pir_isr(void)
{
  if(digitalRead(PIR_PIN))
  {
    if(analogRead(LDR_PIN) < 1000)
      digitalWrite(RELAY_PIN, HIGH);
  }
  else
    digitalWrite(RELAY_PIN, LOW);

  os_thread_yield();
}

int get_log(String val)
{
  if(val == "get")
  {
    app_t queue =
    {
      .data_type = DATA_LOGGER_DATA,
      .data = (void *)&data_logger,
    };

    os_queue_put(app_rx_queue, &queue, 0, NULL);

    return 1;
  }
  
  return -1;
}