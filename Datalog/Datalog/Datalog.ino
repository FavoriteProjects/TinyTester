/*           Battery Voltage Datalog
 * Author : Guilherme Pontes
 * Date : 16/09/2017
 * Purpose: Article on www.tudosobcontrole.net
 * Description :  It logs the voltage readed from the battery 
 *                terminals on the attiny85 EEPROM, leading to 
 *                the measurement of the battery capacity, by 
 *                the knowledge of the discharge resistor value.
 *                It also displays informations on a OLED display
 * Circuit:
 * 
 *  Analog Input
    +----------+--------------------+
               | +                  |
          +----+----+               |
          |         |               |
          |         |             +-+-+
          |         |             |   |
          | Battery |             |   |
          |         |             | R |Resistor
          | 3.7V    |             |   |
          |         |             |   |
          |         |             +-+-+
          |         |               |
          |         |               |
          +----+----+               |
    Ground     |  -                 |
    +----------+--------------------+

*/
#include <DigisparkOLED.h>
#include <Wire.h>
#include <EEPROM.h>
int addr = 0;               // Address pointer to EEPROM
byte value;                 // 8 bit ADC reading 
bool  ovf = false,          // Overflow flag
      begun = false;        // Begin flag
float R = 10;               // Value of the discharge resistor (ohms)
const float k = 5.0/256.0;  // ADC Value to Voltage conversion constant
const int battery = 3;      // Analog input pin, connected to Battery+
const long Ts = 5*1000;     // Sampling Period (ms)
// Keep in mind that the EEPROM size is 512 bytes, so you can only log
// 512 values, leading to a total sample time of 512*Ts 


void draw(){    // Display drawning routine
  float voltage = (float)value*k;
  oled.setCursor(0,0);
  if(begun && !ovf){
    // First line
    oled.print("V_bat = ");
    oled.print(voltage);
    oled.println(" V");
    // Second line
    oled.print("\nADC = ");
    oled.println(value);
    // Third line
    oled.print("\nAddress = ");
    oled.println(addr+1);
    // Fourth line
    oled.print("\nEEPROM = ");
    oled.print(addr*100.0/512.0);
    oled.println("%");
  }else if(!begun)
    oled.println("-Plug-the-Battery-");
  else
    oled.println("----OVERFLOW----");
}
void setup() {
  pinMode(battery,INPUT);   //
  oled.begin();
  oled.clear(); //all black
  oled.setFont(FONT6X8);
  delay(1000);
}
void loop() {
  oled.clear();
  //Reading
  value = analogRead(battery)/4;
  if(value>150) // 204 is the ADC value for about 4.0V
    begun = true;
  if(begun){
    //Storing
    if(addr==EEPROM.length())
      ovf = true;
    else
      EEPROM[addr] = value;
    //Displaying
    draw();
    addr++;
    delay(Ts);
  }
}
