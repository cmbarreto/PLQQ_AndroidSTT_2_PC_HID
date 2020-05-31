# PLQQ_AndroidSTT_2_PC_HID

The goal is to dictate text and commands from an Android device to a PC with a HID interface, any operating system.

For this they use:

An Android application written as a DroidScript http://droidscript.org/ script

An Arduino with HID like leonardo, pro micro, feather etc. https://learn.adafruit.com/adafruit-feather-32u4-bluefruit-le

A ￼JY-MCU Bluetooth device connected to the Arduino for serial communication.

The application on the arduino that processes the words received from the Android.

