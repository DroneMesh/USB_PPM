/*
 * 
 * Script Was Combined and Tested By DroneMesh
 * I did not create the Library this was Found online links are below
 * I did not create the PPM function Was found online links are below
 * I merged both codes to create something usefull out of it
 * 
 * 
 * PPM Output is on Pin 3 
 * 
 * Throttle: 
 * L2 button on the PS3 Controller
 * 
 * Yaw:
 * The right joystick
 * 
 * Pitch and Roll:  
 * The left joytsitck
 * 
 * Useful Note:
 * The pitch axis is inverted when you map defualt however I have inverted it to make it work lik a normal RC transmitter
 * 
 * 
 * Notice the last 2 numbers in the funtion this is how to invert it you will not need by defaul this should work in AETR Mode
 * 
 * This is Roll Axis
 *    ppm[0] = map(PS3.getAnalogHat(RightHatX), 0 , 255, 1000, 2000);
 *    
 * This is Pitch Axis
 *    ppm[1] = map(PS3.getAnalogHat(RightHatY), 0 , 255, 2000, 1000);
 * 
 * 
 * 
 * 
 *   Download Library First 
 *   https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip
 * 
 * 
 * 
 * Ch1 A (Roll ) ==  ppm[0]
 * Ch2 E (Pitch ) ==  ppm[1]
 * Ch3 T (Throttle ) ==  ppm[2]
 * Ch4 R (Yaw ) ==  ppm[3]
 * Ch5 AUX1 (Arm) == ppm[4]
 * Ch6 AUX2 (NOT SET) == ppm[5]
 * Ch7 AUX3 (NOT SET) == ppm[6]
 * Ch8 AUX4 (NOT SET) == ppm[7]
 * 
 */




/*
 Example sketch for the PS3 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS3USB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
/* You can create the instance of the class in two ways */
PS3USB PS3(&Usb); // This will just create the instance
//PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printAngle;
uint8_t state = 0;



////////////

/*
 * PPM generator originally written by David Hasko
 * on https://code.google.com/p/generate-ppm-signal/ 
 */

//////////////////////CONFIGURATION///////////////////////////////
#define CHANNEL_NUMBER 8  //set the number of chanels
#define CHANNEL_DEFAULT_VALUE 1500  //set the default servo value
#define FRAME_LENGTH 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PULSE_LENGTH 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 3  //set PPM signal output pin on the arduino
//////////////////////////////////////////////////////////////////

#define SWITCH_PIN 16
#define CHANNEL_TO_MODIFY 11
#define SWITCH_STEP 100

byte previousSwitchValue;

/*this array holds the servo values for the ppm signal
 change theese values in your code (usually servo values move between 1000 and 2000)*/
int ppm[CHANNEL_NUMBER];

int currentChannelStep;
/////////////



void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 USB Library Started"));

///////////////////////////////
  previousSwitchValue = HIGH;
  
  //initiallize default ppm values
  for(int i=0; i<CHANNEL_NUMBER; i++){
    if (i == 2 || i == CHANNEL_TO_MODIFY) {
      ppm[i] = 1000;
    } else {
      ppm[i]= CHANNEL_DEFAULT_VALUE;
    }
  }

  pinMode(sigPin, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
  
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();

  currentChannelStep = SWITCH_STEP;
  
}

/*    Channel Info Currently Setup AS AETR you can change this by changing the PPM Value in the main loop
 * 
Ch1 A (Roll ) ==  ppm[0]
Ch2 E (Pitch ) ==  ppm[1]
Ch3 T (Throttle ) ==  ppm[2]
Ch4 R (Yaw ) ==  ppm[3]
Ch5 AUX1 (Arm) == ppm[4]
Ch6 AUX2 (NOT SET) == ppm[5]
Ch7 AUX3 (NOT SET) == ppm[6]
Ch8 AUX4 (NOT SET) == ppm[7]
 */



void loop() {
  Usb.Task();
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
     // Roll Axis is on PS3 Right Joystick X Axis
      ppm[0] = map(PS3.getAnalogHat(RightHatX), 0 , 255, 1000, 2000);
      // Pitch Axis on PS3 Right Joystick  Y Axis (Inverted to work correctly)
      ppm[1] = map(PS3.getAnalogHat(RightHatY), 0 , 255, 2000, 1000);
      // Yaw Axis is on PS3 Left Joystick X Axis
      ppm[3] = map(PS3.getAnalogHat(LeftHatX), 0 , 255, 1000, 2000);
 
    
       // IF you want Throttle on Left Joystick Enable Below and Disable the Throttle on R2
       //ppm[2] = map(PS3.getAnalogHat(LeftHatY), 0 , 255, 2000, 1000);

    
    // Analog button values can be read from almost all buttons
    if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
      if (!PS3.PS3NavigationConnected) {

        // Throttle is on R2 Button 
        ppm[2] = map(PS3.getAnalogButton(R2), 0 , 255, 800, 2000);
      }
    }

if (PS3.getButtonClick(SQUARE)){
  // AUX2 Upper  // Bound Acro Mode
  ppm[5] = 2000;
  }

if (PS3.getButtonClick(CROSS)){

  // AUX 2 Lower Bound // Angle Mode set in Betaflight
  ppm[5] = 1000;
  }


    if (PS3.getButtonClick(L1))
    {
      // L1 to Disarm if Arm is on AUX1 
      ppm[4] = 1000;
      }

      
    if (PS3.getButtonClick(R1))
    {
      // R1 is to Arm if Arm is on AUX1
    ppm[4] = 2000;}

  }
if (PS3.PS3Connected == false){

  ppm[4] = 1000;
  }
}


ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if (state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PULSE_LENGTH * 2;
    state = false;
  } else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= CHANNEL_NUMBER){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PULSE_LENGTH;// 
      OCR1A = (FRAME_LENGTH - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PULSE_LENGTH) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}

