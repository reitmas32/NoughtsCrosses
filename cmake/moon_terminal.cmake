#########################
#       Lib Moon        #
#########################
if(${WIN32})

elseif(${UNIX})

    #   Dir of MoonStudio
    set(MOON_STUDIO_DIR $ENV{MOON_STUDIO_DIR})

    set(MOON_TERMINAL_INCLUDE_DIR 
    ${MOON_STUDIO_DIR}
    ${MOON_STUDIO_DIR}/Moon_Terminal/include )

    option(LOGS "ON")
    if(${LOGS} STREQUAL "OFF")
        add_compile_definitions(RELEASE)
        set(MOON_TERMINAL_LIBRARIES ${MOON_STUDIO_DIR}/Moon_Terminal/build/libmoon_terminal.a ncurses)
    else()
        set(MOON_TERMINAL_LIBRARIES ${MOON_STUDIO_DIR}/Moon_Terminal/build/libmoon_terminal.a ncurses)
    endif()
endif()