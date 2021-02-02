cmake_minimum_required(VERSION 3.14.1)

cmake_policy(SET CMP0091 NEW)
include(cmake/InitVcpkg.cmake)

project(MyNanaDemo)

# Determine the platform.
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(OS_MACOSX 1)
  set(OS_POSIX 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(OS_LINUX 1)
  set(OS_POSIX 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(OS_WINDOWS 1)
endif()

# Determine the project architecture.
if(NOT DEFINED PROJECT_ARCH)
  if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PROJECT_ARCH "x64")
  else()
    set(PROJECT_ARCH "x86")
  endif()

  if(OS_MACOSX)
    # PROJECT_ARCH should be specified on Mac OS X.
    message(WARNING "No PROJECT_ARCH value specified, using ${PROJECT_ARCH}")
  endif()
endif()

if(OS_WINDOWS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /source-charset:utf-8")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
elseif(OS_LINUX)
  MESSAGE("platform linux not supported !")
endif()

find_package(unofficial-nana REQUIRED)

file(GLOB_RECURSE _SRC_FILES "src/*.cpp")
source_group("Header Files" FILES ${_SRC_FILES})

file(GLOB_RECURSE _HEADER_FILES "src/*.h" "src/*.hpp")
source_group("Source Files" FILES ${_HEADER_FILES})

file(GLOB _RRSOURCE_FILES src/*.rc src/res/*.ico src/res/*.png)
source_group("Resource Files" FILES ${_RRSOURCE_FILES} resource.h)

add_executable(${PROJECT_NAME} WIN32
  ${_HEADER_FILES}
  ${_SRC_FILES}
  ${_RRSOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  unofficial::nana::nana
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	UNICODE
	_UNICODE
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
	MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
	COMPILE_FLAGS /EHa
)
