
cmake_minimum_required(VERSION 3.4.3)

set(UNQLITE_INCLUDE ".")
set(UNQLITE_LIB ".")
link_directories(${UNQLITE_LIB})
include_directories(${UNQLITE_INCLUDE})

add_executable(unqlite_csv_import unqlite_csv_import.c)
target_link_libraries(unqlite_csv_import unqlite)

add_executable(unqlite_csv_export unqlite_csv_export.c)
target_link_libraries(unqlite_csv_export unqlite)
