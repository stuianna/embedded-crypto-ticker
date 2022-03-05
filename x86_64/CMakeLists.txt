cmake_minimum_required(VERSION 3.0.0)
project(embedded-crypto-ticker)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

file(GLOB_RECURSE LVGL_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR}/lv_drivers/*.c)
file(GLOB_RECURSE LVGL_SOURCES ${PROJECT_ROOT}/esp32/components/lvgl/*.c)

set(COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/hal/driver.cpp
    ${LVGL_DRIVERS}
)

set(COMMON_INCLUDES
    ${PROJECT_ROOT}/esp32/components/lvgl
    ${PROJECT_ROOT}/esp32/components/gui/
    ${PROJECT_ROOT}/esp32/components
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

set(COMMON_LINK_LIBS
    SDL2 m pthread gui
)

add_executable(simulate_legacy_screen ${COMMON_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/legacy_screen.cpp)
include_directories(simulate_legacy_screen PUBLIC ${COMMON_INCLUDES})
target_link_libraries(simulate_legacy_screen PUBLIC ${COMMON_LINK_LIBS})

add_subdirectory(${PROJECT_ROOT}/esp32/components/gui ${CMAKE_CURRENT_BINARY_DIR}/gui)
