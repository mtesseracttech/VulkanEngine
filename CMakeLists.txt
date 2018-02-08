cmake_minimum_required(VERSION 3.8)
project(VulkanEngine)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        src/Utility/Logger.cpp
        src/Utility/Logger.hpp
        src/Utility/ShaderUtils.hpp
        src/Utility/Timer.hpp
        src/Utility/Constants.hpp
        src/Utility/Constants.cpp
        src/Core/Camera.cpp
        src/Core/Camera.hpp
        src/Core/GameObject.cpp
        src/Core/GameObject.hpp
        src/Core/GameBase.cpp
        src/Core/GameBase.hpp
        src/Core/MtVulkanAPI/VulkanRendererBase.cpp
        src/Core/MtVulkanAPI/VulkanRendererBase.hpp
        src/Core/MtVulkanAPI/WrappedVulkanDevice.hpp
        src/Core/MtVulkanAPI/WrappedVulkanBuffer.hpp
        src/Core/MtVulkanAPI/WrappedVulkanValidation.cpp
        src/Core/MtVulkanAPI/WrappedVulkanValidation.hpp
        src/Core/MtVulkanAPI/VulkanHelpers.cpp
        src/Core/MtVulkanAPI/VulkanHelpers.hpp
        src/Core/MtVulkanAPI/WrappedVulkanSwapchain.hpp
        src/Core/MtVulkanAPI/WrappedVulkanWindow.hpp
        src/Core/MtVulkanAPI/VulkanModel.hpp
        src/Core/MtVulkanAPI/VulkanTexture.hpp
	    src/Core/MtVulkanAPI/VulkanShaderProgram.hpp
        src/Core/Renderers/SimpleRenderer.cpp
        src/Core/Renderers/SimpleRenderer.hpp
        src/Material/AbstractMaterial.hpp
        src/Material/SkyboxMaterial.hpp
        src/Utility/GameTimer.hpp
        src/Core/MtVulkanAPI/VulkanRenderable.cpp
        src/Core/MtVulkanAPI/VulkanRenderable.hpp
        src/Core/Renderers/WorldRenderer.cpp
        src/Core/Renderers/WorldRenderer.hpp
        src/Behaviours/Behaviour.hpp
        src/Core/GameWorld.cpp
        src/Core/GameWorld.hpp
        src/Utility/ConsoleGlm.hpp src/Core/Renderers/PbrRenderer.cpp src/Core/Renderers/PbrRenderer.hpp src/Material/SimplePbrMaterial.hpp src/Core/VertexLayouts/StandardLayout.hpp src/Core/MtVulkanAPI/VertexInputDescriptor.hpp src/Utility/KeyInput.hpp src/Utility/KeyInput.cpp src/Utility/MouseInput.cpp src/Utility/MouseInput.hpp src/Utility/GameTimer.cpp src/Core/MtVulkanAPI/WrappedVulkanWindow.cpp src/Core/Renderers/PbrTexRenderer.cpp src/Core/Renderers/PbrTexRenderer.hpp src/Core/GraphicsComponents/ReflectionProbe.cpp src/Core/GraphicsComponents/ReflectionProbe.hpp src/Core/GraphicsComponents/LightSource.cpp src/Core/GraphicsComponents/LightSource.hpp)

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
    #message(STATUS ${ASSIMP_INCLUDE_DIR})
endif (ASSIMP_FOUND)
