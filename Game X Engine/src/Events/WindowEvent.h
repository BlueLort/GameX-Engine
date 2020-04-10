#pragma once
#include "Event.h"
namespace gx {
	namespace event {
		class GX_DLL WindowEvent : public GXEvent {
		public:
			virtual inline GXuint32 getEventClass() const override { return (GXEventClass::GX_APPLICATION); }
		protected:
			WindowEvent(GXuint32 WindowID) : GXEvent(WindowID) {}

		};
		class GX_DLL WindowSizeEvent : public WindowEvent {//in SDL this event is followed by Resize event so i just check for resize.
		public:
	

			virtual inline const char* getName() const override { return "WINDOW_SIZE"; }
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_WINDOW_SIZE_CHANGED; }
			inline GXuint32 getWidth() const { return width; }
			inline GXuint32 getHeight() const { return height; }
			virtual inline const char* toString() const override
			{
				std::stringstream ss;
				ss << this->getName() << "| " << width << " , " << height << " |";
				return ss.str().c_str();
			}
		protected:
			WindowSizeEvent(GXuint32 Width, GXuint32 Height, GXuint32 WindowID)
				: width(Width), height(Height), WindowEvent(WindowID) {}
			GXuint32 width;
			GXuint32 height;

		};
		class GX_DLL WindowMoveEvent : public WindowEvent {//in SDL this event is followed by Resize event so i just check for resize.
		public:
			WindowMoveEvent(GXuint32 X, GXuint32 Y, GXuint32 WindowID)
				: x(X), y(Y),WindowEvent(WindowID) {}
			virtual inline const char* getName() const override { return "WINDOW_MOVED"; }
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_WINDOW_MOVED; }
			inline GXuint32 getXLoc() const { return x; }
			inline GXuint32 getYLoc() const { return y; }

			virtual inline const char* toString() const override
			{
				std::stringstream ss;
				ss << this->getName() << "| " << x << " , " << y << " |";
				return ss.str().c_str();
			}
		protected:
			GXuint32 x;
			GXuint32 y;

		};
		class GX_DLL WindowResizeEvent : public WindowSizeEvent {
		public:
			WindowResizeEvent(GXuint32 Width, GXuint32 Height, GXuint32 WindowID)
				: WindowSizeEvent(Width,Height,WindowID) {}
			virtual inline const char* getName() const override { return "WINDOW_RESIZE"; }
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_WINDOW_RESIZE; }
		};
		class GX_DLL WindowMaximizeEvent : public WindowSizeEvent {
		public:
			WindowMaximizeEvent(GXuint32 Width, GXuint32 Height ,GXuint32 WindowID)
				: WindowSizeEvent(Width, Height,WindowID) {}
			virtual inline const char* getName() const override { return "WINDOW_MAXIMIZE"; }
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_WINDOW_MAXIMIZE; }
		};
		class GX_DLL WindowMinimizeEvent : public WindowSizeEvent {
		public:
			WindowMinimizeEvent(GXuint32 Width, GXuint32 Height, GXuint32 WindowID)
				: WindowSizeEvent(Width, Height,WindowID) {}
			virtual inline const char* getName() const override { return "WINDOW_MINIMIZE"; }
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_WINDOW_MINIMIZE; }
		};
		class GX_DLL WindowCloseEvent : public WindowEvent {
		public:
			WindowCloseEvent(GXuint32 WindowID):WindowEvent(WindowID) {}
			virtual inline const char* getName() const override { return "WINDOW_CLOSE"; }
			virtual inline GXuint32 getEventClass() const override {return (GXEventClass::GX_APPLICATION);}
			virtual inline GXuint32 getEventType() const override { return GXEventType::GX_WINDOW_CLOSE; }
		};

	}

}