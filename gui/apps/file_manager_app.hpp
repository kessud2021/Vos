#pragma once

#include "../windowing/window.hpp"
#include <string>
#include <vector>

namespace vos::gui {

struct FileEntry {
    std::string name;
    bool is_directory;
    uint64_t size;
};

class FileManagerApp : public Window {
public:
    FileManagerApp();
    
    void repaint() override;
    
    void navigate_to(const std::string& path);
    void refresh_directory_listing();

private:
    std::string current_path_;
    std::vector<FileEntry> entries_;
    int32_t selected_index_;
};

} // namespace vos::gui
