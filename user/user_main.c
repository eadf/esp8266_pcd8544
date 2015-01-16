

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "osapi.h"
#include "os_type.h"
#include "include/user_config.h"
#include "user_interface.h"
#include "driver/stdout.h"
#include "driver/pcd8544.h"

#define user_procTaskPeriod      1000

static volatile os_timer_t lcd_timer;

void user_init(void);
static void loop(os_event_t *events);

//Main code function
static void ICACHE_FLASH_ATTR
loop(os_event_t *events) {
  static bool toggle = true;
  static bool firstTime = true;
  if (firstTime) {
    // I wonder why the calls in user_init doesn't 'take'
    PCD8544_initLCD();
    os_delay_us(50000);
    PCD8544_lcdClear();
    firstTime = false;
  }

  if (true){
    // Draw a Box
    PCD8544_drawLine();
    int a=0;
    PCD8544_gotoXY(17,1);
    // Put text in Box
    PCD8544_lcdString("ESP8266");
    PCD8544_gotoXY(24,3);
    if (toggle){
      PCD8544_lcdCharacter('H');
      PCD8544_lcdCharacter('E');
      PCD8544_lcdCharacter('L');
      PCD8544_lcdCharacter('L');
      PCD8544_lcdCharacter('O');
      PCD8544_lcdCharacter(' ');
      PCD8544_lcdCharacter('=');
      // Draw + at this position
      PCD8544_gotoXY(10,3);
      PCD8544_lcdCharacter('=');
      os_delay_us(50000);
    } else {
      PCD8544_gotoXY(24,3);
      PCD8544_lcdCharacter('h');
      PCD8544_lcdCharacter('e');
      PCD8544_lcdCharacter('l');
      PCD8544_lcdCharacter('l');
      PCD8544_lcdCharacter('o');
      PCD8544_lcdCharacter(' ');
      PCD8544_lcdCharacter('-');
      // Draw - at this position
      PCD8544_gotoXY(10,3);
      PCD8544_lcdCharacter('-');
      os_delay_us(50000);
    }
    toggle = !toggle;
  } else {
    os_delay_us(50000);
    PCD8544_lcdClear();
    os_delay_us(50000);
    PCD8544_gotoXY(24,3);
    PCD8544_lcdCharacter('H');
    os_delay_us(50000);
    PCD8544_drawLine();
    PCD8544_gotoXY(4,1);
    // Put text in Box
    PCD8544_lcdString("ESP8266");
    os_delay_us(50000);
  }
  os_timer_disarm(&lcd_timer);
  os_timer_arm(&lcd_timer, user_procTaskPeriod, 0);
}

//Init function 
void ICACHE_FLASH_ATTR
user_init(void)
{
  //Set station mode
  wifi_set_opmode( NULL_MODE );

  // Make os_printf working again. Baud:115200,n,8,1
  stdoutInit();
  PCD8544_init();
  PCD8544_initLCD();
  os_printf("System started\n\r");
  //Start os task
  os_timer_setfn(&lcd_timer, (os_timer_func_t*) loop, NULL);
  os_timer_arm(&lcd_timer, user_procTaskPeriod, 0);
}
