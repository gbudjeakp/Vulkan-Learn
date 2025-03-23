#include <stdio.h>
#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

GLFWwindow *window;
VkInstance instance;
VkSurfaceKHR surface;


const int WIDTH = 800;
const int HEIGHT = 600;

void createWindow()
{

    glfwInit();
    // We have to explicitly tell GLFW that we are using Vulkan else
    // this would create an OpenGL context. 
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Demo", NULL, NULL);

    if (!window)
    {
        printf("Failed to create GLF window!\n");
        exit(1);
    }
}

void createInstance()
{

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // Before we create our vulkan instance, we need to grab all the extensions
    // that GLFW need to work. basically we create the instance with those extensions.
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS)
    {
        printf("Failed to create Vulkan Instance This time\n");
        exit(1);
    }
    printf("Vulkan instance was made okay\n");
}

// This is what connects vulkan to the window we created
void createSurface()
{
    if (glfwCreateWindowSurface(instance, window, NULL, &surface) != VK_SUCCESS)
    {
        printf("Failed to create Vulkan surface!\n");
        exit(1);
    }
}

int main()
{
    createWindow();
    createInstance();
    createSurface();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyInstance(instance, NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
