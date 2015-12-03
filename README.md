# WearableTouchPad

This is the code and circuit diagram for our Wearable Touch Pad, a project for Intro to Physical Computing (60-223) at Carnegie Mellon University. 

The code to load onto the Light Blue Bean (or arduino uno) is in touch_pad_testing. If you want to upload to the bean, you will need to set up your Arduino IDE to do so (mac: http://legacy.punchthrough.com/bean/getting-started-osx/)(windows: http://legacy.punchthrough.com/bean/getting-started-windows/). Also, you will need the following libraries: <br>
ModernDevice's capSense library for the buttons: https://github.com/moderndevice/CapSense<br>
Adafruit's ADS1015 library (only if using the bean, otherwise, just use uno's analog pins): https://github.com/adafruit/Adafruit_ADS1X15<br>

The processing code to visualize the data coming from the sensor (sent over serial) is in touch_pad_visualization. 

The physical construction of the sensor is (from the side):<br>
_____________________________top fabric<br>
_____________________________velostat layer (connections in all 4 corners, see circuit diagram)<br>
------------------------------------------------ spacing mesh (we used a laundry bag material)<br>
_____________________________conductive fabric (grounded)<br>
_____________________________ bottom fabric

Depending on the physical properties of the sensor you make, the linearization constants in the arduino code will need to be fiddled with. 

If you wish to visualize the serial data coming from the light blue bean you will want to check out this post about getting access to the bean's virtual serial port outside of the bean loader (only on macs :'(  ): http://beantalk.punchthrough.com/t/heres-how-to-access-bean-serial-data-in-processing/751

The buttons referenced in the schematic are just pads of conductive fabric on top of the the top fabric layer. The capacitance of these pieces of fabric are then read by the microcontroller to sense touches.
