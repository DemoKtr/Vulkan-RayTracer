#include "View/vkUtil/frame.h"

void vkUtil::SwapChainFrame::destroy(){

	//logicalDevice.destroyImage(mainimage);
	//logicalDevice.destroyImage(depthBuffer);
	logicalDevice.destroyImageView(mainimageView);
	//logicalDevice.destroyImageView(depthBufferView);

	logicalDevice.destroySemaphore(imageAvailable);
	logicalDevice.destroySemaphore(renderFinished);
	logicalDevice.destroySemaphore(computeFinished);
	logicalDevice.destroyFence(inFlight);
	//logicalDevice.destroyFramebuffer(mainframebuffer);
	logicalDevice.destroyFramebuffer(imguiFrameBuffer);

}
