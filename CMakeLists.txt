#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RicochetChoozSim RicochetChoozSim.cc ${sources} ${headers})
target_link_libraries(RicochetChoozSim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# For use with the Geant4 builds provided by CMS on CVMFS
#

option(USE_CMS_CVMFS "Rely on the CMS CVMFS Geant4 set up" OFF)
if(USE_CMS_CVMFS)
    set(dummy ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cms_cvfms.success)
    add_custom_command (
        OUTPUT  ${dummy}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/replace_CMS_paths.sh ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cmake.check_cache
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/replace_CMS_paths.sh ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY} ${PROJECT_NAME}
        COMMAND touch ${dummy}
    )
    add_custom_target(
       ReplaceCMSPaths
       DEPENDS ${dummy}
       )
    add_dependencies(${PROJECT_NAME} ReplaceCMSPaths)
endif(USE_CMS_CVMFS)
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build AnaEx01. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(RicochetChoozSim_SCRIPTS
  run.mac
  runWithVis.mac
  )

foreach(_script ${RicochetChoozSim_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS RicochetChoozSim DESTINATION bin)
