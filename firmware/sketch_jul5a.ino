/*
  Firmware is still a work in progress, will refine further once parts are shipped
*/

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

int appType = 0;
long presetAlarm = 0;
long elapsedTime = 0;
int timeBuffer[2];
bool isPM = false;

#define TFT_SCL TX
#define TFT_SDA RX
#define TFT_RST 10
#define TFT_DC 8
#define TFT_BL 7
#define PIEZO 6
#define FRONTLIGHT VBUS

// i'm going to adjust these if needed because I DON"T HAVE THE BOARD RN
#define KEY_RED 0
#define KEY_BLU 1
#define KEY_GRN 2
#define KEY_YLW 3
#define KEY_VODKA 4 // the settings button
#define KEY_GIMLET 5 // the multifunction button

Adafruit_ST7789 screen = Adafruit_ST7789(TFT_SCL, TFT_SDA, TFT_RST, TFT_DC, TFT_BL);

class SimonSquare {
  private:
    int x;
    int y;
    int length = 20;
    uint8_t color;
  public:
    SimonSquare(int x, int y, uint8_t color) {
      this->x = x;
      this->y = y;
      this->color = color;
      init();
    }

    void init() {
      screen.setCursor(x, y);
      screen.drawRect(x, y, length, length, color);
    }

    void fill() {
      screen.setCursor(x, y);
      screen.fillRect(x, y, length, length, color);
    }

    void unfill() {
      screen.fillRect(x, y, length, length, ST77XX_BLACK);
    }

    void flash() {
      fill();
      delay(333);
      unfill();
    }
};

void resetScreen() {
  screen.fillScreen(ST77XX_BLACK); // make sure there is nothing in the buffer
  screen.setCursor(0,0); // make the cursor at the top left
}

void setup() {
  Serial.begin(115200);

  screen.init(284, 76); // screen dimensions
  // screen.setColRowStart(82, 18); // Offsets that don't f#cking work
  screen.setRotation(2); // this makes the screen landscape, pray and/or try 0-3
  Serial.println("Screen Initialized!");

  resetScreen();
  screen.setTextSize(2);
  screen.print("Tanino Gimlet"); // she uma on my sumume if this works
}

void loop() {
  // put your main code here, to run repeatedly:
  // the main UI code, will need switch case to tell which app to repeatedly run
  int taninoGimlet = digitalRead(KEY_GIMLET);
  int vodka = digitalRead(KEY_VODKA);

  if (taninoGimlet == LOW) {
    // settings UI
    setAlarm();
  } else if (vodka == LOW) {
    // weather UI?
  } else {
    mainTime();
  }
  delay(50);
}

// road work ahead? man i sure HOPE it does
void displayProperTime(long timer) {
  int hours;
  int minutes;

  minutes = timer / 60;
  hours = minutes / 60;
  minutes = minutes - (hours * 60);
  if (hours >= 12) {
    isPM = true; // pretty scheiße but fix later - do we really need this?
  }
  if (hours == 0) {
    hours = 12;
  } else if (hours > 12) {
    hours -= 12;
  }

  timeBuffer[0] = hours;
  timeBuffer[1] = minutes;
}

void mainTime() {
  // todo: figure out how to concatenate on screen using .print stitching
  char vodka = 'A';
  displayProperTime(elapsedTime);

  if (isPM) {
    vodka = 'P';
  }
  resetScreen();

  screen.print(timeBuffer[0]);
  screen.print(":");
  if (timeBuffer[1] < 10) {
    screen.print("0");
  }
  screen.print(timeBuffer[1]);
  screen.print(vodka);
  screen.print("M");

  elapsedTime += 1;
  // is this separate from the loop delay or are they cumulative?
  delay(950);
}

void setAlarm() {
  int move = digitalRead(KEY_YLW);
  int save = digitalRead(KEY_RED);
  int up = digitalRead(KEY_BLU);
  int down = digitalRead(KEY_GRN);
  int hours = presetAlarm / 3600;
  int minutes = (presetAlarm - (hours * 3600)) / 60;
  bool selection = false;

  resetScreen();

  if (move == LOW) {
    Serial.print("nope");
  } else if (save == LOW) {
    presetAlarm = (hours * 3600) + (minutes * 60);
  } else if (up == LOW) {
    Serial.print("nope");
  } else if (down == LOW) {
    Serial.print("nope");
  }
  // 
}

void weather() {
  // weather UI? Or just put it in the home screen?
}

void simon() {
  // Gimlet tries not to explode

  /*
     I might utilize the same methods as the KTaNE manual, but use Uma chr last names instead of random serial numbers
     1 - red
     2 - blue
     3 - green
     4 - yellow
  */
  resetScreen();
  

  // screen.fillRect(Cursorx, Cursory, length, length, color);
}


