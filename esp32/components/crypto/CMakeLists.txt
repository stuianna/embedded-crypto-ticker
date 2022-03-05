set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/data_sources/coin_gecko.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/data_sources/sntp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hal/requests.cpp
)

if(ESP_PLATFORM)

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS .
                       REQUIRES main)

else()

endif()