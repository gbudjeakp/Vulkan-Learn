#include <stdio.h>
#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>



GLFWwindow* window;
VkInstance instance;
VkSurfaceKHR surface;

const int WIDTH = 800;
const int HEIGHT = 600;


void createWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Demo", NULL, NULL);

    if(!window){
        printf("Failed to create GLF window!\n");
        exit(1);
    }
}

void createInstance(){

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    if(vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS){
        printf("Failed to create Vulkan Instance This time\n");
        exit(1);
    }
    printf("Vulkan instance was made okay\n");
}

int main() {


    vkDestroyInstance(instance, NULL);
    return 0;
}
