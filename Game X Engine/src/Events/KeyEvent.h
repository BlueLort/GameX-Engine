#pragma once

#include "Event.h"
namespace gx {
	namespace event {
		class GX_DLL KeyEvent : public GXEvent {
		public:
			inline uint32_t getKeyVal() const { return keyVal; }
			virtual inline uint32_t getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_KEYBOARD); }
		protected:
			KeyEvent(uint32_t KeyValue) : keyVal(KeyValue) {}
			virtual const char* toString()const override {
				std::stringstream ss;
				ss <<  KeyValToSTR(keyVal) + " " <<this->getName();
				return ss.str().c_str();
			}
			uint32_t keyVal;
		};


		class GX_DLL KeyPressEvent : public KeyEvent {

		public:
			KeyPressEvent(uint32_t KeyValue) : KeyEvent(KeyValue) {}
			virtual inline const char* getName() const override { return "KEY_PRESSED"; }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_KEY_PRESSED; }
			
		
		};


		class GX_DLL KeyTypedEvent : public KeyEvent {

		public:
			KeyTypedEvent(uint32_t KeyValue) : KeyEvent(KeyValue) {}
			virtual inline const char* getName() const override { return "KEY_HOLD"; }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_KEY_TYPED; }
	
		};


		class GX_DLL KeyReleaseEvent : public KeyEvent {

		public:
			KeyReleaseEvent(uint32_t KeyValue) : KeyEvent(KeyValue) {}
			virtual inline const char* getName() const override { return "KEY_RELEASED"; }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_KEY_RELEASED; }

		};

	}

}