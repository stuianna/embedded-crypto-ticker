add_executable(test_volatile_database 
  test_volatile_database.cpp
  )

target_link_libraries(test_volatile_database doctest)

target_include_directories(test_volatile_database PUBLIC 
  "../../../../../esp32/main/lib/tools/"
)

add_test(NAME test_volatile_database COMMAND test_volatile_database)