#include "file_manager_app.hpp"

namespace vos::gui {

FileManagerApp::FileManagerApp()
    : Window("File Manager", 1024, 768), selected_index_(0) {
    navigate_to("/");
}

void FileManagerApp::repaint() {
    // TODO: Implement file manager rendering
    // Draw directory listing
    // Draw selected item highlight
    // Draw navigation breadcrumb
}

void FileManagerApp::navigate_to(const std::string& path) {
    current_path_ = path;
    refresh_directory_listing();
}

void FileManagerApp::refresh_directory_listing() {
    entries_.clear();
    // TODO: Implement directory scanning using kernel filesystem API
    // entries_ should contain FileEntry objects for each file/folder
}

} // namespace vos::gui
