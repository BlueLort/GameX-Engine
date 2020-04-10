#pragma once

#include "Event.h"
namespace gx {
	namespace event {
		class GX_DLL MouseButtonEvent : public GXEvent {
		public:

			inline GXuint32 getKeyVal() const { return keyVal; }
			virtual inline GXuint32 getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE | GXEventClass::GX_MOUSE_BUTTON); }
			virtual inline const char* toString()const override {
				std::stringstream ss;
				ss << KeyValToSTR(keyVal) + " " << this->getName();
				return ss.str().c_str();
			}
		protected:

			MouseButtonEvent(GXuint32 KeyValue, GXuint32 WindowID) : keyVal(KeyValue) , GXEvent(WindowID) {}
		
			GXuint32 keyVal;

		};

		class GX_DLL MousePressEvent : public MouseButtonEvent {

		public:
			MousePressEvent(GXuint32 KeyValue, GXuint32 WindowID) : MouseButtonEvent(KeyValue,WindowID) {}
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_MOUSE_PRESSED; };
			virtual inline  const char* getName() const override { return "MOUSE_PRESSED"; }

		};

		class GX_DLL MouseReleaseEvent : public MouseButtonEvent {

		public:
			MouseReleaseEvent(GXuint32 KeyValue, GXuint32 WindowID) : MouseButtonEvent(KeyValue,WindowID) {}
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_MOUSE_RELEASED; };
			virtual inline const char* getName() const override { return "MOUSE_RELEASED"; }

		};


		class GX_DLL MouseMotionEvent : public GXEvent {
		public:
			inline GXint32 getXVal() const { return x; }
			inline GXint32 getYVal() const { return y; }
			virtual inline GXuint32 getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE); }
			virtual inline const char* getName() const override { return "MOUSE_MOVED"; }
		protected:
			MouseMotionEvent(GXint32 X, GXint32 Y, GXuint32 WindowID) : x(X), y (Y),GXEvent(WindowID) {}
			virtual inline const char* toString() const override {
				std::stringstream ss;
				ss << this->getName() << "|X: " + std::to_string(x) + " |Y: " + std::to_string(y) + " |";
				return ss.str().c_str();
			}
			GXint32 x;
			GXint32 y;
		};

		class GX_DLL MouseMoveEvent : public MouseMotionEvent {

		public:
	
			MouseMoveEvent(GXint32 X, GXint32 Y, GXuint32 WindowID) : MouseMotionEvent(X,Y,WindowID) {}
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_MOUSE_MOVED; }
		};
		class GX_DLL MouseMoveRelEvent : public MouseMotionEvent {

		public:
			MouseMoveRelEvent(GXint32 XRel, GXint32 YRel, GXuint32 WindowID) : MouseMotionEvent(XRel, YRel,WindowID) {}
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_MOUSE_MOVED; }
		};

		class GX_DLL MouseScrollEvent : public GXEvent {

		public:
			MouseScrollEvent(GXint32 xOffset, GXint32 yOffset, GXuint32 WindowID) : xOff(xOffset), yOff(yOffset),GXEvent(WindowID) {}
			inline GXint32 getXOffset() const { return xOff; }
			inline GXint32 getYOffset() const { return yOff; }
			virtual inline GXuint32 getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE); }
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_MOUSE_SCROLL; }
			virtual inline const char* getName() const override { return "MOUSE_SCROLLED"; }
		private:
			GXint32 xOff;
			GXint32 yOff;
		};
	}

}