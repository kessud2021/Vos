#include "widget.hpp"
#include <algorithm>

namespace vos::gui {

// Widget
Widget::Widget(int32_t x, int32_t y, int32_t width, int32_t height)
    : rect_{x, y, width, height}, visible_(true), enabled_(true) {
}

void Widget::set_position(int32_t x, int32_t y) {
    rect_.x = x;
    rect_.y = y;
}

void Widget::set_size(int32_t width, int32_t height) {
    rect_.width = width;
    rect_.height = height;
}

void Widget::add_child(std::shared_ptr<Widget> child) {
    children_.push_back(child);
}

void Widget::remove_child(std::shared_ptr<Widget> child) {
    children_.erase(
        std::remove(children_.begin(), children_.end(), child),
        children_.end()
    );
}

bool Widget::point_in_rect(int32_t x, int32_t y) const {
    return x >= rect_.x && x < rect_.x + rect_.width &&
           y >= rect_.y && y < rect_.y + rect_.height;
}

// Button
Button::Button(int32_t x, int32_t y, int32_t width, int32_t height, const std::string& label)
    : Widget(x, y, width, height), label_(label), pressed_(false) {
}

void Button::draw() {
    // TODO: Implement button rendering
}

void Button::on_event(const WindowEvent& event) {
    if (event.type == WindowEventType::MOUSE_DOWN && point_in_rect(event.mouse.x, event.mouse.y)) {
        pressed_ = true;
    } else if (event.type == WindowEventType::MOUSE_UP && pressed_) {
        pressed_ = false;
        if (click_callback_) {
            click_callback_();
        }
    }
}

// Label
Label::Label(int32_t x, int32_t y, const std::string& text)
    : Widget(x, y, 100, 20), text_(text) {
}

void Label::draw() {
    // TODO: Implement label rendering
}

// TextBox
TextBox::TextBox(int32_t x, int32_t y, int32_t width, int32_t height)
    : Widget(x, y, width, height), cursor_pos_(0) {
}

void TextBox::draw() {
    // TODO: Implement textbox rendering
}

void TextBox::on_event(const WindowEvent& event) {
    if (event.type == WindowEventType::KEY_DOWN && visible_ && enabled_) {
        // TODO: Handle text input
    }
}

// Panel
Panel::Panel(int32_t x, int32_t y, int32_t width, int32_t height)
    : Widget(x, y, width, height) {
}

void Panel::draw() {
    // TODO: Implement panel rendering
    for (auto& child : children_) {
        if (child->is_visible()) {
            child->draw();
        }
    }
}

void Panel::on_event(const WindowEvent& event) {
    for (auto& child : children_) {
        if (child->is_enabled()) {
            child->on_event(event);
        }
    }
}

} // namespace vos::gui
