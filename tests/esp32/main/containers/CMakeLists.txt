
# Unfortunately due to the currency configuration of the crypto container, the image sources also need to be added to the tests.
add_executable(test_crypto 
  test_crypto.cpp
  ../../../../esp32/main/lib/crypto/data_table.cpp
  ../../../../esp32/main/lib/gui/assets/btc_icon_60.c
  ../../../../esp32/main/lib/gui/assets/eth_icon_60.c
  ../../../../esp32/main/lib/gui/assets/ltc_icon_60.c
  ../../../../esp32/main/lib/gui/assets/dge_icon_60.c
  )

target_link_libraries(test_crypto doctest)

target_include_directories(test_crypto PUBLIC 
  ../../../doctest
  ../../../../esp32/main/containers
  ../../../../esp32/components/lvgl
  ../../../../esp32/components
  ../../../../esp32/main
  ../../../../esp32/main/lib
  ../../../../esp32/components/crypto
  ../../../../esp32/components/database
  ../../../../x86_64
)

add_test(NAME test_crypto COMMAND test_crypto)
