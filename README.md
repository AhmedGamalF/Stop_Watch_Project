# "Stop Watch" using AVR ATmega32." 
## Description:-

##### -This project is based on AVR ATmega32 with frequency 1Mhz.

##### -Timer1 CTC mode configuration is used to count the Stop Watch time. (1 TIMER1_COMPA interrupt = 1 second)

##### -Six Common Anode 7-segments is connected using multiplexed technique.

##### - One 7447 decoder is used for all the six 7-segments controlling the enable/disable for each one.

##### - Configure External Interrupt INT0 with falling edge,Connect a push button with the internal pull-up resistor
##### If a falling edge is detected the Stopwatch time should be reset..

##### - Configure External Interrupt INT1 with rising edge,Connect a push button with the external pull-down resistor
##### If a rising edge is detected the Stopwatch time should be paused.

##### -Configure External Interrupt INT2 with a falling edge , Connect a push button with the internal pull-up resistor
##### If a falling edge is detected the Stopwatch time should be resumed.

