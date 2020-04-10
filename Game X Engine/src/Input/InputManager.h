#pragma once
#include "Config.h"
#include "pch.h"
#include "GXKeys.h"
#include "Window/GXWindow.h"
namespace gx {
	class GX_DLL InputManager {
	public:

		InputManager(const InputManager& inst) = delete;
		inline void init() {
			keyState = std::vector<bool>(event::key::GX_NUM_SCANCODES);
			mouseLoc = std::make_pair(0, 0);//x,y
			mouseLocRel = std::make_pair(0, 0);//xRel,yRel
			mouseWheel = std::make_pair(0, 0);//horizontal,vertical
		}
		inline void update() {
			mouseWheel.first = 0;
			mouseWheel.second = 0;
			mouseLocRel.first = 0;
			mouseLocRel.second = 0;
			text = "";
		}
		inline static InputManager& getInstance() {
			//TO DO MAKE IT HEAP ALLOCATED INSTEAD.
			static InputManager instance;
			return instance;
		}
		//EVENTS
		template<class T>
		inline GXint32 handleEvent(std::shared_ptr<T>& Event) { return 0; }

		//INPUT POLLING 
		inline bool isPressed(GXuint32 keyVal) const { return keyState[keyVal]; }
		inline std::string getTextTyped() const { return text; }
		//x,y
		inline std::pair<GXint32, GXint32> getMouseLoc() const { return mouseLoc; }
		//xRel,yRel
		inline std::pair<GXint32, GXint32> getMouseLocRel() const { return mouseLocRel; }
		//horizontal,vertical
		inline std::pair<GXint32, GXint32> getMouseWheel() const { return mouseWheel; }
	private:
		InputManager() {}
		std::vector<bool> keyState;
		std::string text;
		std::pair<GXint32, GXint32> mouseLoc;//x,y
		std::pair<GXint32, GXint32> mouseLocRel;//xRel,yRel
		std::pair<GXint32, GXint32> mouseWheel;//horizontal,vertical
	};

	//EVENTS 
	template<>
	inline GXint32 InputManager::handleEvent<gx::event::KeyPressEvent>(std::shared_ptr<gx::event::KeyPressEvent>& Event) {
		keyState[Event->getKeyVal()] = true;
		return 1;
	}
	template<>
	inline GXint32 InputManager::handleEvent<gx::event::KeyReleaseEvent>(std::shared_ptr<gx::event::KeyReleaseEvent>& Event) {
		keyState[Event->getKeyVal()] = false;
		return 1;
	}
	template<>
	inline GXint32 InputManager::handleEvent<gx::event::KeyTypedEvent>(std::shared_ptr<gx::event::KeyTypedEvent>& Event) {
		text = Event->getTextVal();
		return 1;
	}
	template<>
	inline GXint32 InputManager::handleEvent<gx::event::MouseScrollEvent>(std::shared_ptr<gx::event::MouseScrollEvent>& Event) {
		mouseWheel.first += Event->getXOffset() > 0 ? 1 : -1;
		mouseWheel.second += Event->getYOffset() > 0 ? 1 : -1;
		return 1;
	}
	template<>
	inline GXint32 InputManager::handleEvent<gx::event::MousePressEvent>(std::shared_ptr<gx::event::MousePressEvent>& Event) {
		keyState[Event->getKeyVal()] = true;
		return 1;
	}
	template<>
	inline GXint32 InputManager::handleEvent<gx::event::MouseReleaseEvent>(std::shared_ptr<gx::event::MouseReleaseEvent>& Event) {
		keyState[Event->getKeyVal()] = false;
		return 1;
	}
	template<>
	inline GXint32 InputManager::handleEvent<gx::event::MouseMoveEvent>(std::shared_ptr<gx::event::MouseMoveEvent>& Event) {
		mouseLoc.first = Event->getXVal();
		mouseLoc.second = Event->getYVal();
		return 1;
	}
	template<>
	inline GXint32 InputManager::handleEvent<gx::event::MouseMoveRelEvent>(std::shared_ptr<gx::event::MouseMoveRelEvent>& Event) {
		mouseLocRel.first = Event->getXVal();
		mouseLocRel.second = Event->getYVal();
		return 1;
	}

}