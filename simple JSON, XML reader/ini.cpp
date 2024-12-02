#include "ini.h"

ini::Section& ini::Document::AddSection(std::string name) {
    return (sections_[name]);
}

const ini::Section& ini::Document::GetSection(const std::string& name) const {
    static Section static_empty;
    if (sections_.find(name) == sections_.end()) {
        return static_empty;
    }
    return (sections_.at(name));
}

std::size_t ini::Document::GetSectionCount() const {
    return (sections_.size());
}

ini::Document ini::Load(std::istream& input) {
    ini::Document sections;
    ini::Section* section = nullptr;
    std::string line;

    while(getline(input, line)) {
        if (line.empty()) {
            continue;
        }
        auto pos = line.find_first_not_of(' ', 0);
        line = line.substr(pos);
        if (line[0] == '[') {
            std::string section_name = line.substr(1, line.find(']') - 1);
            section = &sections.AddSection(section_name);
            continue;
        } else {
            auto pos = line.find('=');
            std::string unit_key = line.substr(line.find_first_not_of(' ', 0), line.find_last_not_of(' ', pos - 1) + 1);
            line = line.substr(line.find_first_not_of(' ', pos + 1));
            std::string unit_value = line.substr(0, line.find_last_not_of(' ') + 1);
            section->insert({unit_key, unit_value});

        }
    }

    return (sections);
}