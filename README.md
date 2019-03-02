# USB_PPM
USB Joystick To PPM Arduino


# Download Library First 

Download 
https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip

# Connections

PPM Output is on Pin 3 

# Compatible MultiProtocol Modules

Arduino Sheild Required 
http://bit.ly/2GVD3vW

Arduino Uno
http://bit.ly/2Xzztg3


Only these Modules are compatible 
This One Recomended
http://bit.ly/2VwhvJu

http://bit.ly/2IFFQXT


R9M CPPM Mode
http://bit.ly/2NFzMkF


# YouTube Video Tutorial
https://youtu.be/4QKKIAVuhNs

# Default Mapping

Channel Mapping is AETR1234 (Can be Modified)
Configured for 8 Channels You can modify if you know what your are doing.


# Some Notes

Throttle: 
L2 button on the PS3 Controller

Yaw:
The Right joystick

Pitch and Roll:
The Left joytsitck

Useful Note:
The pitch axis is inverted when you map defualt however I have inverted it to make it work like a normal RC transmitter

Notice the last 2 numbers in the map funtion, this is how we invert it..

This is Roll Axis
 ppm[0] = map(PS3.getAnalogHat(RightHatX), 0 , 255, 1000, 2000);
 
This is Pitch Axis
 ppm[1] = map(PS3.getAnalogHat(RightHatY), 0 , 255, 2000, 1000);


Ch1 A (Roll ) ==ppm[0]

Ch2 E (Pitch ) ==ppm[1]

Ch3 T (Throttle ) ==ppm[2]

Ch4 R (Yaw ) ==ppm[3]

Ch5 AUX1 (Arm) == ppm[4]

Ch6 AUX2 (NOT SET) == ppm[5]

Ch7 AUX3 (NOT SET) == ppm[6]

Ch8 AUX4 (NOT SET) == ppm[7]
