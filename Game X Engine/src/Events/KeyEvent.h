#pragma once

#include "Event.h"
namespace gx {
	namespace event {
		class GX_DLL KeyEvent : public GXEvent {
		public:
			inline GXuint32 getKeyVal() const { return keyVal; }
			virtual inline GXuint32 getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_KEYBOARD); }
		protected:
			KeyEvent(GXuint32 KeyValue, GXuint32 WindowID) : keyVal(KeyValue),GXEvent(WindowID) {}
			virtual const char* toString()const override {
				std::stringstream ss;
				ss <<  KeyValToSTR(keyVal) + " " <<this->getName();
				return ss.str().c_str();
			}
			GXuint32 keyVal;
		};


		class GX_DLL KeyPressEvent : public KeyEvent {

		public:
			KeyPressEvent(GXuint32 KeyValue, GXuint32 WindowID) : KeyEvent(KeyValue,WindowID)  {}
			virtual inline const char* getName() const override { return "KEY_PRESSED"; }
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_KEY_PRESSED; }
			
		
		};
		class GX_DLL KeyReleaseEvent : public KeyEvent {

		public:
			KeyReleaseEvent(GXuint32 KeyValue, GXuint32 WindowID) : KeyEvent(KeyValue,WindowID) {}
			virtual inline const char* getName() const override { return "KEY_RELEASED"; }
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_KEY_RELEASED; }

		};
		class GX_DLL KeyTypedEvent : public GXEvent {

		public:
			KeyTypedEvent(char* TextValue, GXuint32 WindowID) : GXEvent(WindowID) { textVal += TextValue; }
			virtual inline const char* getName() const override { return "KEY_TYPED"; }
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_KEY_TYPED; }
			inline const char* getTextVal() const { return textVal.c_str(); }
			virtual inline GXuint32 getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_KEYBOARD); }
		private:
			std::string textVal;
		};

	}

}