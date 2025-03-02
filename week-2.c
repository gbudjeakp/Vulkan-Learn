#include <stdio.h>
#include <vulkan/vulkan.h>

int main() {

    VkInstance instance;
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    if(vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS){
        printf("Failed to create Vulkan Instance This time\n");
        return 1;
    }

    printf("Vulkan instance was made okay\n");

    vkDestroyInstance(instance, NULL);
    return 0;
}
