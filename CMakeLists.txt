cmake_minimum_required(VERSION 3.31)
cmake_policy(VERSION 3.31)

project(Futurewalker CXX)

# ------------------------------------------
# CTest
# ------------------------------------------
include (CTest)

# ------------------------------------------
# Directories
# ------------------------------------------
set(FW_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(FW_SOURCE_DIR ${FW_DIR}/Source)
set(FW_TEST_DIR ${FW_DIR}/Test)
set(FW_EXAMPLE_DIR ${FW_DIR}/Example)

# ------------------------------------------
# Dependencies
# ------------------------------------------

# Boost
find_package(Boost CONFIG REQUIRED)
add_library(Futurewalker-Library-boost INTERFACE)
add_library(Futurewalker::Library::boost ALIAS Futurewalker-Library-boost)
target_link_libraries(Futurewalker-Library-boost INTERFACE Boost::headers)
target_link_libraries(Futurewalker-Library-boost INTERFACE Boost::disable_autolinking)

# fmt
find_package(fmt CONFIG REQUIRED)
add_library(Futurewalker-Library-fmt INTERFACE)
add_library(Futurewalker::Library::fmt ALIAS Futurewalker-Library-fmt)
target_link_libraries(Futurewalker-Library-fmt INTERFACE fmt::fmt)

# Catch2
find_package(Catch2 CONFIG REQUIRED)

# Skia
find_package(skia CONFIG REQUIRED)
add_library(Futurewalker-Library-skia INTERFACE)
add_library(Futurewalker::Library::skia ALIAS Futurewalker-Library-skia)
target_link_libraries(Futurewalker-Library-skia INTERFACE skia::skia)

find_package(icu CONFIG REQUIRED)
add_library(Futurewalker-Library-icu INTERFACE)
add_library(Futurewalker::Library::icu ALIAS Futurewalker-Library-icu)
target_link_libraries(Futurewalker-Library-icu INTERFACE icu::icu)

# ------------------------------------------
# Flags
# ------------------------------------------

# Common compiler flags
set(FW_COMPILE_FLAGS
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /EHsc /permissive- /Zc:__cplusplus /Zc:preprocessor /external:anglebrackets /external:W0>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Werror -g -pedantic -Wshadow -fexperimental-library -fobjc-arc>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror -g -pedantic -Wshadow-compatible-local>
)

# Common linker flags
set(FW_LINK_FLAGS )

# ------------------------------------------
# Configuration
# ------------------------------------------

add_library(Futurewalker-Config INTERFACE)
add_library(Futurewalker::Config ALIAS Futurewalker-Config)
target_compile_options(Futurewalker-Config INTERFACE ${FW_COMPILE_FLAGS})
target_link_options(Futurewalker-Config INTERFACE ${FW_LINK_FLAGS})

if(WIN32)
  target_compile_definitions(Futurewalker-Config INTERFACE -DUNICODE -D_UNICODE WINVER=0x0A00 _WIN32_WINNT=0x0A00)
endif()

# ------------------------------------------
# Utility functions
# ------------------------------------------

# Glob *.hpp files from current directory.
function(fw_glob_cpp_headers dst_list)
  # Get include directory for current source directory.
  string(REPLACE "source" "include" _include_dir ${CMAKE_CURRENT_LIST_DIR})
  # Glob include files.
  file(GLOB _public_inc_files "${_include_dir}/*.hpp")
  # Set result.
  set(${dst_list} ${_public_inc_files} PARENT_SCOPE)
endfunction()

# Glob *.cpp files from current directory.
function(fw_glob_cpp_sources dst_list) 
  # Glob regex of source files.
  set(_source_files_regex
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Platform/*.cpp")
  # Add backend sources.
  # TODO: make this configurable, use CMAKE_SYSTEM_NAME.
  if (WIN32)
    set(_source_files_regex ${_source_files_regex} "${CMAKE_CURRENT_LIST_DIR}/Win/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/Win/*.manifest")
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(_source_files_regex ${_source_files_regex} "${CMAKE_CURRENT_LIST_DIR}/Mac/*.mm")
  endif()
  # Glob source files.
  file(GLOB _source_files CONFIGURE_DEPENDS ${_source_files_regex})
  # Set result
  set(${dst_list} ${_source_files} PARENT_SCOPE)
endfunction()

# Add library target.
function(fw_add_library NAME)

  # Glob files
  fw_glob_cpp_sources(_source_files)
  fw_glob_cpp_headers(_include_files)

  # Merge include files into source list
  set(_lib_sources ${_source_files} ${_include_files})

  # Add library target
  set(_lib_name "${NAME}")
  add_library(${_lib_name} ${_lib_sources})

  # Add include directory.
  target_include_directories(${_lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(${_lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Platform)
  target_include_directories(${_lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Win)
  target_include_directories(${_lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Mac)

  # Convert library name to alias
  string(REPLACE "-" "::" _alias_lib_name ${_lib_name}) 
  add_library(${_alias_lib_name} ALIAS ${_lib_name})

  # Set target folder
  string(REPLACE "-" "/" _lib_folder ${_lib_name})
  set_target_properties(${_lib_name} PROPERTIES FOLDER ${_lib_folder})

  # Set C++ standard version
  set_target_properties(${_lib_name} PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})

endfunction()

# Add interface library target.
function(fw_add_interface_library NAME)

  fw_glob_cpp_headers(_include_files)

  # add library target
  set(_lib_name "${NAME}")
  add_library(${_lib_name} INTERFACE ${_include_files})

  target_include_directories(${_lib_name} INTERFACE ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(${_lib_name} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/Platform)

  # convert library name to alias
  string(REPLACE "-" "::" _alias_lib_name ${_lib_name}) 
  add_library(${_alias_lib_name} ALIAS ${_lib_name})

  # Set target folder
  string(REPLACE "-" "/" _lib_folder ${_lib_name})
  set_target_properties(${_lib_name} PROPERTIES FOLDER ${_lib_folder})

endfunction()

# Define source_group for targets in current directory.
# Since there's no target_source_group, we need to call this function after setting all sources to the target.
function(fw_source_group TARGET)

  # Get include/source dir
  set(_source_dir ${CMAKE_CURRENT_LIST_DIR})
  string(REPLACE "source" "include" _include_dir ${_source_dir})
  
  # Get list of all sources
  get_target_property(_sources ${TARGET} SOURCES)  
  cmake_path(ABSOLUTE_PATH _sources BASE_DIRECTORY ${_source_dir})
  
  # Convert relative paths to absolute paths
  foreach(_path ${_sources})
    cmake_path(ABSOLUTE_PATH _path BASE_DIRECTORY ${_source_dir})
    set(_tmp_sources ${_tmp_sources} ${_path})
  endforeach()
  set(_sources ${_tmp_sources})
  
  # Filter source files 
  set(_source_files ${_sources})
  list(FILTER _source_files INCLUDE REGEX "${_source_dir}*")
  
  # Filter include files
  set(_include_files ${_sources})
  list(FILTER _include_files INCLUDE REGEX "${_include_dir}*")
  
  # Set source group
  source_group(TREE "${_source_dir}" PREFIX "Source" FILES ${_source_files})
  source_group(TREE "${_include_dir}" PREFIX "Source" FILES ${_include_files})
  
endfunction()

# Link libraries
function(fw_link_libraries TARGET)
target_link_libraries(${TARGET} ${ARGN})
endfunction()

# Add executable target.
function(fw_add_binary NAME TYPE)

  # Glob files
  fw_glob_cpp_sources(_source_files)
  fw_glob_cpp_headers(_include_files)

  # Merge include files into source list
  set(_exe_sources ${_source_files} ${_include_files})

  # Add executable target
  set(_exe_name "${NAME}")

  if (TYPE STREQUAL "Console" )
    add_executable(${_exe_name} ${_exe_sources})
  elseif (TYPE STREQUAL "Windowed")
    add_executable(${_exe_name} WIN32 ${_exe_sources})
  else()
    message(FATAL_ERROR "Unrecognized executable type")
  endif()

  # Add include directory.
  target_include_directories(${_exe_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

  # Set target folder
  string(REPLACE "-" "/" _exe_folder ${_exe_name})
  set_target_properties(${_exe_name} PROPERTIES FOLDER ${_exe_folder})

  # Set C++ standard version
  set_target_properties(${_exe_name} PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})

endfunction()

# Add Test target.
function (fw_add_test LABEL NAME)
  set(TARGET Test-${LABEL}-${NAME})
  add_executable(${TARGET} ${NAME}.cpp)
  add_test(NAME ${TARGET} COMMAND ${TARGET})
  set_tests_properties(${TARGET} PROPERTIES LABELS ${LABEL})
  target_compile_options(${TARGET} PRIVATE ${FW_COMPILE_FLAGS})
  target_link_options(${TARGET} PRIVATE ${FW_LINK_FLAGS})
  target_link_libraries(${TARGET} PRIVATE Catch2::Catch2WithMain)
  foreach(lib ${ARGN})
    target_link_libraries(${TARGET} PRIVATE ${lib})
  endforeach()
  string(REPLACE "-" "/" _test_folder "Test-${LABEL}")
  set_target_properties(${TARGET} PROPERTIES FOLDER ${_test_folder})
  set_target_properties(${TARGET} PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})
endfunction()

# ------------------------------------------
# Subdirectories  
# ------------------------------------------

add_subdirectory(${FW_SOURCE_DIR})
add_subdirectory(${FW_EXAMPLE_DIR})

if(BUILD_TESTING)
  add_subdirectory(${FW_TEST_DIR})
endif()
