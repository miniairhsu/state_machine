#include <Arduino.h>
#include "qpn.h"
#include "ClockAlarm_SM.h"
#include "button_SM.h"
#include "lcd.h"
#include "main.h"
Q_DEFINE_THIS_FILE;

static void Timer1_setup(void);
static void display_init(void);
static void attach_button_interrupts(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  display_init();
  Clock_Alarm_ctor();
  Button_ctor();
  Timer1_setup();
}

void loop() {
}

static void Timer1_setup(void){
  TCCR1A = 0;                 //CTC mode            
  TCCR1B = B00001100;        //prescaler=256,CTC mode
  TIMSK1 |= B00000010;       //Interrupt enable for OCR1A compare match
  OCR1A = 6250-1;          //OC match value for 100ms time base generation
}

void SET_handler(){
  
  QF_INT_DISABLE();
  if(flag_report_button_press){
    flag_report_button_press = false;
    QActive_armX(AO_Button,0,MS_TO_TICKS(50),0U);
  }
  QF_INT_ENABLE();

}

void OK_handler(){
  QF_INT_DISABLE();
  if(flag_report_button_press){
    flag_report_button_press = false;
    QActive_armX(AO_Button,0,MS_TO_TICKS(50),0U);
  }
  QF_INT_ENABLE();
}

static void attach_button_interrupts(void)
{
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON1), SET_handler, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON2), OK_handler, RISING);
}

static void display_init(void)
{
  lcd_begin(16,2);
  lcd_clear();
  lcd_move_cursor_L_to_R();
  lcd_set_cursor(0,0);
  lcd_no_auto_scroll();
  lcd_cursor_off();
}

Q_NORETURN Q_onAssert	(	char_t const Q_ROM *const 	module,int_t const 	location ){

  Serial.println("Assertion failure!!");
  Serial.println((String)module);
  Serial.println(location);
  while(1);
}	