#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <displays.h>
#include <buttons.h>
#include <communication.h>

#include <countimer.h>


void configgame(int sport);
void loadnexttime();
void loadnexttime2();
void displaygamedata();
void controlgamedata();
void senddata();

int sport;
int loadtime = 1;

enum STATE_ENUM {
    STANDBY,
    START,
    CONFIG_GAME,
    RUN_GAME,
    EXIT,
    EDIT
} system_state;

Countimer gameclk;
int settimes[8];
int setperiods[8];
int tapts = 0, tafls = 0, tatos = 0, tbpts = 0, tbfls = 0, tbtos = 0, gameper = 0;
int maxpts, maxfls, maxtos, ngameper;

bool showMessage = true; // To toggle the message display
unsigned long lastBlinkTime = 0; // For controlling the blink timing
const unsigned long blinkInterval = 1000; // Blink interval in milliseconds

void Console_state_machine() {
    unsigned long currentTime = 0; // Declare outside the switch
    keypadEvent e; // Declare outside the switch
    char timedata[20];

    switch(system_state) {
        case STANDBY: 
            menukeypad.tick();
            e = menukeypad.read();

            currentTime = millis();
            if (currentTime - lastBlinkTime >= blinkInterval) {
                lastBlinkTime = currentTime;
                showMessage = !showMessage;

                if (showMessage) {
                    menulcd.setCursor(3, 0);    
                    menulcd.print("PRESS ANY KEY");
                    menulcd.setCursor(2, 1);  
                    menulcd.print("TO START A GAME");
                    menulcd.setCursor(17, 3); menulcd.print(":");
                } else {
                    menulcd.clear();
                }
            }

            menulcd.setCursor(0, 3); /*menulcd.print(currenttime.day);
            menulcd.setCursor(2, 3); menulcd.print("/");
            menulcd.setCursor(3, 3); menulcd.print(currenttime.month);
            menulcd.setCursor(5, 3); menulcd.print("/");
            menulcd.setCursor(6, 3); menulcd.print(currenttime.year);
            menulcd.setCursor(15, 3); menulcd.print(currenttime.hour);
            menulcd.setCursor(17, 3); menulcd.print(":");
            menulcd.setCursor(18, 3); menulcd.print(currenttime.minute);*/

            sprintf(timedata, "%02d/%02d/%04d     %02d:%02d", currenttime.day, currenttime.month, currenttime.year, currenttime.hour, currenttime.minute);
            menulcd.print(timedata);

            if (e.bit.EVENT == KEY_JUST_PRESSED) {
                delay(50);
                system_state = START;
            }
            break;

        case START:
            static bool startScreenDisplayed = false; // Tracks whether the START screen is displayed

            if (!startScreenDisplayed) {
                // Clear the screen only once when entering the START state
                menulcd.clear(); // Clear the entire display
                menulcd.setCursor(4, 0); menulcd.print("SELECT MODE:");
                menulcd.setCursor(4, 1); menulcd.print("A"); menulcd.write(0); menulcd.print("MANUAL");
                menulcd.setCursor(4, 2); menulcd.print("B"); menulcd.write(0); menulcd.print("AUTOMATIC");
                menulcd.setCursor(0, 3); menulcd.print("#"); menulcd.write(0); menulcd.print("BACK");
                startScreenDisplayed = true; // Mark the START screen as displayed
            }
            menulcd.setCursor(15, 3); menulcd.print(currenttime.hour);
            menulcd.setCursor(18, 3); menulcd.print(currenttime.minute);
            menulcd.setCursor(17, 3); menulcd.print(":");

            menukeypad.tick();
            e = menukeypad.read();

            if (e.bit.EVENT == KEY_JUST_PRESSED) {
                delay(50);
                switch((char)e.bit.KEY) {
                    case 'A': system_state = RUN_GAME; sport = 0; break;
                    case 'B': system_state = CONFIG_GAME; break;
                    case '#': startScreenDisplayed = false; system_state = STANDBY; break;
                }
            }
            break; 

        case CONFIG_GAME:
            static bool configScreenDisplayed = false; // Tracks whether the START screen is displayed

            if (!configScreenDisplayed) {
                // Clear the screen only once when entering the START state
                menulcd.clear(); // Clear the entire display
                menulcd.setCursor(3, 0); menulcd.print("SELECT SPORT:");
                menulcd.setCursor(2, 1); menulcd.print("A"); menulcd.write(0); menulcd.print("BBALL");
                menulcd.setCursor(11, 1); menulcd.print("FBALL"); menulcd.write(0); menulcd.print("B");
                menulcd.setCursor(2, 2); menulcd.print("C"); menulcd.write(0); menulcd.print("HBALL");
                menulcd.setCursor(11, 2); menulcd.print("FTSAL"); menulcd.write(0); menulcd.print("D");
                menulcd.setCursor(0, 3); menulcd.print("#"); menulcd.write(0); menulcd.print("BACK");
                configScreenDisplayed = true; // Mark the START screen as displayed
            }
            
            menulcd.setCursor(15, 3); menulcd.print(currenttime.hour);
            menulcd.setCursor(18, 3); menulcd.print(currenttime.minute);
            menulcd.setCursor(17, 3); menulcd.print(":");

            
            menukeypad.tick();
            e = menukeypad.read();
            
            if(e.bit.EVENT == KEY_JUST_PRESSED){    
                switch((char)e.bit.KEY){
                    case 'A' : configgame(1); system_state = RUN_GAME; sport = 1; break;
                    case 'B' : configgame(2); system_state = RUN_GAME; sport = 2; break;
                    case 'C' : configgame(3); system_state = RUN_GAME; sport = 3; break;
                    case 'D' : configgame(4); system_state = RUN_GAME; sport = 4; break;
                    case '#' : startScreenDisplayed = false; configScreenDisplayed = false; system_state = START; break;
                }
            }
            break;

        case RUN_GAME:{
            gameclk.run();
            
            controlgamedata();

            displaygamedata();

            //senddata();
            
            static bool showgamerunningdisplay = false; // Tracks whether the START screen is displayed
            static bool showgamestoppeddisplay = false;
            
            if(gameclk.isStopped()){
                if (!showgamestoppeddisplay) {
                    menulcd.clear();// Clear the screen only once
                    menulcd.setCursor(0, 0); menulcd.print("M:"); menulcd.setCursor(13, 0); menulcd.print("S:");
                    switch(sport){
                        case 0: menulcd.setCursor(2, 0); menulcd.print("MANUAL"); menulcd.setCursor(15, 0); menulcd.print("-----"); break;
                        case 1: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("BBALL"); break;
                        case 2: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("FBALL"); break;
                        case 3: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("HBALL"); break;
                        case 4: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("FTSAL"); break;
                    }
                    menulcd.setCursor(1, 1); menulcd.print("GAME CLOCK");menulcd.setCursor(13, 1); menulcd.print("PERIOD");
                    menulcd.setCursor(3, 2); menulcd.print(gameclk.getCurrentGameTime()); menulcd.setCursor(15, 2); menulcd.print(gameper);
                    menulcd.setCursor(0, 3); menulcd.print("#"); menulcd.write(0); menulcd.print("EXIT"); 
                    menulcd.setCursor(7, 3); menulcd.print("*"); menulcd.write(0); menulcd.print("EDIT");
                    showgamerunningdisplay = false; // Mark the START screen as displayed
                    showgamestoppeddisplay = true;
                }
                
                menulcd.setCursor(15, 3); menulcd.print(currenttime.hour);
                menulcd.setCursor(18, 3); menulcd.print(currenttime.minute);
                menulcd.setCursor(17, 3); menulcd.print(":");

                            
                menukeypad.tick();
                e = menukeypad.read();
                
                if (e.bit.EVENT == KEY_JUST_PRESSED){    
                    switch((char)e.bit.KEY){
                        case '*' : system_state = EDIT; showgamestoppeddisplay =false; break;
                        case '#' : system_state = EXIT; showgamestoppeddisplay =false; break;
                    }
                }
            }
            else {
                currentTime = millis();
                if (currentTime - lastBlinkTime >= blinkInterval) {
                        lastBlinkTime = currentTime;
                        showMessage = !showMessage;

                        if (showMessage) {
                            menulcd.setCursor(5, 1);    
                            menulcd.print("GAME CLOCK");
                            menulcd.setCursor(6, 2);  
                            menulcd.print("RUNNING");
                            menulcd.setCursor(17, 3); menulcd.print(":");
                        } else {
                            menulcd.clear();
                        }
                }
                
                menulcd.setCursor(0, 0); menulcd.print("M:"); menulcd.setCursor(13, 0); menulcd.print("S:");
                    switch(sport){
                        case 0: menulcd.setCursor(2, 0); menulcd.print("MANUAL"); menulcd.setCursor(15, 0); menulcd.print("-----"); break;
                        case 1: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("BBALL"); break;
                        case 2: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("FBALL"); break;
                        case 3: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("HBALL"); break;
                        case 4: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("FTSAL"); break;
                    }
                    menulcd.setCursor(0, 3); /*menulcd.print(currenttime.day);
                    menulcd.setCursor(2, 3); menulcd.print("/");
                    menulcd.setCursor(3, 3); menulcd.print(currenttime.month);
                    menulcd.setCursor(5, 3); menulcd.print("/");
                    menulcd.setCursor(6, 3); menulcd.print(currenttime.year);
                    menulcd.setCursor(15, 3); menulcd.print(currenttime.hour);
                    menulcd.setCursor(17, 3); menulcd.print(":");
                    menulcd.setCursor(18, 3); menulcd.print(currenttime.minute);*/
                    sprintf(timedata, "%02d/%02d/%04d     %02d:%02d", currenttime.day, currenttime.month, currenttime.year, currenttime.hour, currenttime.minute);
                    menulcd.print(timedata);
                
                if (!showgamerunningdisplay) {
                    menulcd.clear();// Clear the screen only onc
                    showgamestoppeddisplay = false; // Mark the START screen as displayed
                    showgamerunningdisplay = true;
                }

            }

            break;
        }
        case EXIT:{
            static bool showexitdisplay = false;
            if (!showexitdisplay) {   
                menulcd.clear();
                menulcd.setCursor(0, 0); menulcd.print("ARE YOU SURE");
                menulcd.setCursor(0, 1); menulcd.print("YOU WANT TO EXIT?");
                menulcd.setCursor(0, 3); menulcd.print("A"); menulcd.write(0); menulcd.print("YES  "); menulcd.print("B"); menulcd.write(0); menulcd.print("NO");
                showexitdisplay = true;
            }

            menukeypad.tick();
            e = menukeypad.read();
            
            switch((char)e.bit.KEY){
                case 'A' : break;
                case 'B' : system_state = RUN_GAME; showexitdisplay = false; break;
            }

            break;
        }
        case EDIT: {
            static bool showeditdisplay = false;

            if (!showeditdisplay) {
                menulcd.clear(); // Clear the screen only once
                menulcd.setCursor(0, 0); menulcd.print("M:"); menulcd.setCursor(13, 0); menulcd.print("S:");
                switch (sport) {
                    case 0: menulcd.setCursor(2, 0); menulcd.print("MANUAL"); menulcd.setCursor(15, 0); menulcd.print("-----"); break;
                    case 1: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("BBALL"); break;
                    case 2: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("FBALL"); break;
                    case 3: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("HBALL"); break;
                    case 4: menulcd.setCursor(2, 0); menulcd.print("AUTO"); menulcd.setCursor(15, 0); menulcd.print("FTSAL"); break;
                }
                menulcd.setCursor(1, 1); menulcd.print("GAME CLOCK"); menulcd.setCursor(13, 1); menulcd.print("PERIOD");
                menulcd.setCursor(3, 2); menulcd.print(gameclk.getCurrentGameTime()); menulcd.setCursor(15, 2); menulcd.print(gameper);
                menulcd.setCursor(2, 3); menulcd.print("*"); menulcd.write(0); menulcd.print("EXIT EDIT MENU");

                showeditdisplay = true;
            }

            uint8_t minutes = gameclk.getCurrentMinutes();
            uint8_t seconds = gameclk.getCurrentSeconds();
            
            for(int z = 0; z<5; z++) { // Loop for editing
                menukeypad.tick();
                e = menukeypad.read();

                int editnum = 0;
                uint8_t minutescpy = minutes;
                uint8_t secondscpy = seconds;

                // Check for input
                if (e.bit.EVENT == KEY_JUST_PRESSED && (48 <= (int)e.bit.KEY) && ((int)e.bit.KEY <= 57)) {
                    editnum = (int)e.bit.KEY - 48;

                    Serial.print(z);
                    switch (z) {
                        case 0: minutes = (editnum*10 + minutescpy % 10); break;
                        case 1: minutes = (minutescpy/10)*10 + editnum; break;
                        case 2: seconds = editnum*10 + secondscpy % 10;break;
                        case 3: seconds = (secondscpy/10)*10 + editnum; break;
                        case 4: gameper = editnum; break;
                    }

                    char clock[5];
                    sprintf(clock, "%02d:%02d", minutes, seconds);
                    // Update display after editing
                    menulcd.setCursor(3, 2); menulcd.print(clock);
                    menulcd.setCursor(15, 2); menulcd.print(gameper);
                }

                // Exit edit mode on '*'
                if (e.bit.EVENT == KEY_JUST_PRESSED && (char)e.bit.KEY == '*') {
                    showeditdisplay = false; // Reset for next time
                    system_state = RUN_GAME;
                    break;
                }

            }

            break;

        }

        default:
            system_state = STANDBY;
            break;
    }
}

