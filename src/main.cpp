#include <iostream>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


int main()
{
    glfwInit();

    //uint32_t extensionCount = 0;
    //vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    //std::cout << extensionCount << " extensions supported" << std::endl;

    glm::vec3 testVec(0,1,1);
    std::cout << testVec.x << testVec.y << testVec.z << std::endl;

    return 0;
}