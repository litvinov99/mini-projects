#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <functional>

namespace fs = std::filesystem;

class FileManager {
public:
    FileManager(const std::string& path) : currentPath(path) {}

    void listFiles() const {
        try {
            for (const auto& entry : fs::directory_iterator(currentPath)) {
                std::cout << (entry.is_directory() ? "[DIR] " : "      ") << entry.path().filename() << '\n';
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Ошибка: " << e.what() << '\n';
        }
    }

    void createFile(const std::string& filename) const {
        std::ofstream file(currentPath + "/" + filename);
        if (file) {
            std::cout << "Файл создан: " << filename << '\n';
        } else {
            std::cerr << "Ошибка создания файла" << '\n';
        }
    }

    void deleteFile(const std::string& filename) const {
        try {
            if (fs::remove(currentPath + "/" + filename)) {
                std::cout << "Файл удален: " << filename << '\n';
            } else {
                std::cerr << "Файл не найден" << '\n';
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Ошибка: " << e.what() << '\n';
        }
    }

    void copyFile(const std::string& source, const std::string& destination) const {
        try {
            fs::copy(currentPath + "/" + source, currentPath + "/" + destination, fs::copy_options::overwrite_existing);
            std::cout << "Файл скопирован: " << source << " -> " << destination << '\n';
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Ошибка копирования: " << e.what() << '\n';
        }
    }

    void moveFile(const std::string& source, const std::string& destination) const {
        try {
            fs::rename(currentPath + "/" + source, currentPath + "/" + destination);
            std::cout << "Файл перемещен: " << source << " -> " << destination << '\n';
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Ошибка перемещения: " << e.what() << '\n';
        }
    }

    void createDirectory(const std::string& dirname) const {
        try {
            if (fs::create_directory(currentPath + "/" + dirname)) {
                std::cout << "Папка создана: " << dirname << '\n';
            } else {
                std::cerr << "Ошибка создания папки" << '\n';
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Ошибка: " << e.what() << '\n';
        }
    }

    void removeDirectory(const std::string& dirname) const {
        try {
            if (fs::remove(currentPath + "/" + dirname)) {
                std::cout << "Папка удалена: " << dirname << '\n';
            } else {
                std::cerr << "Ошибка удаления папки" << '\n';
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Ошибка: " << e.what() << '\n';
        }
    }

    void readFile(const std::string& filename) const {
        std::ifstream file(currentPath + "/" + filename);
        if (!file) {
            std::cerr << "Ошибка открытия файла" << '\n';
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << '\n';
        }
    }

    void writeFile(const std::string& filename) const {
        std::ofstream file(currentPath + "/" + filename, std::ios::app);
        if (!file) {
            std::cerr << "Ошибка открытия файла" << '\n';
            return;
        }
        std::string text;
        std::cout << "Введите текст (Ctrl+D для завершения):" << '\n';
        std::cin.ignore();
        while (std::getline(std::cin, text)) {
            file << text << '\n';
        }
    }

    void printWorkingDirectory() const {
        std::cout << "Текущая директория: " << currentPath << '\n';
    }

private:
    std::string currentPath;
};

class CommandProcessor {
public:
    CommandProcessor(FileManager& fm) : fileManager(fm) {
        commands["ls"] = [&]() { fileManager.listFiles(); };
        commands["pwd"] = [&]() { fileManager.printWorkingDirectory(); };
        commands["exit"] = [&]() { exit(0); };
        commands["touch"] = [&]() {
            std::string filename;
            std::cin >> filename;
            fileManager.createFile(filename);
        };
        commands["rm"] = [&]() {
            std::string filename;
            std::cin >> filename;
            fileManager.deleteFile(filename);
        };
        commands["cp"] = [&]() {
            std::string source, destination;
            std::cin >> source >> destination;
            fileManager.copyFile(source, destination);
        };
        commands["mv"] = [&]() {
            std::string source, destination;
            std::cin >> source >> destination;
            fileManager.moveFile(source, destination);
        };
        commands["mkdir"] = [&]() {
            std::string dirname;
            std::cin >> dirname;
            fileManager.createDirectory(dirname);
        };
        commands["rmdir"] = [&]() {
            std::string dirname;
            std::cin >> dirname;
            fileManager.removeDirectory(dirname);
        };
        commands["cat"] = [&]() {
            std::string filename;
            std::cin >> filename;
            fileManager.readFile(filename);
        };
        commands["write"] = [&]() {
            std::string filename;
            std::cin >> filename;
            fileManager.writeFile(filename);
        };
    }

    void processCommands() {
        std::string command;
        while (true) {
            std::cout << "[#]> ";
            std::cin >> command;
            auto it = commands.find(command);
            if (it != commands.end()) {
                it->second();
            } else {
                std::cout << "Неизвестная команда. Доступные: ls, touch <файл>, rm <файл>, cp <файл> <новый_файл>, mv <файл> <новый_путь>, mkdir <папка>, rmdir <папка>, cat <файл>, write <файл>, pwd, exit" << '\n';
            }
        }
    }

private:
    FileManager& fileManager;
    std::unordered_map<std::string, std::function<void()>> commands;
};

int main() {
    FileManager fileManager(fs::current_path().string());
    CommandProcessor processor(fileManager);
    processor.processCommands();
    return 0;
}
