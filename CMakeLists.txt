cmake_minimum_required(VERSION 3.8)
project(VulkanEngine)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} “${CMAKE_SOURCE_DIR}/cmake/Modules”)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp)
add_executable(VulkanEngine ${SOURCE_FILES})

#Setting up PkgConfig

find_package(PkgConfig REQUIRED)

#Finding and linking GLFW3

pkg_search_module(GLFW3 3.2 REQUIRED glfw3)
if(GLFW3_FOUND)
    MESSAGE(STATUS "Found GLFW, Including and Linking now")
    include_directories(${GLFW3_INCLUDE_DIRS})
    target_link_libraries(VulkanEngine ${GLFW3_STATIC_LIBRARIES})
endif(GLFW3 _FOUND)


#Finding and linking Vulkan

find_package (Vulkan)
if (Vulkan_FOUND)
    MESSAGE(STATUS "Found Vulkan, Including and Linking now")
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries (VulkanEngine ${Vulkan_LIBRARIES})
endif (Vulkan_FOUND)