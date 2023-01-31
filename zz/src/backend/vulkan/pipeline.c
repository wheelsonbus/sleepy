#if defined(ZZ_BACKEND_VULKAN)

#include "pipeline.h"

#include <stdio.h>

#include "zz/log.h"

b8 backend_vulkan_pipeline_create(struct backend_vulkan_pipeline* pipeline, struct backend_vulkan_pipeline_config* config)
{
    pipeline->device = config->device;
    pipeline->render_pass = config->render_pass;
    pipeline->swapchain = config->swapchain;

    VkShaderModule vertexShaderModule;
    backend_vulkan_create_shader_module(&vertexShaderModule, pipeline->device->device, "../playground/shaders/shader.vert.spv");
    VkShaderModule fragmentShaderModule;
    backend_vulkan_create_shader_module(&fragmentShaderModule, pipeline->device->device, "../playground/shaders/shader.frag.spv");

    VkPipelineShaderStageCreateInfo vertexPipelineShaderStageCreateInfo;
    vertexPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexPipelineShaderStageCreateInfo.pNext = NULL;
    vertexPipelineShaderStageCreateInfo.flags = 0;
    vertexPipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexPipelineShaderStageCreateInfo.module = vertexShaderModule;
    vertexPipelineShaderStageCreateInfo.pName = "main";
    vertexPipelineShaderStageCreateInfo.pSpecializationInfo = NULL;
    
    VkPipelineShaderStageCreateInfo fragmentPipelineShaderStageCreateInfo;
    fragmentPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentPipelineShaderStageCreateInfo.pNext = NULL;
    fragmentPipelineShaderStageCreateInfo.flags = 0;
    fragmentPipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentPipelineShaderStageCreateInfo.module = fragmentShaderModule;
    fragmentPipelineShaderStageCreateInfo.pName = "main";
    fragmentPipelineShaderStageCreateInfo.pSpecializationInfo = NULL;

    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2] = {vertexPipelineShaderStageCreateInfo, fragmentPipelineShaderStageCreateInfo};

    VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo;
    pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipelineDynamicStateCreateInfo.pNext = NULL;
    pipelineDynamicStateCreateInfo.flags = 0;
    pipelineDynamicStateCreateInfo.dynamicStateCount = 2;
    VkDynamicState dynamicStates[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    pipelineDynamicStateCreateInfo.pDynamicStates = dynamicStates;

    VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
    pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pipelineVertexInputStateCreateInfo.pNext = NULL;
    pipelineVertexInputStateCreateInfo.flags = 0;
    pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
    pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = NULL;
    pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;
    pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = NULL;

    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.pNext = NULL;
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
    pipelineViewportStateCreateInfo.pNext = NULL;
    pipelineViewportStateCreateInfo.flags = 0;
    pipelineViewportStateCreateInfo.viewportCount = 1;
    pipelineViewportStateCreateInfo.pViewports = &viewport;
    pipelineViewportStateCreateInfo.scissorCount = 1;
    pipelineViewportStateCreateInfo.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo;
    pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipelineRasterizationStateCreateInfo.pNext = NULL;
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
    pipelineMultisampleStateCreateInfo.pNext = NULL;
    pipelineMultisampleStateCreateInfo.flags = 0;
    pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
    pipelineMultisampleStateCreateInfo.pSampleMask = NULL;
    pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

    // TODO Depth and stencil pipeline states?

    VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
    pipelineColorBlendAttachmentState.blendEnable = VK_FALSE; // TODO Color blending?
    pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo pipelineColorBlendState;
    pipelineColorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipelineColorBlendState.pNext = NULL;
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
    pipelineLayoutCreateInfo.pNext = NULL;
    pipelineLayoutCreateInfo.flags = 0;
    pipelineLayoutCreateInfo.setLayoutCount = 0;
    pipelineLayoutCreateInfo.pSetLayouts = NULL;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = NULL;

    if (vkCreatePipelineLayout(pipeline->device->device, &pipelineLayoutCreateInfo, NULL, &pipeline->pipelineLayout) != VK_SUCCESS)
    {
        return FALSE;
    }

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
    graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.pNext = NULL;
    graphicsPipelineCreateInfo.flags = 0;
    graphicsPipelineCreateInfo.stageCount = 2;
    graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfos;
    graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
    graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
    graphicsPipelineCreateInfo.pTessellationState = NULL;
    graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
    graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
    graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
    graphicsPipelineCreateInfo.pDepthStencilState = NULL;
    graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendState;
    graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
    graphicsPipelineCreateInfo.layout = pipeline->pipelineLayout;
    graphicsPipelineCreateInfo.renderPass = pipeline->render_pass->renderPass;
    graphicsPipelineCreateInfo.subpass = 0;
    graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    graphicsPipelineCreateInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(pipeline->device->device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, NULL, &pipeline->pipeline) != VK_SUCCESS)
    {
        return FALSE;
    }

    backend_vulkan_destroy_shader_module(&vertexShaderModule, pipeline->device->device);
    backend_vulkan_destroy_shader_module(&fragmentShaderModule, pipeline->device->device);

    return TRUE;
}

void backend_vulkan_pipeline_destroy(struct backend_vulkan_pipeline* pipeline)
{
    vkDestroyPipeline(pipeline->device->device, pipeline->pipeline, NULL);
    pipeline->pipeline = VK_NULL_HANDLE;
    vkDestroyPipelineLayout(pipeline->device->device, pipeline->pipelineLayout, NULL);
    pipeline->pipelineLayout = VK_NULL_HANDLE;
}

b8 backend_vulkan_create_shader_module(VkShaderModule* shaderModule, VkDevice device, const char* filename)
{
    FILE* file;

    file = fopen(filename, "rb");
    if (file == NULL)
    {
        return FALSE;
    }
    fseek(file, 0, SEEK_END);
    u64 bytecode_size = ftell(file);
    fclose(file);

    u8 bytecode[bytecode_size];
    file = fopen(filename, "rb");
    fread(bytecode, sizeof(u8), bytecode_size - 1, file);
    fclose(file);

    VkShaderModuleCreateInfo shaderModuleCreateInfo;
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.pNext = NULL;
    shaderModuleCreateInfo.flags = 0;
    shaderModuleCreateInfo.codeSize = bytecode_size;
    shaderModuleCreateInfo.pCode = (uint32_t*)bytecode;

    if (vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, shaderModule) != VK_SUCCESS)
    {
        return FALSE;
    }

    return TRUE;
}

void backend_vulkan_destroy_shader_module(VkShaderModule* shaderModule, VkDevice device)
{
    vkDestroyShaderModule(device, *shaderModule, NULL);
    *shaderModule = VK_NULL_HANDLE;
}

#endif