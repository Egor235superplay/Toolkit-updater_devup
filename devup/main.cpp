#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <array>
#include <iomanip>

const std::string RESET   = "\033[0m";
const std::string RED     = "\033[31m";
const std::string GREEN   = "\033[32m";
const std::string YELLOW  = "\033[33m";
const std::string CYAN    = "\033[36m";
const std::string BOLD    = "\033[1m";

struct SoftwareInfo {
    std::string name;
    std::string current_version;
    std::string latest_version;
    std::string status;
    std::string color;
};

std::string executeCommand(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("Ошибка popen()");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main() {
    std::cout << BOLD << CYAN << "=== Linux Software Version Visualizer ===" << RESET << "\n";
    std::cout << "Сканирование системы, пожалуйста подождите...\n\n";

    std::string script_output = executeCommand("./scanner.sh");
    std::vector<SoftwareInfo> app_list;
    std::stringstream ss(script_output);
    std::string line;

    while (std::getline(ss, line)) {
        if (line.empty()) continue;
        std::stringstream line_stream(line);
        std::string name, current, latest;
        std::getline(line_stream, name, ';');
        std::getline(line_stream, current, ';');
        std::getline(line_stream, latest, ';');

        SoftwareInfo info{name, current, latest, "", ""};
        if (current == "not_installed") { info.status = "Missing"; info.color = YELLOW; }
        else if (current == "unknown") { info.status = "Unknown"; info.color = YELLOW; }
        else if (current < latest) { info.status = "Outdated"; info.color = RED; }
        else { info.status = "Up to date"; info.color = GREEN; }
        app_list.push_back(info);
    }

    std::cout << std::left << std::setw(15) << (BOLD + "Программа" + RESET)
              << std::setw(18) << (BOLD + "Текущая версия" + RESET)
              << std::setw(18) << (BOLD + "Актуальная" + RESET)
              << (BOLD + "Статус" + RESET) << "\n";
    std::cout << std::string(60, '-') << "\n";

    for (const auto& app : app_list) {
        std::cout << std::left << std::setw(15) << app.name
                  << std::setw(18) << app.current_version
                  << std::setw(18) << app.latest_version
                  << app.color << app.status << RESET << "\n";
    }
    std::cout << "\n" << BOLD << CYAN << "Проверка завершена успешно!" << RESET << "\n";
    return 0;
}