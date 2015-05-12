# Install script for directory: C:/PFC/SlicerModules/MyExtension/LusSegmenter

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/MyExtension")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "RuntimeLibraries")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE EXECUTABLE FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/Debug/LusSegmenter.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE EXECUTABLE FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/Release/LusSegmenter.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE EXECUTABLE FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/MinSizeRel/LusSegmenter.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE EXECUTABLE FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/RelWithDebInfo/LusSegmenter.exe")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/Debug/LusSegmenterLib.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/Release/LusSegmenterLib.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/MinSizeRel/LusSegmenterLib.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/RelWithDebInfo/LusSegmenterLib.lib")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "RuntimeLibraries")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE SHARED_LIBRARY FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/Debug/LusSegmenterLib.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE SHARED_LIBRARY FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/Release/LusSegmenterLib.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE SHARED_LIBRARY FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/MinSizeRel/LusSegmenterLib.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./lib/Slicer-4.4/cli-modules" TYPE SHARED_LIBRARY FILES "C:/PFC/SlicerModules/MyExtension/Vs2008Build/lib/Slicer-4.4/cli-modules/RelWithDebInfo/LusSegmenterLib.dll")
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Testing/cmake_install.cmake")

endif()

