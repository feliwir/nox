project(example)

if(NOT MSVC)
find_package(PkgConfig)
pkg_check_modules(GTKMM3 gtkmm-3.0) # look into FindPkgConfig.cmake, 
                                                                # it contains documentation
# Now the variables GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES 
# contain what you expect 
else()
    # GTKMM3
    find_path(GTKMM3_INCLUDE_DIRS
        gtkmm.h
    )
    
    # Libraries
    # GTKMM
    find_library(GTKMM_LIBRARY gtkmm REQUIRED)
    list(APPEND GTKMM3_LIBRARIES ${GTKMM_LIBRARY})
    # GIOMM-
    find_library(GIOMM_LIBRARY giomm REQUIRED)
    list(APPEND GTKMM3_LIBRARIES ${GIOMM_LIBRARY})
    # GLIBMM
    find_library(GLIBMM_LIBRARY glibmm REQUIRED)
    list(APPEND GTKMM3_LIBRARIES ${GLIBMM_LIBRARY})
    # ATKMM
    find_library(ATKMM_LIBRARY atkmm REQUIRED)
    list(APPEND GTKMM3_LIBRARIES ${ATKMM_LIBRARY})
    # SIGC
    find_library(SIGC_LIBRARY sigc-2.0 REQUIRED)
    list(APPEND GTKMM3_LIBRARIES ${SIGC_LIBRARY})
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GTKMM3 DEFAULT_MSG GTKMM3_INCLUDE_DIRS GTKMM3_LIBRARIES VERSION_OK)