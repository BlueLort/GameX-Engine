#pragma once

#include "Event.h"
namespace gx {
	namespace event {
		class GX_DLL MouseButtonEvent : public GXEvent {
		public:

			inline uint32_t getKeyVal() const { return keyVal; }
			virtual inline uint32_t getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE | GXEventClass::GX_MOUSE_BUTTON); }
			virtual inline const char* toString()const override {
				std::stringstream ss;
				ss << KeyValToSTR(keyVal) + " " << this->getName();
				return ss.str().c_str();
			}
		protected:

			MouseButtonEvent(uint32_t KeyValue, uint32_t WindowID) : keyVal(KeyValue) , GXEvent(WindowID) {}
		
			uint32_t keyVal;

		};

		class GX_DLL MousePressEvent : public MouseButtonEvent {

		public:
			MousePressEvent(uint32_t KeyValue, uint32_t WindowID) : MouseButtonEvent(KeyValue,WindowID) {}
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_MOUSE_PRESSED; };
			virtual inline  const char* getName() const override { return "MOUSE_PRESSED"; }

		};

		class GX_DLL MouseReleaseEvent : public MouseButtonEvent {

		public:
			MouseReleaseEvent(uint32_t KeyValue, uint32_t WindowID) : MouseButtonEvent(KeyValue,WindowID) {}
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_MOUSE_RELEASED; };
			virtual inline const char* getName() const override { return "MOUSE_RELEASED"; }

		};


		class GX_DLL MouseMotionEvent : public GXEvent {
		public:
			inline int32_t getXVal() const { return x; }
			inline int32_t getYVal() const { return y; }
			virtual inline uint32_t getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE); }
			virtual inline const char* getName() const override { return "MOUSE_MOVED"; }
		protected:
			MouseMotionEvent(int32_t X, int32_t Y, uint32_t WindowID) : x(X), y (Y),GXEvent(WindowID) {}
			virtual inline const char* toString() const override {
				std::stringstream ss;
				ss << this->getName() << "|X: " + std::to_string(x) + " |Y: " + std::to_string(y) + " |";
				return ss.str().c_str();
			}
			int32_t x;
			int32_t y;
		};

		class GX_DLL MouseMoveEvent : public MouseMotionEvent {

		public:
	
			MouseMoveEvent(int32_t X, int32_t Y, uint32_t WindowID) : MouseMotionEvent(X,Y,WindowID) {}
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_MOUSE_MOVED; }
		};
		class GX_DLL MouseMoveRelEvent : public MouseMotionEvent {

		public:
			MouseMoveRelEvent(int32_t XRel, int32_t YRel, uint32_t WindowID) : MouseMotionEvent(XRel, YRel,WindowID) {}
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_MOUSE_MOVED; }
		};

		class GX_DLL MouseScrollEvent : public GXEvent {

		public:
			MouseScrollEvent(int32_t xOffset, int32_t yOffset, uint32_t WindowID) : xOff(xOffset), yOff(yOffset),GXEvent(WindowID) {}
			inline uint32_t getXOffset() const { return xOff; }
			inline uint32_t getYOffset() const { return yOff; }
			virtual inline uint32_t getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE); }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_MOUSE_SCROLL; }
			virtual inline const char* getName() const override { return "MOUSE_SCROLLED"; }
		private:
			uint32_t xOff;
			uint32_t yOff;
		};
	}

}