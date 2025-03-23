cmake_minimum_required(VERSION 3.10)
project(VulkanProject C)

# Set the Vulkan SDK path (Optional if you set VULKAN_SDK in environment variables)
set(VULKAN_SDK "C:/VulkanSDK/1.4.304.1")

# Set the GLFW path (Optional if you set GLFW_ROOT in environment variables)
set(GLFW_INCLUDE_DIR "C:/glfw-3.4.bin.WIN64/include")
set(GLFW_LIB_DIR "C:/glfw-3.4.bin.WIN64/lib-vc2022")

# Add the Vulkan SDK include and lib directories to the project
include_directories(${VULKAN_SDK}/include)
link_directories(${VULKAN_SDK}/lib)

# Add the GLFW include and lib directories to the project
include_directories(${GLFW_INCLUDE_DIR})
link_directories(${GLFW_LIB_DIR})

# Specify the source files
set(SOURCES week-4.c)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the Vulkan and GLFW libraries to your project
target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)
