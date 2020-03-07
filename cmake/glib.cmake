find_program(GLib_COMPILE_SCHEMAS glib-compile-schemas)
if(GLib_COMPILE_SCHEMAS AND NOT GLib_FOUND)
  add_executable(glib-compile-schemas IMPORTED)
  set_property(TARGET glib-compile-schemas PROPERTY IMPORTED_LOCATION "${GLib_COMPILE_SCHEMAS}")
endif()

# glib_install_schemas(
#   [DESTINATION directory]
#   schemas…)
#
# Validate and install the listed schemas.
function(glib_install_schemas)
  set (options)
  set (oneValueArgs DESTINATION SCHEMES)
  set (multiValueArgs)
  cmake_parse_arguments(GSCHEMA "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  unset (options)
  unset (oneValueArgs)
  unset (multiValueArgs)

  foreach(schema ${GSCHEMA_UNPARSED_ARGUMENTS})
    get_filename_component(schema_name "${schema}" NAME)
    string(REGEX REPLACE "^(.+)\.gschema.xml$" "\\1" schema_name "${schema_name}")
    set(schema_output "${CMAKE_CURRENT_BINARY_DIR}/${schema_name}.gschema.xml.valid")

    add_custom_command(
      OUTPUT "${schema_output}"
      COMMAND glib-compile-schemas
        --strict
        --dry-run
        --schema-file="${schema}"
      COMMAND "${CMAKE_COMMAND}" ARGS -E touch "${schema_output}"
      DEPENDS "${schema}"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      COMMENT "Validating ${schema}")

    add_custom_target("gsettings-schema-${schema_name}" ALL
      DEPENDS "${schema_output}")
   
  endforeach()
endfunction()