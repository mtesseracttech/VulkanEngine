cmake_minimum_required(VERSION 3.8)
project(VulkanEngine)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} “${CMAKE_SOURCE_DIR}/cmake/Modules”)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp)
add_executable(VulkanEngine ${SOURCE_FILES})

#Finding and linking GLFW3

find_package(GLFW3 3.2 REQUIRED)
if (GLFW3_FOUND)
    include_directories(${GLFW3_INCLUDE_DIRS})
    target_link_libraries (VulkanEngine ${GLFW3_LIBRARIES})
endif (GLFW3_FOUND)

#Finding and linking Vulkan

find_package (Vulkan)
if (Vulkan_FOUND)
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries (VulkanEngine ${Vulkan_LIBRARIES})
endif (Vulkan_FOUND)