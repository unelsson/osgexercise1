# set the minimum required version across the board
cmake_minimum_required(VERSION 3.10)

project(OSG_renderingexercise_1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

# Dependencies
find_package(OpenSceneGraph 3.3.4 REQUIRED osgDB osgViewer osgGA)
include_directories(SYSTEM ${OPENSCENEGRAPH_INCLUDE_DIRS})

set(USED_OSG_PLUGINS
    osgdb_jpeg
    osgdb_osg
    osgdb_png)

set(OSGPlugins_LIB_DIR "")
foreach(OSGDB_LIB ${OSGDB_LIBRARY})
	# Skip library type names
	if(EXISTS ${OSGDB_LIB} AND NOT IS_DIRECTORY ${OSGDB_LIB})
		get_filename_component(OSG_LIB_DIR ${OSGDB_LIB} DIRECTORY)
		list(APPEND OSGPlugins_LIB_DIR "${OSG_LIB_DIR}/osgPlugins-${OPENSCENEGRAPH_VERSION}")
	endif()
endforeach(OSGDB_LIB)

add_executable(main main.cpp)
add_library(mycustomcameramanipulator mycustomcameramanipulator.cpp)
target_link_libraries(mycustomcameramanipulator ${OPENSCENEGRAPH_LIBRARIES})
target_link_libraries(main ${OPENSCENEGRAPH_LIBRARIES})
target_link_libraries(main mycustomcameramanipulator)
