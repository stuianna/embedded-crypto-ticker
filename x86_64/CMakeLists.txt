cmake_minimum_required(VERSION 3.0.0)
project(embedded-crypto-ticker)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

file(GLOB_RECURSE LVGL_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR}/lv_drivers/*.c)
file(GLOB_RECURSE LVGL_SOURCES ${PROJECT_ROOT}/esp32/components/lvgl/*.c)

add_executable(embedded-crypto-ticker 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/hal/driver.cpp
    ${LVGL_DRIVERS}
)

include_directories(embedded-crypto-ticker PUBLIC
    ${PROJECT_ROOT}/esp32/components/lvgl
    ${PROJECT_ROOT}/esp32/components/gui/
    ${PROJECT_ROOT}/esp32/components
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_subdirectory(${PROJECT_ROOT}/esp32/components/gui ${CMAKE_CURRENT_BINARY_DIR}/gui)

target_link_libraries(embedded-crypto-ticker PUBLIC SDL2 m pthread gui)
