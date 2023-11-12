set(TARGET_NAME SolarSystemSim)

add_executable(${TARGET_NAME}
    main.cpp
    Shader.cpp
    SolarSystem.cpp
    Terrestrial.cpp
    Sun.cpp
    Satellite.cpp)

target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_ROOT_DIR}/3rdparty)
target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_ROOT_DIR}/src)

target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC glad)
target_link_libraries(${TARGET_NAME} PUBLIC glm)

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE "/W4")
    target_compile_options(${TARGET_NAME} PRIVATE "/WX-")
    target_compile_options(${TARGET_NAME} PRIVATE "/O2")
else()
    target_compile_options(${TARGET_NAME} PRIVATE -O2 -Wall -Wextra)
endif()

install(TARGETS ${TARGET_NAME} glfw glad
    RUNTIME DESTINATION ./
)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJ_ROOT_DIR}/shaders 
                ${CMAKE_INSTALL_PREFIX})
