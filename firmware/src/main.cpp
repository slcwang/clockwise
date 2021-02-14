#include <Arduino.h>
#include <PxMatrix.h>
// Clockface
#include <Clockface.h>
// Commons
#include <WiFiConnect.h>
#include <DateTime.h>

#ifdef ESP32

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 16
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#endif

// Pins for LED MATRIX

#define matrix_width 64
#define matrix_height 64


// This defines the 'on' time of the display is us. The larger this number,
// the brighter the display. If too large the ESP will crash
uint8_t display_draw_time=50; //10-50 is usually fine


// PxMATRIX display(matrix_width,matrix_height,P_LAT, P_OE,P_A,P_B,P_C);
//PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
PxMATRIX display(matrix_width,matrix_height,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16_t myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};


Display engine_display(&display);
Clockface clockface(&engine_display);

WiFiConnect wifi;
DateTime dateTime;


#ifdef ESP32
void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}
#endif


void display_update_enable(bool is_enable)
{

#ifdef ESP32
  if (is_enable)
  {
    timer = timerBegin(0, 40, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 4000, true);
    timerAlarmEnable(timer);
  }
  else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
#endif
}

void setup() 
{
  Serial.begin(9600);


  pinMode(2, OUTPUT);

  wifi.connect();
  dateTime.begin();

  display.begin(32);  
  display.clearDisplay();

  display_update_enable(true);

  display.setBrightness(20);

  clockface.setup(&dateTime);  
}


bool led = false;
unsigned long mainLastMillis = 0;

void loop() 
{  
  clockface.update();

  if (millis() - mainLastMillis > 1000) {
    digitalWrite(2, led ? HIGH : LOW);
    led = !led;  

    mainLastMillis = millis();
  }  
}
