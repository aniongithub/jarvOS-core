find_package(fmt REQUIRED)

file(GLOB examples CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(example ${examples})
  if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${example})
    file(GLOB_RECURSE example_sources CONFIGURE_DEPENDS "${example}/*.cpp" "${example}/*.h")
    if (${example} MATCHES "lib.*")
      string(REPLACE "lib" "" libName ${example})
      message(STATUS "Adding example library ${libName}...")
      set(example ${libName})
      add_library(${libName} SHARED ${example_sources})
    else()
      message(STATUS "Adding example executable ${example}...")
      add_executable(${example} ${example_sources})
    endif()
    
    add_dependencies(${example} Polyverse)
    target_link_libraries(${example} 
      PUBLIC 
        Polyverse)
  endif()
endforeach()