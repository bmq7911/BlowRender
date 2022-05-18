#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "win/VulkanEnd.h"
#include <exception>

namespace VK{
    #define MAKE_VERSION( x,y,z) ((( 0xFF & x) << 24 ) + ((0xFF &y)<<16 ) + 0xFFFF & z )


    VulkanShader::VulkanShader( )
        : m_shaderModule( VK_NULL_HANDLE)
    {

    }
    VkShaderModule VulkanShader::getShaderModule() const{
        return m_shaderModule;
    }
    VkShaderStageFlagBits VulkanShader::getShaderStageFlagBits() const{
        return m_shaderStage;
    }
    const char * VulkanShader::getEntryName() const{
        return m_entryName.c_str();
    }

    static VulkanShader createVulkanShader( VkDevice device,
                                            VkShaderStageFlagBits stage,
                                            uint32_t *pSpivCode, size_t size,
                                             const char * entry){
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.codeSize = size;
        createInfo.pCode = pSpivCode;

        VkShaderModule module;
        VkResult result = vkCreateShaderModule( device, &createInfo, nullptr,&module  );
        if( VkResult::VK_SUCCESS != result ){

        }
    }


    VulkanEnd::VulkanEnd()
        : m_instance( VK_NULL_HANDLE)
        , m_physicalDevice( VK_NULL_HANDLE )
        , m_device( VK_NULL_HANDLE)
        , m_queueIndex( 0 )
        , m_commandPool( VK_NULL_HANDLE)
    {
        vk::DispatchLoaderStatic dispatch;
        
        if( GLFW_FALSE == glfwInit()){
            throw MessageException("init glfw failed");
        }
        if( GLFW_FALSE == glfwVulkanSupported()){
            throw MessageException("glfw not support vulkan" );
        }
        //PFN_vkCreateInstance vkCreateInstance = (PFN_vkCreateInstance)glfwGetInstanceProcAddress(nullptr, "vkCreateInstance");
        
        
        VkApplicationInfo applicationInfo = {};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext = nullptr;
        applicationInfo.pApplicationName ="TestVulkan";
        applicationInfo.applicationVersion = MAKE_VERSION(0,0,1);
        applicationInfo.pEngineName   = "TestVulkan";
        applicationInfo.engineVersion = MAKE_VERSION(0,0,1);
        applicationInfo.apiVersion    = VK_API_VERSION_1_0;
        VkInstanceCreateInfo  instanceCreateInfo ={};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount   = 0;
        instanceCreateInfo.ppEnabledLayerNames = nullptr;
        instanceCreateInfo.enabledLayerCount   =0;
        instanceCreateInfo.ppEnabledExtensionNames = nullptr;

        VkInstance instance = VK_NULL_HANDLE;
        VkResult result = dispatch.vkCreateInstance( &instanceCreateInfo, nullptr, &instance);

        if( VkResult::VK_SUCCESS != result ){
            throw MessageException("Create Vulkan Instance Error");
        }
        else{
            m_instance = instance;
        }
        uint32_t deviceCount = 0;
        if( VkResult::VK_SUCCESS != dispatch.vkEnumeratePhysicalDevices( instance,&deviceCount ,nullptr )){
            dispatch.vkDestroyInstance( m_instance, nullptr);
            m_instance = VK_NULL_HANDLE;
            throw MessageException("Enumerate Physical Device Error");            
        }
        std::vector<VkPhysicalDevice> physicalDevices(deviceCount,VK_NULL_HANDLE);
        if( VkResult::VK_SUCCESS != dispatch.vkEnumeratePhysicalDevices( instance, &deviceCount, physicalDevices.data() )){
            dispatch.vkDestroyInstance( m_instance, nullptr);
            m_instance = VK_NULL_HANDLE;
            throw MessageException("Enumerate Physical Device Error");            
        }
        auto theBestPhysicalDevice = _SelectBestPhysicalDevice( physicalDevices);
        m_physicalDevice = theBestPhysicalDevice;
        uint32_t queueFamilyCount = 0;
        dispatch.vkGetPhysicalDeviceQueueFamilyProperties( theBestPhysicalDevice,&queueFamilyCount,nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties( queueFamilyCount);
        dispatch.vkGetPhysicalDeviceQueueFamilyProperties( theBestPhysicalDevice,&queueFamilyCount,queueFamilyProperties.data());
        uint32_t queueIndex = 0;
        for( size_t i =0; i < queueFamilyProperties.size(); ++i ){
            if( _IsSupportCompute( queueFamilyProperties[i]) ){
                queueIndex = static_cast<uint32_t>( i );
                break;
            }
        }


        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pNext = nullptr;
        queueCreateInfo.flags = 0;
        queueCreateInfo.queueFamilyIndex = queueIndex;
        queueCreateInfo.queueCount = 1;
        float queuePriorities = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriorities;

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
        deviceCreateInfo.enabledLayerCount = 0;

        VkDevice device = VK_NULL_HANDLE;
        result = dispatch.vkCreateDevice( theBestPhysicalDevice, &deviceCreateInfo, nullptr, & device );
        if( VkResult::VK_SUCCESS != result ){
            dispatch.vkDestroyInstance( m_instance, nullptr);
            m_instance = VK_NULL_HANDLE;
            throw MessageException("Create The Logic Device Error");     
        }
        m_queueIndex = queueIndex;
        m_device = device;
        VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
        cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cmdPoolCreateInfo.pNext = nullptr;
        cmdPoolCreateInfo.flags = 0;
        cmdPoolCreateInfo.queueFamilyIndex = queueIndex;
        result = dispatch.vkCreateCommandPool( m_device,&cmdPoolCreateInfo,nullptr, &m_commandPool );
        if( VkResult::VK_SUCCESS != result ){
            dispatch.vkDestroyDevice( m_device, nullptr);
            dispatch.vkDestroyInstance( m_instance, nullptr);
            throw MessageException("Create Command Pool Error");
        }
        VkCommandBufferAllocateInfo allocateInfo ={};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.pNext = nullptr;
        allocateInfo.commandPool = m_commandPool;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = 1;

        
        result = dispatch.vkAllocateCommandBuffers( m_device, &allocateInfo, &m_commandBuffer);
        if( VkResult::VK_SUCCESS != result ){
            dispatch.vkDestroyCommandPool( m_device, m_commandPool, nullptr );
            dispatch.vkDestroyDevice( m_device, nullptr);
            dispatch.vkDestroyInstance( m_instance, nullptr);
            throw MessageException( "Allocate Command Buffer Error");

        }

        VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {};
        shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfo.pNext = nullptr;
        shaderStageCreateInfo.flags = 0;
        shaderStageCreateInfo.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
        shaderStageCreateInfo.module = VK_NULL_HANDLE;
        shaderStageCreateInfo.pName  = nullptr;
        shaderStageCreateInfo.pSpecializationInfo = nullptr;


        VkComputePipelineCreateInfo computePipelineCreateInfo = {};
        computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        computePipelineCreateInfo.pNext = nullptr;
        computePipelineCreateInfo.flags = 0;
        computePipelineCreateInfo.stage = shaderStageCreateInfo;
        computePipelineCreateInfo.layout = 0;
        computePipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        computePipelineCreateInfo.basePipelineIndex = 0;


    }

    VkPhysicalDevice VulkanEnd::_SelectBestPhysicalDevice( std::vector<VkPhysicalDevice> const &devices ){
        if( 1 == devices.size() ){
            return devices[0];
        }
        else{
            return devices[0];
        }
    }
    bool             VulkanEnd::_IsSupportGraphics( VkQueueFamilyProperties & properties ){
        return 0 != properties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
    }
    bool             VulkanEnd::_IsSupportCompute( VkQueueFamilyProperties & properties ){
        return 0 != properties.queueFlags & VK_QUEUE_COMPUTE_BIT;

    }
    bool             VulkanEnd::_IsSupportTransfer( VkQueueFamilyProperties & properties ){
        return 0 != properties.queueFlags & VK_QUEUE_TRANSFER_BIT;
    }
}
