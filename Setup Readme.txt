Vulkan + GLFW + GLM Setup Process with CMake and Package Managers

Windows:

Preparation:
Create CLion Application Project 
Install MSYS2 (Cygwin-like package manager that includes a windows port of Arch's PacMan)
Install LunarG VK SDK
Download FindVulkan.cmake from GFLW's Github (it includes a path for MoltenVK(for OSX Later)) and put it in (proj_root/cmake/modules)

MSYS2 Commands:

$ pacman -Su //Updates pacman
$ pacman -Ss {packageName} //Is used for searching for exact package names

Installed Packages through msys2:

$ pacman -S mingw-w64-x86_64-toolchain //Installs the CLion toolchain that includes CMake, Make, GCC, etc.
$ pacman -S mingw-w64-x86_64-glfw //Installs GLFW
$ pacman -S mingw-w64-x86_64-glm //Installs GLM
$ pacman -S mingw-w64-x86_64-vulkan //Vulkan can also be installed through this method, but I went with LunarG

Enter the following in the CMakeLists.txt file in the root of the project:

######################################################################################

cmake_minimum_required(VERSION 3.8)
project(VulkanEngine)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp)
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

######################################################################################




OSX:

Preparation

Create CLion Application Project 
Install HomeBrew (package manager)
Download MoltenVK
Download FindVulkan.cmake from GFLW's Github (it includes a path for MoltenVK(for OSX Later)) and put it in (proj_root/cmake/modules)

Homebrew:
brew update //Updates homebrew
brew install glfw //Installs GLFW
brew install glm //Installs GLM

MoltenVK:
Place MoltenVK/macOS's contents into /usr/local/lib
Place MoltenVK/macOS/MoltenVK.framework/headers's contents into /usr/local/inc/MoltenVK

CMake: 
Same as on windows, but in the FindVulkan.cmake file, add at the very end of the elseif(APPLE) block the line:
set(VULKAN_INCLUDE_DIR "/usr/local/include/MoltenVK")
This is not a pretty or flexible solution, but for now it will do.