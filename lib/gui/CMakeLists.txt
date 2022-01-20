cmake_minimum_required(VERSION 3.9)

project(GUI DESCRIPTION "GUI components for embedded-crypto-ticker")

SET(ASSETS
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/btc_icon_60.c
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/views/ticker/legacy.cpp
)

if(ESP_PLATFORM)
else()
    file(GLOB_RECURSE LVGL_SOURCES lvgl/src/*.c)

    add_library(gui SHARED
        ${LVGL_SOURCES}
        ${ASSETS}
        ${SOURCES}
    )

    target_include_directories(gui PUBLIC 
        . 
        "lvgl/src"
        "lvgl"
    )
endif()