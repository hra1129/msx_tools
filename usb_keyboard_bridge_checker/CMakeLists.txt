if (TARGET tinyusb_device)
    add_executable(usb_keyboard_bridge_checker
            usb_keyboard_bridge_checker.c
            )

    # Pull in our pico_stdlib which aggregates commonly used features
    target_link_libraries(usb_keyboard_bridge_checker pico_stdlib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(usb_keyboard_bridge_checker 1)
    pico_enable_stdio_uart(usb_keyboard_bridge_checker 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(usb_keyboard_bridge_checker)

    # add url via pico_set_program_url
    example_auto_set_url(usb_keyboard_bridge_checker)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()