void configgame(int sport){
    switch (sport) {
        case 1: {
            settimes[0] = 2; settimes[1] = 1; settimes[2] = 2; settimes[3] = 10;
            settimes[4] = 15; settimes[5] = 10; settimes[6] = 2; settimes[7] = 10;
            maxfls = 5; maxtos = 3; ngameper = 4;
            gameclk.setCounter(0, settimes[0], 0, gameclk.COUNT_DOWN, loadnexttime);
            break;
        }
        case 2: {
            settimes[0] = 1; settimes[1] = 45; settimes[2] = 15; settimes[3] = 45;
            maxfls = 0; maxtos = 0; ngameper = 2;
            gameclk.setCounter(0, settimes[0], 0, gameclk.COUNT_DOWN, loadnexttime2);
            break;
        }
        default:
            // Optionally handle unexpected values of `sport`
            break;
    }


}

void loadnexttime(){
    gameclk.setCounter(0, settimes[loadtime], 0, gameclk.COUNT_DOWN, loadnexttime);
    if(loadtime%2!=0){tafls = 0, tatos = 0, tbfls = 0, tbtos = 0, gameper++;}
    loadtime++;
}

void loadnexttime2(){
    //gameclk.setCounter(0, settimes[loadtime], 0, gameclk.COUNT_DOWN, loadnexttime);
    if(loadtime%2!=0){gameclk.setCounter(0, settimes[loadtime], 0, gameclk.COUNT_UP, loadnexttime); tafls = 0, tatos = 0, tbfls = 0, tbtos = 0, gameper++;}
    else{gameclk.setCounter(0, settimes[loadtime], 0, gameclk.COUNT_DOWN, loadnexttime);}
    loadtime++;
}

