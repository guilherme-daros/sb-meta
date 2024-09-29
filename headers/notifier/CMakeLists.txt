get_filename_component(libname ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" libname ${libname})

add_library(${libname} INTERFACE)
target_include_directories(${libname} INTERFACE "include")

target_link_libraries(${libname} INTERFACE types)
