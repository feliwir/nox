project(example)

find_package(PkgConfig)

pkg_check_modules(GTKMM3 gtkmm-3.0) # look into FindPkgConfig.cmake, 
                                                                # it contains documentation
# Now the variables GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES 
# contain what you expect 


include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GTKMM3 DEFAULT_MSG GTKMM3_INCLUDE_DIRS GTKMM3_LIBRARIES VERSION_OK)