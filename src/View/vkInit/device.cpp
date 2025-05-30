#include "View/vkInit/device.h"
#include "View/vkInit/logging.h"
#include "View/vkUtil/queues.h"
#include "settings.h"
#include <set>
vk::PhysicalDevice vkInit::choose_physical_device(vk::Instance& instance, bool debugMode)
{
	if (debugMode) {
		std::cout << "Choosing physical device " << std::endl;
	}

	std::vector<vk::PhysicalDevice> avilableDevices = instance.enumeratePhysicalDevices();

	if (debugMode) {
		std::cout << "Are " << avilableDevices.size() << "Physical Device/s avilable" << std::endl;
	}

	for (vk::PhysicalDevice device : avilableDevices) {
		if (debugMode) {
			log_device_properties(device);
		}
		if (isSuitable(device, debugMode)) {
			return device;
		}
	}

	return nullptr;
}

bool vkInit::isSuitable(const vk::PhysicalDevice& device, bool debugMode)
{
	if (debugMode) {
		std::cout << "Checking if device is suitable\n";
	}

	/*
	* A device is suitable if it can present to the screen, ie support
	* the swapchain extension
	*/
	const std::vector<const char*> requestedExtensions = {
		{VK_KHR_SWAPCHAIN_EXTENSION_NAME},  {VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS_EXTENSION_NAME},
		VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME ,
		VK_KHR_RAY_QUERY_EXTENSION_NAME, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
		VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME, VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
		VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME, VK_KHR_SPIRV_1_4_EXTENSION_NAME,
		VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
		VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,
		VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
		VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME,
		VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,

	};
	
	if (debugMode) {
		std::cout << "We are requesting device extensions:\n";

		for (const char* extension : requestedExtensions) {
			std::cout << "\t\"" << extension << "\"\n";
		}

	}

	if (bool extensionsSupported = checkDeviceExtensionSupport(device, requestedExtensions, debugMode)) {

		if (debugMode) {
			std::cout << "Device can support the requested extensions!\n";
		}
	}
	else {

		if (debugMode) {
			std::cout << "Device can't support the requested extensions!\n";
		}

		return false;
	}
	return true;
}

bool vkInit::checkDeviceExtensionSupport(
	const vk::PhysicalDevice& device,
	const std::vector<const char*>& requestedExtensions,
	const bool debugMode
) {

	/*
	* Check if a given physical device can satisfy a list of requested device
	* extensions.
	*/

	std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());

	if (debugMode) {
		std::cout << "Device can support extensions:\n";
	}

	for (vk::ExtensionProperties& extension : device.enumerateDeviceExtensionProperties()) {

		if (debugMode) {
			std::cout << "\t\"" << extension.extensionName << "\"\n";
		}

		//remove this from the list of required extensions (set checks for equality automatically)
		requiredExtensions.erase(extension.extensionName);
	}

	//if the set is empty then all requirements have been satisfied
	return requiredExtensions.empty();
}

