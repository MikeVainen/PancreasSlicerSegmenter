# CMake generated Testfile for 
# Source directory: C:/PFC/SlicerModules/MyExtension/LusSegmenter/Testing/Cxx
# Build directory: C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Testing/Cxx
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(LusSegmenterTest "C:/SlicerPFC/Slicer-build/Slicer.exe" "--launcher-no-splash" "--launcher-additional-settings" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/AdditionalLauncherSettings.ini" "--launch" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Testing/Cxx/Debug/LusSegmenterTest.exe" "--compare" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Data/Baseline/LusSegmenterTest.nhdr" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/Testing/Temporary/LusSegmenterTest.nhdr" "ModuleEntryPoint" "--sigma" "2.5" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Data/Input/CTHeadAxial.nhdr" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/Testing/Temporary/LusSegmenterTest.nhdr")
  set_tests_properties(LusSegmenterTest PROPERTIES  LABELS "LusSegmenter")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(LusSegmenterTest "C:/SlicerPFC/Slicer-build/Slicer.exe" "--launcher-no-splash" "--launcher-additional-settings" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/AdditionalLauncherSettings.ini" "--launch" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Testing/Cxx/Release/LusSegmenterTest.exe" "--compare" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Data/Baseline/LusSegmenterTest.nhdr" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/Testing/Temporary/LusSegmenterTest.nhdr" "ModuleEntryPoint" "--sigma" "2.5" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Data/Input/CTHeadAxial.nhdr" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/Testing/Temporary/LusSegmenterTest.nhdr")
  set_tests_properties(LusSegmenterTest PROPERTIES  LABELS "LusSegmenter")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(LusSegmenterTest "C:/SlicerPFC/Slicer-build/Slicer.exe" "--launcher-no-splash" "--launcher-additional-settings" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/AdditionalLauncherSettings.ini" "--launch" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Testing/Cxx/MinSizeRel/LusSegmenterTest.exe" "--compare" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Data/Baseline/LusSegmenterTest.nhdr" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/Testing/Temporary/LusSegmenterTest.nhdr" "ModuleEntryPoint" "--sigma" "2.5" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Data/Input/CTHeadAxial.nhdr" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/Testing/Temporary/LusSegmenterTest.nhdr")
  set_tests_properties(LusSegmenterTest PROPERTIES  LABELS "LusSegmenter")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(LusSegmenterTest "C:/SlicerPFC/Slicer-build/Slicer.exe" "--launcher-no-splash" "--launcher-additional-settings" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/AdditionalLauncherSettings.ini" "--launch" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Testing/Cxx/RelWithDebInfo/LusSegmenterTest.exe" "--compare" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Data/Baseline/LusSegmenterTest.nhdr" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/Testing/Temporary/LusSegmenterTest.nhdr" "ModuleEntryPoint" "--sigma" "2.5" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/LusSegmenter/Data/Input/CTHeadAxial.nhdr" "C:/PFC/SlicerModules/MyExtension/Vs2008Build/Testing/Temporary/LusSegmenterTest.nhdr")
  set_tests_properties(LusSegmenterTest PROPERTIES  LABELS "LusSegmenter")
else()
  add_test(LusSegmenterTest NOT_AVAILABLE)
endif()
