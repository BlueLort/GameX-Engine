#pragma once
#include "Event.h"
namespace gx {
	namespace event {
		class GX_DLL WindowSizeEvent : public GXEvent {//in SDL this event is followed by Resize event so i just check for resize.
		public:
			WindowSizeEvent(uint32_t Width, uint32_t Height)
				: width(Width), height(Height) {}
			virtual inline uint32_t getEventClass() const override { return (GXEventClass::GX_APPLICATION); }
			virtual inline const char* getName() const override { return "WINDOW_SIZE"; }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_WINDOW_SIZE_CHANGED; }
			inline uint32_t getWidth() const { return width; }
			inline uint32_t getHeight() const { return height; }

			virtual inline const char* toString() const override
			{
				std::stringstream ss;
				ss << this->getName() << "| " << width << " , " << height << " |";
				return ss.str().c_str();
			}
		protected:
			uint32_t width;
			uint32_t height;

		};
		class GX_DLL WindowResizeEvent : public WindowSizeEvent {
		public:
			WindowResizeEvent(uint32_t Width, uint32_t Height)
				: WindowSizeEvent(Width,Height) {}
			virtual inline const char* getName() const override { return "WINDOW_RESIZE"; }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_WINDOW_RESIZE; }
		};
		class GX_DLL WindowMaximizeEvent : public WindowSizeEvent {
		public:
			WindowMaximizeEvent(uint32_t Width, uint32_t Height)
				: WindowSizeEvent(Width, Height) {}
			virtual inline const char* getName() const override { return "WINDOW_MAXIMIZE"; }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_WINDOW_MAXIMIZE; }
		};
		class GX_DLL WindowMinimizeEvent : public WindowSizeEvent {
		public:
			WindowMinimizeEvent(uint32_t Width, uint32_t Height)
				: WindowSizeEvent(Width, Height) {}
			virtual inline const char* getName() const override { return "WINDOW_MINIMIZE"; }
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_WINDOW_MINIMIZE; }
		};
		class GX_DLL WindowCloseEvent : public GXEvent {
		public:
			WindowCloseEvent() {}
			virtual inline const char* getName() const override { return "WINDOW_CLOSE"; }
			virtual inline uint32_t getEventClass() const override {return (GXEventClass::GX_APPLICATION);}
			virtual inline uint32_t getEventType() const override { return GXEventType::GX_WINDOW_CLOSE; }
		};

	}

}