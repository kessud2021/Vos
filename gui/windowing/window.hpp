#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

namespace vos::gui {

enum class WindowEventType {
    CLOSE,
    RESIZE,
    MOVE,
    PAINT,
    MOUSE_DOWN,
    MOUSE_UP,
    MOUSE_MOVE,
    KEY_DOWN,
    KEY_UP,
    FOCUS_IN,
    FOCUS_OUT
};

struct Point {
    int32_t x, y;
};

struct Size {
    int32_t width, height;
};

struct Rect {
    int32_t x, y, width, height;
};

struct MouseEvent {
    int32_t x, y;
    uint8_t button;
    bool is_pressed;
};

struct KeyEvent {
    uint32_t keycode;
    uint32_t modifiers;
};

struct WindowEvent {
    WindowEventType type;
    union {
        MouseEvent mouse;
        KeyEvent key;
        Size size;
        Point position;
    };
};

using EventCallback = std::function<void(const WindowEvent&)>;

class Window {
public:
    Window(const std::string& title, int32_t width, int32_t height);
    virtual ~Window();

    void set_position(int32_t x, int32_t y);
    void set_size(int32_t width, int32_t height);
    void set_title(const std::string& title);
    
    void show();
    void hide();
    void close();
    
    int32_t get_width() const { return size_.width; }
    int32_t get_height() const { return size_.height; }
    int32_t get_x() const { return position_.x; }
    int32_t get_y() const { return position_.y; }
    const std::string& get_title() const { return title_; }
    uint32_t get_window_id() const { return window_id_; }
    
    void on_event(EventCallback callback) { event_callbacks_.push_back(callback); }
    void dispatch_event(const WindowEvent& event);
    
    virtual void repaint() = 0;

protected:
    std::string title_;
    Point position_;
    Size size_;
    bool visible_;
    std::vector<EventCallback> event_callbacks_;
    uint32_t window_id_;
    
    static uint32_t next_window_id_;
};

} // namespace vos::gui
