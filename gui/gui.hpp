#pragma once

// Main GUI library header - includes all major components

// Windowing
#include "windowing/window.hpp"
#include "windowing/window_manager.hpp"

// Toolkit
#include "toolkit/widget.hpp"

// Applications
#include "apps/terminal_app.hpp"
#include "apps/file_manager_app.hpp"

namespace vos::gui {
    // Convenience functions for common operations
    
    inline void create_and_register_window(uint32_t window_id, 
                                           std::shared_ptr<Window> window) {
        WindowManager::instance().register_window(window_id, window);
    }
    
    inline std::shared_ptr<Window> get_window(uint32_t window_id) {
        return WindowManager::instance().get_window(window_id);
    }
    
    inline void run_event_loop() {
        auto& wm = WindowManager::instance();
        while (wm.is_running()) {
            wm.pump_events();
        }
    }
}
