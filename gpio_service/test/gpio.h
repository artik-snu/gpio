
#define BUFF_SIZE 128

typedef struct {
  long data_type;
  long data_num;
  char data_buff[BUFF_SIZE];
} msg_data;


typedef enum
{
    GPIO_ERROR_NONE                  ,                 /**< Successful */
    GPIO_ERROR_IO_ERROR              ,             /**< I/O error */
    GPIO_ERROR_INVALID_PARAMETER     ,    /**< Invalid parameter */
    GPIO_ERROR_NOT_SUPPORTED         ,        /**< Not supported */
    GPIO_ERROR_PERMISSION_DENIED     ,    /**< Permission denied */
    GPIO_ERROR_OUT_OF_MEMORY         ,        /**< Out of memory */
    GPIO_ERROR_NO_DATA                             /**< No data available
                                                                                @if MOBILE (Since 3.0) @elseif WEARABLE (Since 2.3.2) @endif */
 } gpio_error_e;

typedef enum {
    GPIO_OPEN_PIN,
    GPIO_CLOSE_PIN,
    GPIO_SET_DIRECTION,
    GPIO_GET_DIRECTION,
    GPIO_SET_VALUE,
    GPIO_GET_VALUE
} gpio_msg_e;

typedef enum {
    GPIO_IN = 0,
    GPIO_OUT = 1
} gpio_direction_e;

typedef enum {
    LOW = 0,
    HIGH = 1
} gpio_value_e;

typedef enum {
  GPX0 = 0x300,
  GPX1 = 0x308,
  GPA0 = 0x00,
  GPA1 = 0x08,
  GPA2 = 0x10,
  GPD0 = 0x28,
  GPB2 = 0x28
} gpio_port_e;

typedef enum {
  GPX0_0 = (GPX0 << 3) + 0,
  GPX0_1 = (GPX0 << 3) + 1,
  /* More to be added */
  GPX1_0 = (GPX1 << 3) + 0,
  GPX1_5 = (GPX1 << 3) + 5,
  GPX1_6 = (GPX1 << 3) + 6,
  J27_11 = GPX1_0,
  J27_12 = GPX1_5,
  J27_13 = GPX1_6
} gpio_pin_e;


typedef struct {
  gpio_pin_e pin;
  gpio_direction_e direction;
} gpio_t;

typedef gpio_t* gpio_h;

