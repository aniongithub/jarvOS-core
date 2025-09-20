cmake_minimum_required(VERSION 3.13)

find_package(Threads REQUIRED)
find_package(fmt REQUIRED)
find_package(plog REQUIRED)

target_include_directories(${PROJECT_NAME}
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        plog
        magic_enum)

# Add all private sources here
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "pv_*.cpp" "pv_*.h" "utils/pv_*.cpp" "utils/pv_*.h")

target_sources(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> 
        ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    INTERFACE 
        ${CMAKE_THREAD_LIBS_INIT}
        fmt::fmt
        dl)