# Define an executable
add_executable(app
        src/main2.cpp)

add_executable(main src/main.cpp)

# Define the libraries this project depends upon
target_link_libraries(app
        lib
        Boost::serialization)

target_link_libraries(main
        lib
        Boost::serialization)