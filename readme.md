# Rick's Sanchez Portal Gun Project

### *Wubba Lubba Dub-Dub*

Congratulations you have come across one of the many horribly written scripts for arduino that allow you to build an imitation of the portal gun from the Rick & Morty animated series.

## How is it done?

3d printing, a lot of Patience, some electronics and some lines of code that make the arduino do what it does.


## Materials needed to build:

* Arduino Uno (or clone) *You can use a different model, except that you will probably have to edit the awful code.*
* 4-Digit 7-segment Display (I have used 5462AS-1)
* X / Y Joystick
* 6 Resistors; Match them to your components.
* 4 green LEDs 
* Basic manual or 3d printing skills, arduino programming. 

## How does it work?

* When turned on:
  * the LED showing the amount of liquid in the tank is turned on. It will always be full, because the LED will always be lit at full power in its color.
  * "----" will appear on the screen. 
* Moving joystick to the right or left selects the dimension you want to go.
  * When dimension is selected you can press on the joystick to activate protal gun. 
  * When the portal gun is activated, in other words when we fire:
    *  it lights up the 3 LEDs on the front of the device imitating the portal firing. They remain on for 5 seconds. 
    *  The screen changes to "----"
    *  Previous and Current location we are in is stored in device memory
* Moving joystick up shows current universe we are in
* Moving joystick down shows previous universe we were in
* Device has an option to self-destruct; To do so, we must first unlock the gun portal.
  
   We do this with a simple combination. Just lift the joystick up 10 times, and then select the 3rd world (D-99). The text on the screen will change to "UNLC." All you have to do now is press fire. 
   
   P.S The second time you move the joystick up, the device will tell us "LCK" on the screen, suggesting that the device is locked.


* Self-destruction has its own screen animation after which the tank LED will be turned off, and the device itself will turn off for 15 seconds.



## Will the project be updated someday?

Maybe, but I'm not promising anything. Maybe someday there will be more information about Rick's portal gun, and I'll feel like updating my version.

## The wall of glory:

* ~~Me~~
* [The creator of this interactive animation editor for seven-segment displays.](https://jasonacox.github.io/TM1637TinyDisplay/examples/7-segment-animator.html)
* Creators / Writers of Rick & Morthy series 
* [Creators / Authors of the series fandom page](https://rickandmorty.fandom.com/wiki/Rickipedia)
* [Creators of this project I was inspired by.](https://www.thingiverse.com/thing:2935246)

###### P.S; I know that it deviates from the orginal.