vk::Device vkInit::create_logical_device(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debugMode)
{
	vkUtil::QueueFamilyIndices indices = vkUtil::findQueueFamilies(physicalDevice, surface, debugMode);
	std::vector<uint32_t> uniqueIndices;
	uniqueIndices.push_back(indices.graphicsFamily.value());
	uniqueIndices.push_back(indices.computeFamily.value());
	uniqueIndices.push_back(indices.transferFamily.value());
	/*
	if (indices.graphicsFamily.value() != indices.computeFamily.value()) {
		uniqueIndices.push_back(indices.presentFamily.value());
	}
	*/
	std::vector<float> queuePriorities = { 1.0f, 1.0f };
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
	for (uint32_t queueFamilyIndex : uniqueIndices) {
			// Sprawd�, czy chcesz utworzy� wi�cej ni� jedn� kolejk� w tej rodzinie
			uint32_t queueCount = (physicalDevice.getQueueFamilyProperties()[queueFamilyIndex].queueCount > 1) ?
				2 : 1;
			vk::DeviceQueueCreateInfo info = {};
			info.sType = vk::StructureType::eDeviceQueueCreateInfo;
			info.queueFamilyIndex = queueFamilyIndex;
			info.queueCount = queueCount;
			info.pQueuePriorities = queuePriorities.data();
			queueCreateInfo.push_back(info);
	

	}

	std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME, 
		VK_KHR_RAY_QUERY_EXTENSION_NAME, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
		VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME, VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
		VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME, VK_KHR_SPIRV_1_4_EXTENSION_NAME,
		VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
		VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,
		VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
		VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME, 
		VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
		VK_EXT_MESH_SHADER_EXTENSION_NAME,
		//VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME,
		VK_KHR_MAINTENANCE_4_EXTENSION_NAME,
		VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME,
		VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME,
		VK_KHR_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_EXTENSION_NAME,
		VK_KHR_SHADER_SUBGROUP_ROTATE_EXTENSION_NAME,
	};
	
	
		
	vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();
	deviceFeatures.geometryShader = VK_TRUE;
	deviceFeatures.sampleRateShading = VK_TRUE;


	vk::PhysicalDeviceMeshShaderFeaturesEXT meshShaderFeatures = {};
	meshShaderFeatures.sType = vk::StructureType::ePhysicalDeviceMeshShaderFeaturesEXT;
	meshShaderFeatures.meshShader = VK_TRUE;
	meshShaderFeatures.taskShader = VK_TRUE;
	//meshShaderFeatures.pNext = &deviceFeatures;


	/*
	vk::PhysicalDeviceGraphicsPipelineLibraryFeaturesEXT gpl{};
	gpl.sType = vk::StructureType::ePhysicalDeviceGraphicsPipelineLibraryFeaturesEXT;
	gpl.graphicsPipelineLibrary = VK_TRUE;
	gpl.pNext = &meshShaderFeatures;
	*/
	vk::PhysicalDeviceMaintenance4FeaturesKHR maintenance4Features{};
	maintenance4Features.sType = vk::StructureType::ePhysicalDeviceMaintenance4Features;
	maintenance4Features.maintenance4 = VK_TRUE;
	maintenance4Features.pNext = &meshShaderFeatures;

	//deviceFeatures.samplerAnisotropy = true;
	std::vector<const char*> enabledLayers;
	if (debugMode) {
		enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
		enabledLayers.push_back("VK_LAYER_RENDERDOC_Capture");
	}
	vk::PhysicalDeviceBufferAddressFeaturesEXT bufferDeviceAddressFeatures;
	bufferDeviceAddressFeatures.sType = vk::StructureType::ePhysicalDeviceBufferDeviceAddressFeatures;
	bufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;
	bufferDeviceAddressFeatures.pNext = &maintenance4Features;


	vk::PhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures;
	dynamicRenderingFeatures.dynamicRendering = VK_TRUE;
	dynamicRenderingFeatures.sType = vk::StructureType::ePhysicalDeviceDynamicRenderingFeaturesKHR;
	dynamicRenderingFeatures.pNext = &bufferDeviceAddressFeatures;


	vk::PhysicalDeviceFeatures2 deviceFeatures2;
	deviceFeatures2.pNext = &dynamicRenderingFeatures;
	deviceFeatures2.sType = vk::StructureType::ePhysicalDeviceFeatures2;

	vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(vk::DeviceCreateFlags(),
		queueCreateInfo.size(), queueCreateInfo.data(),
		enabledLayers.size(), enabledLayers.data(),
		deviceExtensions.size(), deviceExtensions.data(),
		NULL);

	deviceInfo.pNext = &dynamicRenderingFeatures;

		
	try {
		vk::Device device = physicalDevice.createDevice(deviceInfo);
		if (debugMode) {
			std::cout << "Device is successfully created" << std::endl;
		}
		
		return device;
	}
	catch (vk::SystemError err) {
		if (debugMode) {
			std::cout << "Device creation FAILED!!!" << std::endl;
			return nullptr;
		}
	}
	return nullptr;
}

std::array<vk::Queue,4> vkInit::get_Queues(vk::PhysicalDevice physicalDevice, vk::Device device, vk::SurfaceKHR surface, bool debugMode)
{
	vkUtil::QueueFamilyIndices indices = vkUtil::findQueueFamilies(physicalDevice, surface, debugMode);
	return { { device.getQueue(indices.graphicsFamily.value(),0), device.getQueue(indices.presentFamily.value(),0), device.getQueue(indices.computeFamily.value(),0), device.getQueue(indices.transferFamily.value(),0)} };
}
