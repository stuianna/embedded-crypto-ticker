# cmake_minimum_required(VERSION 3.9)

# project(GUI DESCRIPTION "GUI components for embedded-crypto-ticker")

SET(ASSETS
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/btc_icon_60.c
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/eth_icon_60.c
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/ltc_icon_60.c
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/dge_icon_60.c
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/views/ticker/legacy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/views/startup/provisioning.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/views/startup/loading.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hal/driver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_lib_qrcode/lv_qrcode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_lib_qrcode/qrcodegen.c
    ${ASSETS}
)

if(ESP_PLATFORM)

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS .
                       REQUIRES main)

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