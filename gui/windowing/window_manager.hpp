#pragma once

#include "window.hpp"
#include <unordered_map>
#include <memory>
#include <queue>

namespace vos::gui {

class WindowManager {
public:
    static WindowManager& instance();
    
    // Register an existing window
    void register_window(uint32_t window_id, std::shared_ptr<Window> window);
    void destroy_window(uint32_t window_id);
    std::shared_ptr<Window> get_window(uint32_t window_id);
    
    void process_events();
    void pump_events();
    bool is_running() const { return running_; }
    void quit() { running_ = false; }
    
    void post_event(uint32_t window_id, const WindowEvent& event);
    
    void set_focus_window(uint32_t window_id);
    uint32_t get_focus_window() const { return focused_window_; }

private:
    WindowManager();
    ~WindowManager();
    
    std::unordered_map<uint32_t, std::shared_ptr<Window>> windows_;
    std::queue<std::pair<uint32_t, WindowEvent>> event_queue_;
    uint32_t focused_window_;
    bool running_;
    
    // Prevent copy/move
    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
};

} // namespace vos::gui
