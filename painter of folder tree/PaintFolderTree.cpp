#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <algorithm>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

void PrintFolderTree(ostream& dst, const path& p, const filesystem::file_status& status, int offset) {
    string folder_name = p.filename().string();
    dst << string(offset, ' ') << folder_name << endl;

    if (status.type() == filesystem::file_type::directory) {
        vector<filesystem::directory_entry> objects;
        for (const auto& dir_entry : filesystem::directory_iterator(p)) {
            objects.push_back(dir_entry);
        }
        sort(objects.begin(), objects.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.path().filename() > rhs.path().filename();
            });
            
        for (const auto& dir_entry : objects) {
            PrintFolderTree(dst, dir_entry.path(), dir_entry.status(), offset + 2);
        }
    }
}

void PrintFolderTree(ostream& dst, const path& p) {
    error_code err;
    auto status = filesystem::status(p, err);
    if (err) {
        return;
    }
    PrintFolderTree(dst, p, status, 0); 
}

void TestPrintingFolderTree() {

}

int main() {
    error_code err;
    filesystem::remove_all("test_dir", err);
    filesystem::create_directories("test_dir"_p / "folder_a"_p, err);
    filesystem::create_directories("test_dir"_p / "folder_b"_p, err);

    ofstream("test_dir"_p / "folder_b"_p / "file_1.txt"_p);
    ofstream("test_dir"_p / "folder_b"_p / "file_2.txt"_p);
    ofstream("test_dir"_p / "folder_a"_p / "file_3.txt"_p);

    PrintFolderTree(std::cout, "test_dir"_p);

    ostringstream out;
    PrintFolderTree(out, "test_dir"_p);
    assert(out.str() ==
        "test_dir\n"
        "  folder_b\n"
        "    file_2.txt\n"
        "    file_1.txt\n"
        "  folder_a\n"
        "    file_3.txt\n"s
    );
    
}