#include "View/Interface/Thumbs/thumbs.h"

vkThumbs::Thumb::Thumb(ThumbInput input) {
	this->device = input.logicalDevice;
	this->physicalDevice = input.physicalDevice;
	this->commandBuffer = input.commandBuffer;
	this->queue = input.queue;
	this->layout = input.layout;
	this->descriptorPool = input.descriptorPool;
}
