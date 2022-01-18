cmake_minimum_required(VERSION 3.0.0)
project(embedded-crypto-ticker)

file(GLOB_RECURSE LVGL_SOURCES ${CMAKE_SOURCE_DIR}/../components/lvgl/src/*.c)
file(GLOB_RECURSE LVGL_DRIVERS ${CMAKE_SOURCE_DIR}/third_party/lv_drivers/*.c)

add_executable(embedded-crypto-ticker 
    main.cpp
    hal/lvgl/lvgl_driver.cpp
    ${LVGL_DRIVERS}
    ${LVGL_SOURCES}
)

include_directories(embedded-crypto-ticker PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../components/
    ${CMAKE_CURRENT_SOURCE_DIR}/../components/lvgl/
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/
    ${CMAKE_CURRENT_SOURCE_DIR}/hal/lvgl
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(embedded-crypto-ticker PUBLIC SDL2 m pthread)
