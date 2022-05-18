#pragma once
#include <exception>
#include <string>
#include <vulkan/vulkan.h>

namespace VK{

#define WARP_VK_FUNCTION( FUNC )

    class vk_func_wraper{
        vk_func_wraper(){
        }

    };

    class MessageException : public std::bad_exception{
    public:
        MessageException( const char * msg)
            : m_msg( msg )
        {
        }
        MessageException( MessageException const& me )
            : m_msg( me.m_msg )
        {
        }
        MessageException& operator=( MessageException const& me){
            if( this != &me)
                m_msg = me.m_msg;
            return *this;
        }

        const char * what() const noexcept override{
            return m_msg.c_str();
        }
    private:
        std::string m_msg;
    };
    
    class VukandShader{
    public:
        VulkanShader( );
        VkShaderModule getShaderModule() const;
        VkShaderStageFlagsBits getShaderStageFlagBits()const;
        const char * getEntryName() const;
    private:
        VkShaderModule m_shaderModule;
        VkShaderStageFlagBits m_shaderStage;
        std::string   m_entryName;

    };

    class VulkanEnd{
    public:
        VulkanEnd();
    private:
        VkPhysicalDevice _SelectBestPhysicalDevice( std::vector<VkPhysicalDevice> const &devices );
        bool             _IsSupportGraphics( VkQueueFamilyProperties & properties );
        bool             _IsSupportCompute( VkQueueFamilyProperties & properties );
        bool             _IsSupportTransfer( VkQueueFamilyProperties & properties );
    private:
        VkInstance       m_instance;
        VkPhysicalDevice m_physicalDevice;
        VkDevice         m_device;
        uint32_t         m_queueIndex;
        VkCommandPool    m_commandPool;
        VkCommandBuffer  m_commandBuffer;
    };


}