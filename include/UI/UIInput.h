#pragma once

namespace UI {

	struct UIRenderingDrawData {
		size_t UIinstanceCount = 0;
		size_t UILettersinstanceCount = 0;

		void reset() {
			UIinstanceCount = 0;
			UILettersinstanceCount = 0;
		}
	};
 
}