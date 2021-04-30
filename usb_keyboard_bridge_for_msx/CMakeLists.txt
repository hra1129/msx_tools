add_executable( usb_keyboard_bridge_for_msx
	usb_keyboard_bridge_for_msx.c
)

target_include_directories( usb_keyboard_bridge_for_msx PRIVATE ${CMAKE_CURRENT_LIST_DIR} )
target_link_libraries( usb_keyboard_bridge_for_msx PRIVATE pico_stdlib pico_multicore tinyusb_host tinyusb_board )
pico_add_extra_outputs( usb_keyboard_bridge_for_msx )

# add url via pico_set_program_url
example_auto_set_url( usb_keyboard_bridge_for_msx )
