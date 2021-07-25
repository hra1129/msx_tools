USB Gamepad bridge for MSX
-------------------------------------------------------------------------------

1. What's this?
  This is a "program that runs on a Raspberry Pi Pico to allow a USB gamepad/Mouse
to be connected to the MSX joystick port using the Raspberry Pi Pico.

  As a bonus, I've included support for SEGA MegaDrive/Genesis, but since I 
don't have a MegaDrive/Genesis, I haven't tested it yet.

2. Disclaimer
  I have actually made one and confirmed that it can control non-Joymega 
compatible games on an actual MSX (FS-A1GT) and an FPGA compatible machine 
(8bits4ever SX-2 prototype) without any problems, and that it recognizes 
the directional keys and 8 buttons using the joymega operation check tool, 
but I have only checked the following two game pads.

    BUFFALO BSGP815GY : SNES type. With continuous shooting function. It was available.
    ELECOM JC-U1608TBK: SNES type. With continuous shooting function. It was available.

  Also, I have confirmed that the following are not recognized

    SEGA MegaDrive Mini Controller: Not available.

  I am not sure if I can use other gamepads, joysticks, etc.
  Also, if you make a wiring mistake, you may damage the Rasberry Pi Pico 
or the MSX/MegaDrive itself. Please use it at your own risk.
  Please note that HRA! (t.hara) is not responsible for any problems that may arise 
from the use of this software.

3. Build
  If you want to change the placement of the buttons or change it for MegaDrive/Genesis, 
you will need to recompile it.

  Prepare the environment to build the Raspberry Pi sample program (see below).
  https://github.com/raspberrypi/pico-examples

  Add the following to CMakeLists.txt in the pico-examples file that you cloned locally.

    add_subdirectory(usb_gamepad_bridge_for_msx)

  Create a directory usb_gamepad_bridge_for_msx in the same location and copy the complete 
set (usb_gamepad_bridge_for_msx.c, etc.) to that directory.

  Enter the directory build and rebuild_cache with make or nmake.

    > nmake rebuild_cache

  After that, build as usual.

    > nmake usb_gamepad_bridge_for_msx

  When usb_gamepad_bridge_for_msx.uf2 is created in build/usb_gamepad_bridge_for_msx, 
connect the USB cable while holding down the Pico button, and copy usb_gamepad_bridge_for_msx.uf2 
to the storage that comes up. for_msx.uf2 to the storage that comes up, and the storage 
will be disconnected automatically. Now the writing is complete.

  Connect the OTG cable to the USB port on the Pico and connect the USB gamepad to it.
  The pin assignments are shown in usb_gamepad_bridge_for_msx.pdf, so you can connect it 
where you need it.

4. Customize
  The settings are written at the top of the source file usb_gamepad_bridge_for_msx.c.
  Please rewrite it as needed.

5. Attension
  The I/O voltage of the Raspberry Pi Pico is 3.3V.
  The MSX, on the other hand, is 5V.
  The MSX --> Pico connection should be made with a resistor of about 10kƒ¶ in between.
  For Pico --> MSX connection, you can use direct connection.
