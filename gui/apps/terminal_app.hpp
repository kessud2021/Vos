#pragma once

#include "../windowing/window.hpp"
#include <string>
#include <deque>

namespace vos::gui {

class TerminalApp : public Window {
public:
    TerminalApp();
    
    void repaint() override;
    
private:
    std::deque<std::string> output_buffer_;
    std::string input_buffer_;
    uint32_t cursor_line_;
    uint32_t cursor_column_;
    
    static constexpr uint32_t COLS = 80;
    static constexpr uint32_t ROWS = 24;
};

} // namespace vos::gui
