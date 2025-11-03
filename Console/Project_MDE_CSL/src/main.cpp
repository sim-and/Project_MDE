#include <Arduino.h>

#include <statemachine.h>
#include <displays.h>
#include <buttons.h>
#include <communication.h>

/*int tapts = 0, tbpts = 0;
Countimer tDown;*/

void print_time2(){
}

/*void tDownComplete()
{
	digitalWrite(13, LOW);
}*/

void setup() {
  initlcddisplay();
  initI2C7seg();
  init7seg();
  initkeypad();
  initbuttons();

  Serial.begin(9600);

  /*tDown.setCounter(0, 9, 21, tDown.COUNT_DOWN, tDownComplete);
    // Call print_time2() method every 1s.*/
	gameclk.setInterval(print_time2, 1000);

  system_state = STANDBY;
}

void loop() {
  Console_state_machine();
  getcurrenttime();

  /*gameclkdisplay.print(tDown.getCurrentGameTime());
  gameclkdisplay.writeDisplay();
  tDown.run();
  tDown.start();
  menulcd.setCursor(7,2); //menulcd.print("1234"); 
  menulcd.blink();*/
}

