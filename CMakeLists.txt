# CMakeList.txt : CMake project for simple_engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)
project ("simple_engine")

# Set C++ version.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug if not set.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (WIN32)
	file(GLOB OPENGL_SOURCES "src/graphics/opengl/graphics_opengl.cpp" "src/graphics/opengl/graphics_opengl_win32.cpp")
	file(GLOB PLATFORM_SOURCES  "src/platform/win32/*.cpp")
elseif (UNIX AND NOT APPLE)
	file(GLOB OPENGL_SOURCES "src/graphics/opengl/graphics_opengl.cpp" "src/graphics/opengl/graphics_opengl_linux.cpp")
	file(GLOB PLATFORM_SOURCES  "src/platform/linux/*.cpp")
else()
	message(FATAL_ERROR "Platform not supported")
endif()

file(GLOB UTILITIES_SOURCES "src/utilities/*.cpp")

set(SOURCES ${PLATFORM_SOURCES} ${OPENGL_SOURCES} ${UTILITIES_SOURCES} "src/main.cpp")

# Add source to this project's executable.
add_executable (simple_engine ${SOURCES})

# Define header search path.
target_include_directories(simple_engine PRIVATE
	"include"
	"include/common"
	"thirdparty/opengl"
)


if (WIN32)
	target_link_libraries(simple_engine PRIVATE
		"user32"
		"opengl32"
	)
elseif (UNIX AND NOT APPLE)
	find_package(X11 REQUIRED)
	find_package(OpenGL REQUIRED)
#	find_package(EGL REQUIRED)

	target_include_directories(simple_engine PRIVATE
		${X11_INCLUDE_DIR}
	)
	target_link_libraries(simple_engine PRIVATE
		${X11_LIBRARIES}
		${OPENGL_LIBRARIES}
		EGL
	)
endif()

# Set to C++20 if possible.
if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET simple_engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
