cmake_minimum_required (VERSION 3.20)

project(AggCirclesHaiku)

include_directories("/boot/system/develop/headers/agg2/")

add_executable(AggCirclesHaiku
	src/Circles.cpp
	src/AGG.cpp
	src/AGGView.cpp
	src/AGGWindow.cpp
	src/AGGApplication.cpp
	src/Util.cpp
)

set_property(TARGET AggCirclesHaiku PROPERTY CXX_STANDARD 11)

target_link_libraries(AggCirclesHaiku be agg translation)

add_custom_command(TARGET AggCirclesHaiku POST_BUILD
	COMMAND rc -o Resources.rsrc ${CMAKE_SOURCE_DIR}/Resources.rdef
	COMMAND mimeset -f ${CMAKE_BINARY_DIR}/AggCirclesHaiku
	DEPENDS AggCirclesHaiku/Resources.rdef
)
