file(GLOB BASE_SRC "*.c" "*.cpp")
file(GLOB BASE_HEADERS "*.h" "*.hpp")

add_executable(cpp ${BASE_SRC} ${BASE_HEADERS})

set_target_properties(cpp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

target_include_directories(cpp PRIVATE ${PROJECT_SOURCE_DIR}/mono/include/mono-2.0)
target_link_libraries(cpp ${PROJECT_SOURCE_DIR}/mono/lib/mono-2.0-sgen.lib)

add_custom_command(TARGET cpp POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/mono/mono-2.0-sgen.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:cpp>)                 # <--this is out-file path
