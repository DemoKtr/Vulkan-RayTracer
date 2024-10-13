#pragma once
#include "config.h"

namespace vkInit {


	vk::Semaphore make_semaphore(vk::Device device, bool debug);

	/**
		Make a fence.

		\param device the logical device
		\param debug whether the system is running in debug mode
		\returns the created fence
	*/
	vk::Fence make_fence(vk::Device device, bool debug);
}