#include "../apps/terminal_app.hpp"
#include "../windowing/window_manager.hpp"

using namespace vos::gui;

int main() {
    // Get window manager instance
    auto& wm = WindowManager::instance();
    
    // Create terminal application
    auto terminal = std::make_shared<TerminalApp>();
    wm.register_window(terminal->get_window_id(), terminal);
    terminal->show();
    
    // Main event loop
    while (wm.is_running()) {
        wm.pump_events();
        
        // Repaint all windows
        for (uint32_t i = 1; i < 100; ++i) {
            auto window = wm.get_window(i);
            if (window) {
                window->repaint();
            }
        }
    }
    
    return 0;
}
