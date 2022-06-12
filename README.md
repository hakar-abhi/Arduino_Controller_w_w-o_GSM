# Arduino_Controller_w_w-o_GSM
This is a basic load controller design implemented using Arduino. Initially the load control was achieved via simple transistor logic, however flexibility and ease to code had better tradeoff over cost of Arduino.
So, we put Arduinos into practice. The control board's design was a team effort. The PCB design was made in KiCAD, an open source software. The algorithm was developed depending on practical field's scenario. Here, we present a very basic control code. 

The control is very basic, the sensors (depending on sensors you use) detect status of loads in the field and signal the control board, the control board having interface to sensor unit takes in the signal and decides what to output depending on algorithm. In this case, we dispalyed a motor controller, where the sensors are level sensors, two sensors are used to detect height of water in overhead and reserve tanks. The sensors send the status of water in the tanks, the algorithm then decides what to do and actuates the ouptput unit, the relays in this particular case.

This is an attempt to design a control board able to control variety of discrete and analog loads, also it attempts to devise algorithms accordingly.

# Demo of Conrtol Board in Action

https://user-images.githubusercontent.com/78263671/173224044-5a2003e3-b94f-4020-ac1d-87e28bd79a62.mp4

