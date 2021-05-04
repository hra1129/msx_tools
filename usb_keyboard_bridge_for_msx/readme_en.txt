USB Keybard bridge for MSX
-------------------------------------------------------------------------------

1. What's this?
    This is a "program running on the Raspberry Pi Pico" to convert the key state 
  of a USB keyboard into MSX key matrix signals using the Raspberry Pi Pico.

2. Disclaimer
    I have done some simple tests with Pico itself, but I have not yet confirmed 
  whether it is fast enough to respond to the X signal from the Y signal specified 
  by the MSX when it is actually connected to the MSX.
    Please use it at your own risk.
    Please note that HRA! (t.hara) will not be held responsible for any problems 
  that may occur as a result of using this.

3. Build
    Prepare the environment to build the Raspberry Pi sample program (see below).
    https://github.com/raspberrypi/pico-examples

    Add the following to CMakeLists.txt in the pico-examples file that you cloned locally.

        add_subdirectory(usb_keyboard_bridge_for_msx)

    Create a directory usb_keyboard_bridge_for_msx in the same location and copy the 
  complete set (usb_keyboard_bridge_for_msx.c, etc.) to that directory.

    Enter the directory build and rebuild_cache with make or nmake.

        > nmake rebuild_cache

    Then, build as usual.

        > nmake usb_keyboard_bridge_for_msx

    When usb_keyboard_bridge_for_msx.uf2 is created in build/usb_keyboard_bridge_for_msx, 
  connect the USB cable while holding down the Pico button, and copy usb_keyboard_bridge 
  Copy _for_msx.uf2 to the storage that comes up, and the storage will automatically 
  disconnect. Now the writing is complete.

    Connect an OTG cable to the USB port on the Pico and connect a USB keyboard to it.
    The pin assignments are shown in usb_keyboard_bridge_for_msx.pdf, so you can connect 
  it where you need it.

4. Customize
    The settings are written at the top of the source file usb_keyboard_bridge_for_msx.c.
    Please rewrite it as needed.

	// --------------------------------------------------------------------
	//	MSX_KEYMATRIX_ROW_TYPE
	//		0: ROW address is specified for Y3-Y0.
	//		1: The result of decoding the ROW address is specified for Y11-Y0 (ONE HOT)
	//
	#define MSX_KEYMATRIX_ROW_TYPE 1

	//	MSX_KEYMATRIX_INV
	//		0: Y11-Y0 is positive logic.
	//		1: Y11-Y0 is negative logic.
	//
	#define MSX_KEYMATRIX_INV 1

	//	MSX_KEYMATRIX_ROW_PULL_UP
	//		0: Do not use the built-in PULL UP/DOWN on the Y0-Y11 input pins.
	//		1: Use built-in PULL UP for Y0-Y11 input pins
	//		2: Use built-in PULL DOWN for Y0-Y11 input pins
	//
	#define MSX_KEYMATRIX_ROW_PULL_UP 1

5. Attension!
	The I/O voltage of the Raspberry Pi Pico is 3.3V.
	The MSX, on the other hand, is 5V.
	Connect the MSX --> Pico with a resistor of about 10kƒ¶ in between.
	For Pico --> MSX connection, you can use direct connection.
