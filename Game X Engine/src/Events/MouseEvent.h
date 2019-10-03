#pragma once

#include "Event.h"
namespace gx {
	namespace event {
		class GX_DLL MouseButtonEvent : public GXEvent {
		public:

			inline uint32_t getKeyVal() const { return keyVal; }
			virtual inline uint32_t getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE | GXEventClass::GX_MOUSE_BUTTON); }

		protected:

			MouseButtonEvent(uint32_t KeyValue) : keyVal(KeyValue) {}
			virtual inline const char* toString()const override {
				std::stringstream ss;
				ss << KeyValToSTR(keyVal) + " " << this->getName();
				return ss.str().c_str();
			}
			uint32_t keyVal;

		};

		class GX_DLL MousePressEvent : public MouseButtonEvent {

		public:
			MousePressEvent(uint32_t KeyValue) : MouseButtonEvent(KeyValue) {}
			virtual inline  const char* getName() const override { return "MOUSE_PRESSED"; }

		};

		class GX_DLL MouseReleasedEvent : public MouseButtonEvent {

		public:
			MouseReleasedEvent(uint32_t KeyValue) : MouseButtonEvent(KeyValue) {}
			virtual inline const char* getName() const override { return "MOUSE_RELEASED"; }

		};


		class GX_DLL MouseMotionEvent : public GXEvent {
		public:
			inline int32_t getXVal() const { return x; }
			inline int32_t getYVal() const { return y; }
			virtual inline uint32_t getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE); }
			virtual inline const char* getName() const override { return "MOUSE_MOVED"; }
		protected:
			MouseMotionEvent(int32_t X, int32_t Y) : x(X), y (Y) {}
			virtual inline const char* toString() const override {
				std::stringstream ss;
				ss << this->getName() << "|X: " + std::to_string(x) + " |Y: " + std::to_string(y) + " |";
				return ss.str().c_str();
			}
			double x;
			double y;
		};

		class GX_DLL MouseMoveEvent : public MouseMotionEvent {

		public:
	
			MouseMoveEvent(int32_t X, int32_t Y) : MouseMotionEvent(X,Y) {}
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_MOUSE_MOVED; }
		};
		class GX_DLL MouseMoveRelEvent : public MouseMotionEvent {

		public:
			MouseMoveRelEvent(int32_t XRel, int32_t YRel) : MouseMotionEvent(XRel, YRel) {}
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_MOUSE_MOVED; }
		};

		class GX_DLL MouseScrollEvent : public GXEvent {

		public:
			MouseScrollEvent(int32_t xOffset, int32_t yOffset) : xOff(xOffset), yOff(yOffset) {}
			inline int32_t getXOffset() const { return xOff; }
			inline int32_t getYOffset() const { return yOff; }
			virtual inline uint32_t getEventClass() const override { return (GXEventClass::GX_INPUT | GXEventClass::GX_MOUSE); }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_MOUSE_SCROLL; }
			virtual inline const char* getName() const override { return "MOUSE_SCROLLED"; }
		private:
			double xOff;
			double yOff;
		};
	}

}