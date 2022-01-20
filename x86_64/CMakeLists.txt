cmake_minimum_required(VERSION 3.0.0)
project(embedded-crypto-ticker)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

file(GLOB_RECURSE LVGL_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR}/lv_drivers/*.c)

add_executable(embedded-crypto-ticker 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/hal/driver.cpp
    ${LVGL_DRIVERS}
)

include_directories(embedded-crypto-ticker PUBLIC
    ${PROJECT_ROOT}/lib/gui/lvgl
    ${PROJECT_ROOT}/lib/gui/
    ${PROJECT_ROOT}/lib/
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_subdirectory(${PROJECT_ROOT}/lib/gui ${CMAKE_CURRENT_BINARY_DIR}/lib/gui)

target_link_libraries(embedded-crypto-ticker PUBLIC SDL2 m pthread gui)
