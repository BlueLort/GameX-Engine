#include <pch.h>
#include <Config.h>

namespace gx {
	namespace event {
		enum EventType
		{
			GX_NONE = 0,
			GX_KEY_PRESSED,
			GX_KEY_RELEASED,
			GX_KEY_TYPED,
			GX_MOUSE_PRESSED,
			GX_MOUSE_RELEASED,
			GX_MOUSE_MOVED,
			GX_MOUSE_SCROLL,
			GX_WINDOW_RESIZE,
			GX_WINDOW_CLOSE
		};
		enum EventClass {
			GX_NO_CLASS = 0,
			GX_APPLICATION = 1,
			GX_INPUT = 1 << 1,
			GX_KEYBOARD = 1 << 2,
			GX_MOUSE = 1 << 3,
			GX_MOUSE_BUTTON = 1 << 4
		};
		class GX_DLL Event
		{
		public:
			bool handled = false;
			virtual const char* getName() const = 0;
			virtual uint32_t getEventClass() const = 0;
			inline bool isSameClassAs(const Event& e) {
				return this->getEventClass() & e.getEventClass();
			}

			virtual const char* toString() { return this->getName(); }
		};
		constexpr uint32_t EVENTS_LIMIT = 64;
		class GX_DLL EventQueue {
		public:
			static bool enqueue(const Event& e) {
				if (size == EVENTS_LIMIT)return false;
				EventBuffer[backIte] = std::make_unique<Event>(e);
				backIte = (backIte + 1) % EVENTS_LIMIT;
				size++;
				return true;
			}
			static bool processEvent() {
				if (size == 0)return false;
				//TODO EVENT DISPATCHER TO DISPATCH EVENTS
				EventBuffer[frontIte]->handled = true;
				size--;
				frontIte = (frontIte + 1) % EVENTS_LIMIT;
				return true;

			}
			inline static uint32_t getSize(){ return size; }

		private:
			static std::vector<std::unique_ptr<Event>> EventBuffer;
			static uint32_t frontIte;
			static uint32_t backIte;
			static uint32_t size;

		};
		std::vector<std::unique_ptr<Event> > EventQueue::EventBuffer(EVENTS_LIMIT);
		uint32_t EventQueue::frontIte = 0;
		uint32_t EventQueue::backIte = 0;
		uint32_t EventQueue::size = 0;
	}

}