void displaygamedata(){
  uint8_t minutes = gameclk.getCurrentMinutes(); // Retrieve current minutes
  uint8_t seconds = gameclk.getCurrentSeconds(); // Retrieve current seconds

  // Format the time as MMSS for the display
  int timeToDisplay = (minutes * 100) + seconds;

  // Display the time
  gameclkdisplay.print(timeToDisplay);
  gameclkdisplay.drawColon(true);
  gameclkdisplay.writeDisplay();

  taptsdisplay.print(tapts);
  taptsdisplay.writeDisplay();
  tbptsdisplay.print(tbpts);
  tbptsdisplay.writeDisplay();

  displayNumber(gameper, gameperdisplay);
  displayNumber(tafls, taflsdisplay);
  displayNumber(tbfls, tbflsdisplay);
  displayNumber(tatos, tatosdisplay);
  displayNumber(tbtos, tbtosdisplay);	
}

void controlgamedata(){
    if (isButtonPressedOnce(GAMECLK_BUTTON)){
        if(gameclk.isStopped())gameclk.start();
        else gameclk.pause();
    }

    if (!digitalRead(CORRECT_BUTTON)){
        if (buttondebounce(TAPTS3_BUTTON))tafls +=1;
        /*if (isButtonPressedOnce(TATOS_BUTTON,tatos_button))tatos +=1;
        if (isButtonPressedOnce(TAPTS1_BUTTON,tapts1_button))tapts +=1;
        if (isButtonPressedOnce(TAPTS2_BUTTON,tapts2_button))tapts +=2;
        if (buttondebounce(TAPTS3_BUTTON))tapts +=3;*/

        /*if (isButtonPressedOnce(TBFLS_BUTTON))tbfls +=1;
        if (isButtonPressedOnce(TBTOS_BUTTON))tbtos +=1;
        if (isButtonPressedOnce(TBPTS1_BUTTON))tbpts +=1;
        if (isButtonPressedOnce(TBPTS2_BUTTON))tbpts +=2;
        if (isButtonPressedOnce(TBPTS3_BUTTON))tbpts +=3;*/
    }
    else {
        /*if (isButtonPressedOnce(TAFLS_BUTTON))tafls -=1;
        if (isButtonPressedOnce(TATOS_BUTTON))tatos -=1;
        if (isButtonPressedOnce(TAPTS1_BUTTON))tapts -=1;
        if (isButtonPressedOnce(TAPTS2_BUTTON))tapts -=2;*/
        if (buttondebounce(TAPTS3_BUTTON))tafls -=1;

        /*if (isButtonPressedOnce(TBFLS_BUTTON))tbfls -=1;
        if (isButtonPressedOnce(TBTOS_BUTTON))tbtos -=1;
        if (isButtonPressedOnce(TBPTS1_BUTTON))tbpts -=1;
        if (isButtonPressedOnce(TBPTS2_BUTTON))tbpts -=2;
        if (isButtonPressedOnce(TBPTS3_BUTTON))tbpts -=3;*/
    }
}

unsigned long lastSendTime = 0; // For controlling the blink timing
unsigned long currentTime = 0; // For controlling the blink timing

void senddata(){
  char gamedata[100];
  sprintf(gamedata, "[GAME DATA] %s %d %03d %d %d %03d %d %d", gameclk.getCurrentGameTime(), gameper, tapts, tafls, tatos, tbpts, tbfls, tbtos);
  currentTime = millis();
  if (currentTime - lastSendTime >= 1000) {
          lastSendTime = currentTime;
          Serial.println(gamedata);
  }
}

#endif // STATEMACHINE_H
