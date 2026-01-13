# VOS GUI Library (Phase 3)

Modern C++ GUI framework for the VOS operating system.

## Architecture

### Windowing System (`windowing/`)
- **Window**: Abstract base class for all window types
- **WindowManager**: Singleton managing window lifecycle and events
- Event routing and dispatch system
- Support for multiple window types

### Widget Toolkit (`toolkit/`)
- **Widget**: Base class for UI components
- **Button**: Clickable button with callbacks
- **Label**: Text display component
- **TextBox**: Text input component
- **Panel**: Container for layout and child widgets

### Applications (`apps/`)
- **TerminalApp**: VOS terminal emulator
- **FileManagerApp**: File system browser

## Features

- **Event-driven architecture**: Mouse, keyboard, window events
- **Hierarchical widget system**: Parent-child widget relationships
- **Callback-based interaction**: Functional event handling
- **Lazy rendering**: On-demand repaint optimization
- **Extensible design**: Easy to add new widgets and applications

## Build

```bash
cd build
cmake .. -G Ninja
ninja libgui
```

## Usage Example

```cpp
#include "apps/terminal_app.hpp"
#include "windowing/window_manager.hpp"

int main() {
    auto& wm = vos::gui::WindowManager::instance();
    
    auto terminal = std::make_shared<vos::gui::TerminalApp>();
    wm.register_window(terminal->get_window_id(), terminal);
    terminal->show();
    
    while (wm.is_running()) {
        wm.pump_events();
        terminal->repaint();
    }
    
    return 0;
}
```

## Phase 3 Roadmap

### Year 5-6 (GUI + Desktop)

**Graphics & Windowing**
- [ ] Framebuffer driver
- [ ] GPU driver (NVIDIA/AMD basic support)
- [ ] X11 server implementation
- [ ] Wayland compositor
- [x] Window manager (basic)
- [x] Event system (mouse, keyboard, touch)

**Widget Toolkit**
- [x] Basic widgets (button, textbox, label, listbox)
- [ ] Layout managers
- [ ] Theme system
- [ ] Accessibility (keyboard navigation, screen readers)

**Desktop Applications**
- [x] Terminal emulator (stub)
- [x] File manager (stub)
- [ ] Text editor (gedit-like)
- [ ] System monitor
- [ ] Calculator
- [ ] Image viewer
- [ ] Web browser (lightweight)

**Desktop Environment**
- [ ] Panel/taskbar
- [ ] Application menu
- [ ] Settings/control panel
- [ ] Notification system
- [ ] Power management

## Standards

- **C++20** with modern patterns
- **Header-only where possible** for template code
- **STL containers** for data structures
- **Smart pointers** for memory management
- **Abstract base classes** for extensibility

## Future Work

1. **Rendering Engine**: Hardware-accelerated graphics with OpenGL/Vulkan
2. **Layout System**: Flex/Grid layouts for responsive design
3. **Theme Engine**: Customizable colors, fonts, icons
4. **Input Methods**: IME support, accessibility features
5. **Platform Integration**: X11, Wayland, Windows, macOS backends
