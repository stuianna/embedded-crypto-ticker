cmake_minimum_required(VERSION 3.9)


SET(ASSETS
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/btc_icon_60.c
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/eth_icon_60.c
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/ltc_icon_60.c
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/dge_icon_60.c
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/warning_30.c
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/black_60.c
)

set(GUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/views/ticker/legacy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/views/startup/provisioning.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/views/startup/loading.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_lib_qrcode/lv_qrcode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_lib_qrcode/qrcodegen.c
    ${ASSETS}
)

# set(ESP32_SOURCES
#     ${CMAKE_CURRENT_SOURCE_DIR}/hal/driver.cpp
# )

if(ESP_PLATFORM)
    set(GUI_SOURCES ${GUI_SOURCES} PARENT_SCOPE)
    # set(SOURCES
    #     ${ASSETS}
    #     ${GUI_SOURCES}
    #     # ${ESP32_SOURCES}
    # )
    # idf_component_register(SRCS ${SOURCES}
    #                     INCLUDE_DIRS .
    #                     REQUIRES main)
else()
    project(GUI DESCRIPTION "GUI components for embedded-crypto-ticker")

    set(SOURCES
        ${ASSETS}
        ${GUI_SOURCES}
    )

    add_library(gui SHARED
        ${LVGL_SOURCES}
        ${ASSETS}
        ${SOURCES}
    )

endif()