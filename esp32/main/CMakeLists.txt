
idf_component_register(
    SRCS 
        "main.cpp"
        "gui/hal/driver.cpp"
        "wifi/manager.cpp"
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/gui/assets/btc_icon_60.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/gui/views/ticker/legacy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/gui/views/startup/provisioning.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/gui/views/startup/loading.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/gui/lv_lib_qrcode/lv_qrcode.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/gui/lv_lib_qrcode/qrcodegen.c
    INCLUDE_DIRS 
        ${CMAKE_SOURCE_DIR}/components
        ${CMAKE_SOURCE_DIR}/../lib/
)
