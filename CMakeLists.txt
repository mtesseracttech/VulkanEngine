cmake_minimum_required(VERSION 3.8)
project(VulkanEngine)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp src/Utility/Logger.cpp src/Utility/Logger.hpp src/Utility/ShaderUtils.hpp src/Utility/Timer.hpp src/Core/Mesh.cpp src/Core/Mesh.hpp src/Core/Model.cpp src/Core/Model.hpp src/Core/Texture.cpp src/Core/Texture.hpp src/Utility/Constants.hpp src/Utility/Constants.cpp src/Core/VulkanLayer.cpp src/Core/VulkanLayer.hpp)

add_executable(VulkanEngine ${SOURCE_FILES})

#Setting up PkgConfig

find_package(PkgConfig REQUIRED)

#Finding and linking GLFW3

pkg_search_module(GLFW3 3.2 REQUIRED glfw3)
if(GLFW3_FOUND)
    message(STATUS "Found GLFW, Including and Linking now")
    include_directories(${GLFW3_INCLUDE_DIRS})
    target_link_libraries(VulkanEngine ${GLFW3_STATIC_LIBRARIES})
endif(GLFW3_FOUND)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#Finding and linking Vulkan

find_package (Vulkan)
if (VULKAN_FOUND)
    message(STATUS "Found Vulkan, Including and Linking now")
    include_directories(${VULKAN_INCLUDE_DIR})
    target_link_libraries (VulkanEngine ${VULKAN_LIBRARY})
endif (VULKAN_FOUND)

#Finding and linking ASSIMP

find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
    message(STATUS "Found ASSIMP, Including and Linking now")
    include_directories(${ASSIMP_INCLUDE_DIR})
    target_link_libraries (VulkanEngine ${ASSIMP_LIBRARY})
endif (ASSIMP_FOUND)