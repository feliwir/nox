# GTK3
find_path(GTK_INCLUDE_DIRS
    gtkmm.h
)

# Libraries
# GTK
find_library(GTKMM_LIBRARY gtkmm REQUIRED)
list(APPEND GTK_LIBRARIES ${GTKMM_LIBRARY})
find_library(GTK_LIBRARY gtk-3.0 REQUIRED)
list(APPEND GTK_LIBRARIES ${GTK_LIBRARY})
# GIO
find_library(GIOMM_LIBRARY giomm REQUIRED)
list(APPEND GTK_LIBRARIES ${GIOMM_LIBRARY})
find_library(GIO_LIBRARY gio-2.0 REQUIRED)
list(APPEND GTK_LIBRARIES ${GIO_LIBRARY})
# GLIB
find_library(GLIBMM_LIBRARY glibmm REQUIRED)
list(APPEND GTK_LIBRARIES ${GLIBMM_LIBRARY})
find_library(GLIB_LIBRARY glib-2.0 REQUIRED)
list(APPEND GTK_LIBRARIES ${GLIB_LIBRARY})
# ATK
find_library(ATKMM_LIBRARY atkmm REQUIRED)
list(APPEND GTK_LIBRARIES ${ATKMM_LIBRARY})
find_library(ATK_LIBRARY atk-1.0 REQUIRED)
list(APPEND GTK_LIBRARIES ${ATK_LIBRARY})
# GOBJECT
find_library(GOBJECT_LIBRARY gobject-2.0 REQUIRED)
list(APPEND GTK_LIBRARIES ${GOBJECT_LIBRARY})
# SIGC
find_library(SIGC_LIBRARY sigc-2.0 REQUIRED)
list(APPEND GTK_LIBRARIES ${SIGC_LIBRARY})



include(GenerateExportHeader)
add_library(nox-gui SHARED 
    MainWindow.cpp MainWindow.hpp
    PreviewArea.cpp PreviewArea.hpp
    Timeline.cpp Timeline.hpp)
generate_export_header(nox-gui)

target_include_directories(nox-gui PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GTK_INCLUDE_DIRS})

target_link_libraries(nox-gui
    nox-core
    ${GTK_LIBRARIES})