#include "terminal_app.hpp"

namespace vos::gui {

TerminalApp::TerminalApp()
    : Window("Terminal", 800, 600), cursor_line_(0), cursor_column_(0) {
    // Initialize terminal buffer
}

void TerminalApp::repaint() {
    // TODO: Implement terminal rendering
    // Draw output_buffer_ content
    // Draw cursor
}

} // namespace vos::gui
