set(SRC_FILES
    main.cpp
)
set(exec_name a1)
add_executable(${exec_name} ${SRC_FILES})
target_link_libraries(${exec_name} PRIVATE ImGui-SFML::ImGui-SFML)

file(
    COPY ${CMAKE_SOURCE_DIR}/asset/fonts/TwilioSansMNerdFont-Regular.ttf 
    DESTINATION ${CMAKE_BINARY_DIR}/src/fonts
)

file(
    COPY ${CMAKE_SOURCE_DIR}/asset/config.txt 
    DESTINATION ${CMAKE_BINARY_DIR}/src
)
