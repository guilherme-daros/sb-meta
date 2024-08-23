get_filename_component(libname ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" libname ${libname})

add_library(${libname})
target_include_directories(${libname} PUBLIC "include")
