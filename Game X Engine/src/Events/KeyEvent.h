#pragma once

#include "Event.h"

namespace gx {
	namespace event {
		class GX_DLL KeyEvent : public Event {
		public:
			inline uint32_t getKeyVal() const { return keyVal; }
			inline uint32_t getEventClass() const override { return (EventClass::GX_INPUT | EventClass::GX_KEYBOARD); }
		protected:
			KeyEvent(uint32_t KeyValue) : keyVal(KeyValue) {}
			const char* toString()const override {
				std::stringstream ss;
				ss <<  KeyValToSTR(keyVal) + " " <<this->getName();
				return ss.str().c_str();
			}
			uint32_t keyVal;
		};


		class GX_DLL KeyPressEvent : public KeyEvent {

		public:
			KeyPressEvent(uint32_t KeyValue) : KeyEvent(KeyValue) {}
			const char* getName() const override { return "KEY_PRESSED"; }
			uint32_t getEventType() const override { return EventType::GX_KEY_PRESSED; }
			
		
		};


		class GX_DLL KeyHoldEvent : public KeyEvent {

		public:
			KeyHoldEvent(uint32_t KeyValue) : KeyEvent(KeyValue) {}
			const char* getName() const override { return "KEY_HOLD"; }
			uint32_t getEventType() const override { return EventType::GX_KEY_HOLD; }
	
		};


		class GX_DLL KeyReleaseEvent : public KeyEvent {

		public:
			KeyReleaseEvent(uint32_t KeyValue) : KeyEvent(KeyValue) {}
			const char* getName() const override { return "KEY_RELEASED"; }
			uint32_t getEventType() const override { return EventType::GX_KEY_RELEASED; }

		};

	}

}