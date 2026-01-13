#include "window.hpp"

namespace vos::gui {

uint32_t Window::next_window_id_ = 1;

Window::Window(const std::string& title, int32_t width, int32_t height)
    : title_(title),
      position_{0, 0},
      size_{width, height},
      visible_(false),
      window_id_(next_window_id_++) {
}

Window::~Window() = default;

void Window::set_position(int32_t x, int32_t y) {
    position_ = {x, y};
    WindowEvent event;
    event.type = WindowEventType::MOVE;
    event.position = position_;
    dispatch_event(event);
}

void Window::set_size(int32_t width, int32_t height) {
    size_ = {width, height};
    WindowEvent event;
    event.type = WindowEventType::RESIZE;
    event.size = size_;
    dispatch_event(event);
}

void Window::set_title(const std::string& title) {
    title_ = title;
}

void Window::show() {
    visible_ = true;
}

void Window::hide() {
    visible_ = false;
}

void Window::close() {
    visible_ = false;
    WindowEvent event;
    event.type = WindowEventType::CLOSE;
    dispatch_event(event);
}

void Window::dispatch_event(const WindowEvent& event) {
    for (auto& callback : event_callbacks_) {
        callback(event);
    }
}

} // namespace vos::gui
