
# TODO
# These source lists are duplicates from lib/gui. However, I can't find documentation for using `add_subdirectory` in combination with esp idf.
# Surely there is a better way...
SET(GUI_ASSETS
    lib/gui/assets/btc_icon_60.c
    lib/gui/assets/eth_icon_60.c
    lib/gui/assets/ltc_icon_60.c
    lib/gui/assets/dge_icon_60.c
    lib/gui/assets/warning_30.c
    lib/gui/assets/black_60.c
)

set(GUI_SOURCES
    lib/gui/views/ticker/legacy.cpp
    lib/gui/views/startup/provisioning.cpp
    lib/gui/views/startup/loading.cpp
    lib/gui/lv_lib_qrcode/lv_qrcode.c
    lib/gui/lv_lib_qrcode/qrcodegen.c
)

idf_component_register(
    SRCS 
        "main.cpp"
        "wifi/manager.cpp"
        "tasks/currency_update.cpp"
        "containers/crypto.cpp"
        "lib/crypto/coin_gecko.cpp"
        "lib/hal/lvgl_driver.cpp"
        "lib/hal/requests.cpp"
        "lib/hal/sntp.cpp"
        ${GUI_ASSETS}
        ${GUI_SOURCES}
    INCLUDE_DIRS 
        "."
        "./lib"
)
