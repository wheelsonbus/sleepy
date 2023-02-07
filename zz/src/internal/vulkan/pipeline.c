#if defined(ZZ__VULKAN)

#include "pipeline.h"

#include <stdio.h>

#include "zz/log.h"

b8 internal_vulkan_pipeline_create(struct internal_vulkan_pipeline* pipeline, struct internal_vulkan_pipeline_config* config)
{
    pipeline->device = config->device;
    pipeline->render_pass = config->render_pass;
    pipeline->swapchain = config->swapchain;
    pipeline->uniform_buffers = config->uniform_buffers;
    
    VkShaderModule vertexShaderModule;
    internal_vulkan_create_shader_module(&vertexShaderModule, pipeline->device->device, "vertex.spv");
    VkShaderModule fragmentShaderModule;
    internal_vulkan_create_shader_module(&fragmentShaderModule, pipeline->device->device, "fragment.spv");

    VkDescriptorPoolSize descriptorPoolSize;
    descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorPoolSize.descriptorCount = pipeline->uniform_buffers->length;

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = ZZ_NULL;
    descriptorPoolCreateInfo.flags = 0;
    descriptorPoolCreateInfo.maxSets = pipeline->uniform_buffers->length;
    descriptorPoolCreateInfo.poolSizeCount = 1;
    descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;

    if (vkCreateDescriptorPool(pipeline->device->device, &descriptorPoolCreateInfo, ZZ_NULL, &pipeline->descriptorPool) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    VkDescriptorSetLayoutBinding uboDescriptorSetLayoutBinding;
    uboDescriptorSetLayoutBinding.binding = 0;
    uboDescriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboDescriptorSetLayoutBinding.descriptorCount = 1;
    uboDescriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboDescriptorSetLayoutBinding.pImmutableSamplers = ZZ_NULL;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.pNext = ZZ_NULL;
    descriptorSetLayoutCreateInfo.flags = 0;
    descriptorSetLayoutCreateInfo.bindingCount = 1;
    descriptorSetLayoutCreateInfo.pBindings = &uboDescriptorSetLayoutBinding;

    if (vkCreateDescriptorSetLayout(pipeline->device->device, &descriptorSetLayoutCreateInfo, ZZ_NULL, &pipeline->descriptorSetLayout) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    memory_array_t(VkDescriptorSetLayout) descriptorSetLayouts;
    memory_array_create_and_reserve(&descriptorSetLayouts, pipeline->uniform_buffers->length);
    for (u16 i = 0; i < descriptorSetLayouts.capacity; i += 1)
    {
        memory_array_push(&descriptorSetLayouts, pipeline->descriptorSetLayout);
    }

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.pNext = ZZ_NULL;
    descriptorSetAllocateInfo.descriptorPool = pipeline->descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = descriptorSetLayouts.length;
    descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data;

    memory_array_create_and_reserve(&pipeline->descriptorSets, pipeline->uniform_buffers->length);
    if (vkAllocateDescriptorSets(pipeline->device->device, &descriptorSetAllocateInfo, pipeline->descriptorSets.data) != VK_SUCCESS)
    {
        memory_array_destroy(&descriptorSetLayouts);
        return ZZ_FALSE;
    }

    for (u16 i = 0; i < descriptorSetLayouts.length; i += 1)
    {
        VkDescriptorBufferInfo descriptorBufferInfo;
        descriptorBufferInfo.buffer = pipeline->uniform_buffers->data[i].buffer;
        descriptorBufferInfo.offset = 0;
        descriptorBufferInfo.range = VK_WHOLE_SIZE;

        VkWriteDescriptorSet writeDescriptorSet;
        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.pNext = ZZ_NULL;
        writeDescriptorSet.dstSet = pipeline->descriptorSets.data[i];
        writeDescriptorSet.dstBinding = 0;
        writeDescriptorSet.dstArrayElement = 0;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.pImageInfo = ZZ_NULL;
        writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
        writeDescriptorSet.pTexelBufferView = ZZ_NULL;

        vkUpdateDescriptorSets(pipeline->device->device, 1, &writeDescriptorSet, 0, ZZ_NULL);
    }
    
    memory_array_destroy(&descriptorSetLayouts);

    VkPipelineShaderStageCreateInfo vertexPipelineShaderStageCreateInfo;
    vertexPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexPipelineShaderStageCreateInfo.pNext = ZZ_NULL;
    vertexPipelineShaderStageCreateInfo.flags = 0;
    vertexPipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexPipelineShaderStageCreateInfo.module = vertexShaderModule;
    vertexPipelineShaderStageCreateInfo.pName = "main";
    vertexPipelineShaderStageCreateInfo.pSpecializationInfo = ZZ_NULL;
    
    VkPipelineShaderStageCreateInfo fragmentPipelineShaderStageCreateInfo;
    fragmentPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentPipelineShaderStageCreateInfo.pNext = ZZ_NULL;
    fragmentPipelineShaderStageCreateInfo.flags = 0;
    fragmentPipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentPipelineShaderStageCreateInfo.module = fragmentShaderModule;
    fragmentPipelineShaderStageCreateInfo.pName = "main";
    fragmentPipelineShaderStageCreateInfo.pSpecializationInfo = ZZ_NULL;

    uint32_t pipelineShaderStageCreateInfoCount = 2;
    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2] = {vertexPipelineShaderStageCreateInfo, fragmentPipelineShaderStageCreateInfo};

    VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo;
    pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipelineDynamicStateCreateInfo.pNext = ZZ_NULL;
    pipelineDynamicStateCreateInfo.flags = 0;
    pipelineDynamicStateCreateInfo.dynamicStateCount = 2;
    VkDynamicState dynamicStates[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    pipelineDynamicStateCreateInfo.pDynamicStates = dynamicStates;

    VkVertexInputBindingDescription vertexInputBindingDescription;
    vertexInputBindingDescription.binding = 0;
    vertexInputBindingDescription.stride = sizeof(struct internal_vulkan_vertex);
    vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription positionVertexInputAttributeDescription;
    positionVertexInputAttributeDescription.location = 0;
    positionVertexInputAttributeDescription.binding = 0;
    positionVertexInputAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionVertexInputAttributeDescription.offset = offsetof(struct internal_vulkan_vertex, position);

    uint32_t vertexInputAttributeDescriptionCount = 1;
    VkVertexInputAttributeDescription vertexInputAttributeDescriptions[1] = {positionVertexInputAttributeDescription};

    VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
    pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pipelineVertexInputStateCreateInfo.pNext = ZZ_NULL;
    pipelineVertexInputStateCreateInfo.flags = 0;
    pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
    pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
    pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexInputAttributeDescriptionCount;
    pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions;

    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.pNext = ZZ_NULL;
    pipelineInputAssemblyStateCreateInfo.flags = 0;
    pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)pipeline->swapchain->extent.width;
    viewport.height = (float)pipeline->swapchain->extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    
    VkRect2D scissor;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = pipeline->swapchain->extent;

    VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;
    pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineViewportStateCreateInfo.pNext = ZZ_NULL;
    pipelineViewportStateCreateInfo.flags = 0;
    pipelineViewportStateCreateInfo.viewportCount = 1;
    pipelineViewportStateCreateInfo.pViewports = &viewport;
    pipelineViewportStateCreateInfo.scissorCount = 1;
    pipelineViewportStateCreateInfo.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo;
    pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipelineRasterizationStateCreateInfo.pNext = ZZ_NULL;
    pipelineRasterizationStateCreateInfo.flags = 0;
    pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
    pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
    pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;
    pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineMultisampleStateCreateInfo.pNext = ZZ_NULL;
    pipelineMultisampleStateCreateInfo.flags = 0;
    pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
    pipelineMultisampleStateCreateInfo.pSampleMask = ZZ_NULL;
    pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

    // TODO Depth and stencil pipeline states?

    VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
    pipelineColorBlendAttachmentState.blendEnable = VK_TRUE; // TODO Color blending?
    pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo pipelineColorBlendState;
    pipelineColorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipelineColorBlendState.pNext = ZZ_NULL;
    pipelineColorBlendState.flags = 0;
    pipelineColorBlendState.logicOpEnable = VK_FALSE;
    pipelineColorBlendState.logicOp = VK_LOGIC_OP_COPY;
    pipelineColorBlendState.attachmentCount = 1;
    pipelineColorBlendState.pAttachments = &pipelineColorBlendAttachmentState;
    pipelineColorBlendState.blendConstants[0] = 0.0f;
    pipelineColorBlendState.blendConstants[1] = 0.0f;
    pipelineColorBlendState.blendConstants[2] = 0.0f;
    pipelineColorBlendState.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = ZZ_NULL;
    pipelineLayoutCreateInfo.flags = 0;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &pipeline->descriptorSetLayout;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = ZZ_NULL;

    if (vkCreatePipelineLayout(pipeline->device->device, &pipelineLayoutCreateInfo, ZZ_NULL, &pipeline->pipelineLayout) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
    graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.pNext = ZZ_NULL;
    graphicsPipelineCreateInfo.flags = 0;
    graphicsPipelineCreateInfo.stageCount = pipelineShaderStageCreateInfoCount;
    graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfos;
    graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
    graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
    graphicsPipelineCreateInfo.pTessellationState = ZZ_NULL;
    graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
    graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
    graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
    graphicsPipelineCreateInfo.pDepthStencilState = ZZ_NULL;
    graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendState;
    graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
    graphicsPipelineCreateInfo.layout = pipeline->pipelineLayout;
    graphicsPipelineCreateInfo.renderPass = pipeline->render_pass->renderPass;
    graphicsPipelineCreateInfo.subpass = 0;
    graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    graphicsPipelineCreateInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(pipeline->device->device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, ZZ_NULL, &pipeline->pipeline) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    internal_vulkan_destroy_shader_module(&vertexShaderModule, pipeline->device->device);
    internal_vulkan_destroy_shader_module(&fragmentShaderModule, pipeline->device->device);

    return ZZ_TRUE;
}

void internal_vulkan_pipeline_destroy(struct internal_vulkan_pipeline* pipeline)
{
    vkDestroyPipeline(pipeline->device->device, pipeline->pipeline, ZZ_NULL);
    pipeline->pipeline = VK_NULL_HANDLE;
    vkDestroyPipelineLayout(pipeline->device->device, pipeline->pipelineLayout, ZZ_NULL);
    pipeline->pipelineLayout = VK_NULL_HANDLE;
    memory_array_destroy(&pipeline->descriptorSets);
    vkDestroyDescriptorPool(pipeline->device->device, pipeline->descriptorPool, ZZ_NULL);
    pipeline->descriptorPool = VK_NULL_HANDLE;
    vkDestroyDescriptorSetLayout(pipeline->device->device, pipeline->descriptorSetLayout, ZZ_NULL);
    pipeline->descriptorSetLayout = VK_NULL_HANDLE;
}

b8 internal_vulkan_create_shader_module(VkShaderModule* shaderModule, VkDevice device, const char* filename)
{
    FILE* file;

    file = fopen(filename, "rb");
    if (file == ZZ_NULL)
    {
        return ZZ_FALSE;
    }
    fseek(file, 0, SEEK_END);
    u64 bytecode_size = ftell(file);
    fclose(file);

    char bytecode[bytecode_size];
    file = fopen(filename, "rb");
    fread(bytecode, sizeof(char), bytecode_size, file);
    fclose(file);

    VkShaderModuleCreateInfo shaderModuleCreateInfo;
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.pNext = ZZ_NULL;
    shaderModuleCreateInfo.flags = 0;
    shaderModuleCreateInfo.codeSize = bytecode_size;
    shaderModuleCreateInfo.pCode = (uint32_t*)bytecode;

    if (vkCreateShaderModule(device, &shaderModuleCreateInfo, ZZ_NULL, shaderModule) != VK_SUCCESS)
    {
        return ZZ_FALSE;
    }

    return ZZ_TRUE;
}

void internal_vulkan_destroy_shader_module(VkShaderModule* shaderModule, VkDevice device)
{
    vkDestroyShaderModule(device, *shaderModule, ZZ_NULL);
    *shaderModule = VK_NULL_HANDLE;
}

#endif