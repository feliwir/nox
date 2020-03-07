add_executable(nox main.cpp)

target_link_libraries(nox PRIVATE nox-gui)

find_program(GLib_COMPILE_SCHEMAS glib-compile-schemas)
set(share_dir "${CMAKE_CURRENT_BINARY_DIR}/share")
set(schema_dir "${share_dir}/glib-2.0/schemas")
set(schema_output "${schema_dir}/gschemas.compiled")

add_custom_command(
    OUTPUT "${schema_output}"
    COMMAND ${GLib_COMPILE_SCHEMAS}
    "${PROJECT_SOURCE_DIR}/data/schemas"
    --targetdir ${schema_dir}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Validating ${schema}")

file(COPY ${PROJECT_SOURCE_DIR}/data/icons DESTINATION ${share_dir})

add_custom_target(compile-settings ALL
    DEPENDS "${schema_output}")

add_dependencies(nox compile-settings)