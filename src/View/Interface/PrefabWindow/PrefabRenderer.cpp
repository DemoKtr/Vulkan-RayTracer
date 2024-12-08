#include "View/Interface/PrefabWindow/PrefabRenderer.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <View/vkUtil/queues.h>
#include <View/vkInit/descrpitors.h>

void vkPrefab::PrefabRenderer::finalize_setup(Scene* scene) {

}

vkPrefab::PrefabRenderer::PrefabRenderer(PrefabInput input) {
	
}

void vkPrefab::PrefabRenderer::InitImGui(GLFWwindow* window) {
	imguiDescriptorPool = vkInit::make_imgui_descriptor_pool(device);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	int windowWidth, windowHeight;
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	io.DisplaySize = ImVec2(static_cast<float>(windowWidth), static_cast<float>(windowHeight));

	// Ustaw styl
	ImGui::StyleColorsDark();

	// 2. Inicjalizacja ImGui dla GLFW
	ImGui_ImplGlfw_InitForVulkan(window, true);
	vkUtil::QueueFamilyIndices indices = vkUtil::findQueueFamilies(physicalDevice, surface, debugMode);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance;
	init_info.PhysicalDevice = physicalDevice;
	init_info.Device = device;
	init_info.QueueFamily = indices.graphicsFamily.value();
	init_info.Queue = graphicsQueue;
	init_info.PipelineCache = nullptr;
	init_info.DescriptorPool = imguiDescriptorPool; // twoja pula deskryptorów ;
	init_info.MinImageCount = 2; // minimalna liczba obrazów ;
	//init_info.ImageCount = swapchainFrames.size(); // liczba obrazów w swap chain ;
	init_info.CheckVkResultFn = nullptr; // Mo¿esz przypisaæ swoj¹ funkcjê do obs³ugi b³êdów
	//init_info.RenderPass = imguiRenderPass;
	ImGui_ImplVulkan_Init(&init_info);

	// Przyk³ad tworzenia deskryptora dla ImGui
	//ImGui_ImplVulkan_CreateFontsTexture(vkCommandBuffer);
}
