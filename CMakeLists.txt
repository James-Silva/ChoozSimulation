#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(RicochetChoozSim)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
  message(STATUS "Geant4 UI VIS packages found")
else()
  find_package(Geant4 REQUIRED)
endif()


#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})


#----------------------------------------------------------------------------
# Find ROOT (optional package)
#
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
find_package(ROOT)
if(ROOT_FOUND)
  message(STATUS "ROOT found.")
  add_definitions(-DG4ANALYSIS_USE_ROOT)
else()
  message(STATUS "ERROR: ROOT not found!!")
endif()


#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
		    ${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(RicochetChoozSim RicochetChoozSim.cc ${sources} ${headers})
target_link_libraries(RicochetChoozSim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
target_link_libraries(RicochetChoozSim "$ENV{ROOTSYS}/lib/libPhysics.so" "$ENV{ROOTSYS}/lib/libRIO.so" "$ENV{ROOTSYS}/lib/libMathCore.so" "$ENV{ROOTSYS}/lib/libTree.so" "$ENV{ROOTSYS}/lib/libCore.so" "$ENV{ROOTSYS}/lib/libMathMore.so" "$ENV{ROOTSYS}/lib/libHist.so" ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build AnaEx01. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(RicochetChoozSim_SCRIPTS
    run.mac
    vis.mac
  )

foreach(_script ${RicohetChoozSim_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()


SET(CMAKE_MODULE_PATH /home/soft/base/src/geant4.10.02.p01/cmake/Modules/)



#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS RicochetChoozSim DESTINATION bin)
