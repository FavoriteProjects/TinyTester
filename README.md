# TinyTester
An attiny85 based Li-ion Battery tester/monitor. At this time, it measures the capacity of it by discharging into a value-known resistor.
## Tester Types:
1: Datalog - Logs the ADC value of battery voltage (0-255) on EEPROM (size : 512 bytes) and shows informations on an OLED display
2: Datalog without Display - Only Logs the ADC values on EEPROM.
