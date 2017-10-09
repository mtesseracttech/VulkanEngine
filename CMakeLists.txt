cmake_minimum_required(VERSION 3.8)
project(VulkanEngine)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp src/Utility/Logger.cpp src/Utility/Logger.hpp src/Utility/ShaderUtils.hpp src/Utility/Timer.hpp src/Core/Mesh.cpp src/Core/Mesh.hpp src/Core/Model.cpp src/Core/Model.hpp src/Core/Texture.cpp src/Core/Texture.hpp src/Utility/Constants.hpp src/Utility/Constants.cpp src/Core/VulkanLayer.cpp src/Core/VulkanLayer.hpp src/Core/Camera.cpp src/Core/Camera.hpp src/Core/GameObject.cpp src/Core/GameObject.hpp src/Core/Transform.cpp src/Core/Transform.hpp src/Core/ShaderProgram.cpp src/Core/ShaderProgram.hpp src/Core/Material.cpp src/Core/Material.hpp)

add_executable(VulkanEngine ${SOURCE_FILES})

#Sets a base path for the includes so that they can be used in the same way as in Visual Studio/Codeblocks
set(BASEPATH "${CMAKE_SOURCE_DIR}/src")
include_directories("${BASEPATH}")

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
if (Vulkan_FOUND)
    message(STATUS "Found Vulkan, Including and Linking now")
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries (VulkanEngine ${Vulkan_LIBRARIES})
    message(STATUS "Vulkan Include dirs: ${Vulkan_INCLUDE_DIRS} and libs: ${Vulkan_LIBRARIES}")
endif (Vulkan_FOUND)

#Finding and linking ASSIMP

find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
    message(STATUS "Found ASSIMP, Including and Linking now")
    include_directories(${ASSIMP_INCLUDE_DIR})
    target_link_libraries (VulkanEngine ${ASSIMP_LIBRARY})
endif (ASSIMP_FOUND)