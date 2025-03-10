#include <stdio.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>

GLFWwindow *window;
VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
VkDevice device;

VkQueue graphicsQueue; // Logical device queue (added in createLogicalDevice).

const char *validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
const int WIDTH = 800;
const int HEIGHT = 600;

// We want to always check if a device is suitable for Vulkan
int isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    printf("Found GPU: %s\n", deviceProperties.deviceName);

    // Minimal: check if it’s a discrete GPU and if it has a graphics queue
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    int hasGraphicsQueue = 0;
    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            hasGraphicsQueue = 1;
            break;
        }
    }

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && hasGraphicsQueue;
}

// We want to pick a vulkan supported/compatible GPU if it exists
void pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    if (deviceCount == 0) {
        printf("No GPUs with Vulkan support found!\n");
        exit(1);
    }

    VkPhysicalDevice devices[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    for (uint32_t i = 0; i < deviceCount; i++) {
        if (isDeviceSuitable(devices[i])) {
            physicalDevice = devices[i];
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        printf("No suitable GPU found!\n");
        exit(1);
    }

    printf("Physical device selected successfully!\n");
}

// The logical device is basically what lets vulkan control the selected GPU.
void createLogicalDevice() {
    // Find graphics queue family index
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    int graphicsFamilyIndex = -1;
    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamilyIndex = i;
            break;
        }
    }

    if (graphicsFamilyIndex == -1) {
        printf("Failed to find a graphics queue family!\n");
        exit(1);
    }

    // Vulkan requires you to specify the queues to be created along with the logical device
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;

    // This is where we pass the validation layers (optional but good for debugging)
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = validationLayers;

    // Actually create the logical device
    if (vkCreateDevice(physicalDevice, &createInfo, NULL, &device) != VK_SUCCESS) {
        printf("Failed to create logical device\n");
        exit(1);
    }

    // Retrieve the graphics queue handle from the logical device
    vkGetDeviceQueue(device, graphicsFamilyIndex, 0, &graphicsQueue);

    printf("Logical device created successfully!\n");
}

// Before we create our Vulkan instance, we need to check if the validation layers are supported.
int checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    for (int i = 0; i < 1; i++) { // We only have one layer in validationLayers[]
        int layerFound = 0;
        for (uint32_t j = 0; j < layerCount; j++) {
            // strcmp returns 0 when the strings match
            if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
                layerFound = 1;
                break;
            }
        }

        if (!layerFound) {
            return 0;
        }
    }

    return 1;
}

// GLFW is the library that creates our window and abstracts OS-specific windowing operations.
void createWindow() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        exit(1);
    }

    // We tell GLFW to not create an OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Validation Layer", NULL, NULL);

    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        exit(1);
    }
}

// The window surface is how Vulkan draws images to the screen
void createSurface() {
    if (glfwCreateWindowSurface(instance, window, NULL, &surface) != VK_SUCCESS) {
        printf("Failed to create window surface\n");
        exit(1);
    }

    printf("Window surface created successfully!\n");
}

// This function initializes Vulkan and sets up the validation layers and extensions GLFW needs
void createInstance() {
    if (!checkValidationLayerSupport()) {
        printf("Validation layers requested, but not available!\n");
        exit(1);
    }

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // Enable validation layers for the instance
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = validationLayers;

    // Actually create the Vulkan instance
    if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS) {
        printf("Failed to create Vulkan instance\n");
        exit(1);
    }

    printf("Vulkan instance created successfully!\n");
}

int main() {
    createWindow();

    // Before we can do anything with Vulkan, we need to create an instance
    createInstance();

    // We need a surface that Vulkan can draw to (the window)
    createSurface();

    // When our engine is ramping up, this will select the GPU for us
    pickPhysicalDevice();

    // This will create an interface between Vulkan and the GPU
    createLogicalDevice();

    // Main loop - runs until the window is closed
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    // Cleanup - destroy everything in reverse order
    vkDestroyDevice(device, NULL);
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyInstance(instance, NULL);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
