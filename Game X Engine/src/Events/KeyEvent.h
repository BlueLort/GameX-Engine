#pragma once

#include "Event.h"
namespace gx {
	namespace event {
		class GX_DLL KeyEvent : public GXEvent {
		public:
			inline uint32_t getKeyVal() const { return keyVal; }
			virtual inline uint32_t getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_KEYBOARD); }
		protected:
			KeyEvent(uint32_t KeyValue, uint32_t WindowID) : keyVal(KeyValue),GXEvent(WindowID) {}
			virtual const char* toString()const override {
				std::stringstream ss;
				ss <<  KeyValToSTR(keyVal) + " " <<this->getName();
				return ss.str().c_str();
			}
			uint32_t keyVal;
		};


		class GX_DLL KeyPressEvent : public KeyEvent {

		public:
			KeyPressEvent(uint32_t KeyValue, uint32_t WindowID) : KeyEvent(KeyValue,WindowID)  {}
			virtual inline const char* getName() const override { return "KEY_PRESSED"; }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_KEY_PRESSED; }
			
		
		};
		class GX_DLL KeyReleaseEvent : public KeyEvent {

		public:
			KeyReleaseEvent(uint32_t KeyValue, uint32_t WindowID) : KeyEvent(KeyValue,WindowID) {}
			virtual inline const char* getName() const override { return "KEY_RELEASED"; }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_KEY_RELEASED; }

		};
		class GX_DLL KeyTypedEvent : public GXEvent {

		public:
			KeyTypedEvent(char* TextValue, uint32_t WindowID) : GXEvent(WindowID) { textVal += TextValue; }
			virtual inline const char* getName() const override { return "KEY_TYPED"; }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_KEY_TYPED; }
			inline const char* getTextVal() const { return textVal.c_str(); }
			virtual inline uint32_t getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_KEYBOARD); }
		private:
			std::string textVal;
		};

	}

}