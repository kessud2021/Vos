#pragma once

#include "../windowing/window.hpp"
#include <vector>
#include <memory>
#include <string>

namespace vos::gui {

class Widget {
public:
    Widget(int32_t x, int32_t y, int32_t width, int32_t height);
    virtual ~Widget() = default;

    virtual void draw() = 0;
    virtual void on_event(const WindowEvent& event) = 0;
    
    void set_position(int32_t x, int32_t y);
    void set_size(int32_t width, int32_t height);
    void set_visible(bool visible) { visible_ = visible; }
    void set_enabled(bool enabled) { enabled_ = enabled; }
    
    int32_t get_x() const { return rect_.x; }
    int32_t get_y() const { return rect_.y; }
    int32_t get_width() const { return rect_.width; }
    int32_t get_height() const { return rect_.height; }
    bool is_visible() const { return visible_; }
    bool is_enabled() const { return enabled_; }
    
    void add_child(std::shared_ptr<Widget> child);
    void remove_child(std::shared_ptr<Widget> child);
    
    bool point_in_rect(int32_t x, int32_t y) const;

protected:
    Rect rect_;
    bool visible_;
    bool enabled_;
    std::vector<std::shared_ptr<Widget>> children_;
};

class Button : public Widget {
public:
    Button(int32_t x, int32_t y, int32_t width, int32_t height, const std::string& label);
    
    void draw() override;
    void on_event(const WindowEvent& event) override;
    
    void set_label(const std::string& label) { label_ = label; }
    const std::string& get_label() const { return label_; }
    
    using ClickCallback = std::function<void()>;
    void on_click(ClickCallback callback) { click_callback_ = callback; }

private:
    std::string label_;
    bool pressed_;
    ClickCallback click_callback_;
};

class Label : public Widget {
public:
    Label(int32_t x, int32_t y, const std::string& text);
    
    void draw() override;
    
    void set_text(const std::string& text) { text_ = text; }
    const std::string& get_text() const { return text_; }

private:
    std::string text_;
};

class TextBox : public Widget {
public:
    TextBox(int32_t x, int32_t y, int32_t width, int32_t height);
    
    void draw() override;
    void on_event(const WindowEvent& event) override;
    
    void set_text(const std::string& text) { text_ = text; }
    const std::string& get_text() const { return text_; }

private:
    std::string text_;
    uint32_t cursor_pos_;
};

class Panel : public Widget {
public:
    Panel(int32_t x, int32_t y, int32_t width, int32_t height);
    
    void draw() override;
    void on_event(const WindowEvent& event) override;
};

} // namespace vos::gui
