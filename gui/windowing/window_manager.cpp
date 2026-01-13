#include "window_manager.hpp"

namespace vos::gui {

WindowManager::WindowManager()
    : focused_window_(0), running_(true) {
}

WindowManager::~WindowManager() = default;

WindowManager& WindowManager::instance() {
    static WindowManager instance;
    return instance;
}

void WindowManager::register_window(uint32_t window_id, std::shared_ptr<Window> window) {
    if (window) {
        windows_[window_id] = window;
        if (focused_window_ == 0) {
            focused_window_ = window_id;
        }
    }
}

void WindowManager::destroy_window(uint32_t window_id) {
    auto it = windows_.find(window_id);
    if (it != windows_.end()) {
        windows_.erase(it);
        if (focused_window_ == window_id) {
            focused_window_ = 0;
        }
    }
}

std::shared_ptr<Window> WindowManager::get_window(uint32_t window_id) {
    auto it = windows_.find(window_id);
    if (it != windows_.end()) {
        return it->second;
    }
    return nullptr;
}

void WindowManager::post_event(uint32_t window_id, const WindowEvent& event) {
    event_queue_.push({window_id, event});
}

void WindowManager::process_events() {
    while (!event_queue_.empty()) {
        auto [window_id, event] = event_queue_.front();
        event_queue_.pop();
        
        auto window = get_window(window_id);
        if (window) {
            window->dispatch_event(event);
        }
    }
}

void WindowManager::pump_events() {
    process_events();
    // TODO: Poll platform-specific events (X11, Wayland, Win32, Cocoa)
}

void WindowManager::set_focus_window(uint32_t window_id) {
    if (windows_.find(window_id) != windows_.end()) {
        focused_window_ = window_id;
    }
}

} // namespace vos::gui
