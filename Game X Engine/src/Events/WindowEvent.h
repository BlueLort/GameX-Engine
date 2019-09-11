#pragma once
#include "Event.h"
namespace gx {
	namespace event {
		class GX_DLL WindowResizeEvent : public Event {
		public:
			WindowResizeEvent(uint32_t Width, uint32_t Height)
				: width(Width), height(Height) {}
			virtual const char* getName() const override { return "WINDOW_RESIZE"; }
			virtual inline uint32_t getEventClass() const override { return (EventClass::GX_APPLICATION); }
			virtual uint32_t getEventType() const override { return EventType::GX_WINDOW_RESIZE; }
			inline uint32_t getWidth() const { return width; }
			inline uint32_t getHeight() const { return height; }

			virtual const char* toString() const override
			{
				std::stringstream ss;
				ss << this->getName() << "| " << width << " , " << height << " |";
				return ss.str().c_str();
			}
		private:
			uint32_t width;
			uint32_t height;

		};

		class GX_DLL WindowCloseEvent : public Event {
		public:
			WindowCloseEvent() {}
			virtual const char* getName() const override { return "WINDOW_CLOSE"; }
			virtual inline uint32_t getEventClass() const override {return (EventClass::GX_APPLICATION);}
			virtual uint32_t getEventType() const override { return EventType::GX_WINDOW_CLOSE; }
		};

	}

}