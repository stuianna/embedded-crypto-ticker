
idf_component_register(
    SRCS 
        "main.cpp"
        "wifi/manager.cpp"
        "tasks/currency_update.cpp"
        "containers/crypto.cpp"
        "lib/crypto/coin_gecko.cpp"
        "lib/hal/requests.cpp"
        "lib/hal/sntp.cpp"
    INCLUDE_DIRS 
        "."
)
