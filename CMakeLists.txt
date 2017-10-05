#
# <root>/CMakeLists.txt
#

cmake_minimum_required(VERSION 2.8)

#### P r o j e c t   D e f i n i t i o n  ##################################
project(resample LANGUAGES CXX)

#### Definitions ##########################################################
# Windows requires a few extra definitions.
if (WIN32)
    add_definitions(-DNOMINMAX)     #Disable the overrides of std::min/max in Windows.h
endif (WIN32)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -std=c++17 -Wall \
    -Wno-missing-braces \
   -fdiagnostics-color=auto \
    -pthread" )
endif (UNIX)


#### T h i r d   P a r t y  ################################################

if(NOT DEFINED CRUFT_INCLUDE_DIR)
    set(CRUFT_INCLUDE_DIR)
endif()
set(CRUFT_INCLUDE_DIR ${${CRUFT_INCLUDE_DIR}} CACHE PATH "Cruft includes directory path")

if(NOT DEFINED CRUFT_LIBRARIES)
    set(CRUFT_LIBRARIES)
endif()
set(CRUFT_LIBRARIES ${${CRUFT_LIBRARIES}} CACHE FILEPATH "Cruft library file (.a,.dylib,.lib).")

################################################################################
# Sources
set(concat_HEADERS
        src/cmdline.h
        src/grid.h)

set(concat_SOURCES
        src/cmdline.cpp
        src/main.cpp)

#include_directories("${CRUFT_INCLUDE_DIR}")
#include_directories("${THIRDPARTY_DIR}/tclap/include")

################################################################################
# Target
add_executable(resample "${concat_HEADERS}" "${concat_SOURCES}")

target_link_libraries(resample ${CRUFT_LIBRARIES})

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/tclap/include"
        "${CRUFT_INCLUDE_DIR}"
)


install(TARGETS resample RUNTIME DESTINATION "bin/")

add_custom_target(install_${PROJECT_NAME}
        make install
        DEPENDS ${PROJECT_NAME}
        COMMENT "Installing ${PROJECT_NAME}")
