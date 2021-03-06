#include <Buffalo/Core/InputState.hpp>
#include <Buffalo/Core/Debug.hpp>
#include <Buffalo/Core/WindowInputAdapter.hpp>

namespace Buffalo {
	Core::WindowInputAdapter* InputState::adapter = nullptr;

	void InputState::SetAdapter(Core::WindowInputAdapter* windowAdapter) {
		BUFFALO_PROFILE_FUNC();
		BUFFALO_LOG_DEBUG("Initializing InputState");
		assert(windowAdapter != nullptr);
		adapter = windowAdapter;
	}

	void InputState::ReleaseAdapter() {
		BUFFALO_PROFILE_FUNC();
		BUFFALO_LOG_DEBUG("Removed InputState window adapter");
		adapter = nullptr;
	}

	bool InputState::IsKeyPressed(Key key) {
		BUFFALO_PROFILE_FUNC();
		assert(adapter != nullptr);
		return adapter->IsKeyPressed(key);
	}

	bool InputState::IsMouseButtonPressed(MouseCodes mouseCode) {
		BUFFALO_PROFILE_FUNC();
		assert(adapter != nullptr);
		return adapter->IsMouseButtonPressed(mouseCode);
	}

	glm::vec2 InputState::GetMousePosition() {
		BUFFALO_PROFILE_FUNC();
		assert(adapter != nullptr);
		return adapter->GetMousePosition();
	}

	glm::vec2 InputState::GetMousePositionDifferential() {
		BUFFALO_PROFILE_FUNC();
		assert(adapter != nullptr);
		return adapter->GetMousePositionDifferential();
	}

}