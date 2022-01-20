
idf_component_register(
    SRCS 
        "main.cpp"
        "gui/hal/driver.cpp"
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/gui/assets/btc_icon_60.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/gui/views/ticker/legacy.cpp
    INCLUDE_DIRS 
        ${CMAKE_SOURCE_DIR}/components
        ${CMAKE_SOURCE_DIR}/../lib/
)
