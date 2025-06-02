cmake_minimum_required(VERSION 3.6)

# Setup the project.
# Its name is defined to be the controller directory name.
get_filename_component(PROJECT ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT})

option(COMPMODE "Disables the UI" OFF)

find_package(SDL3 REQUIRED)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Get C or C++ sources in the current directory (only). Windows needs these cpp files compiled as well.
file(GLOB C_SOURCES *.c)
if(WIN32)
  file(GLOB CPP_SOURCES *.cpp imgui/*.cpp win_imgui/*.cpp ${WEBOTS_HOME}/src/controller/cpp/*.cpp)
else()
  file(GLOB CPP_SOURCES *.cpp imgui/*.cpp)
endif()
set(CMAKE_CXX_STANDARD 20)
set(SOURCES ${C_SOURCES} ${CPP_SOURCES})
#set(WEBOTS_HOME "/snap/webots/current/usr/share/webots")

add_compile_options(-O2)

# Setup the target executable.
add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES} ${SDL3_LIBRARIES} ${OpenCV_LIBS})

