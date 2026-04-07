#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct Sample {
    std::string timestamp;
    double value;
};

std::vector<Sample> readCsv(const std::string& filename) {
    std::vector<Sample> data;
    std::ifstream file(filename);
    if (!file.is_open()) return data;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string timestamp;
        std::string value;

        std::getline(ss, timestamp, ',');
        std::getline(ss, value, ',');

        if (!timestamp.empty() && !value.empty()) {
            data.push_back({timestamp, std::stod(value)});
        }
    }

    return data;
}

double movingAverage(const std::vector<double>& values, size_t window) {
    if (values.empty()) return 0.0;
    const size_t start = values.size() > window ? values.size() - window : 0;
    double sum = 0.0;
    for (size_t i = start; i < values.size(); ++i) sum += values[i];
    return sum / static_cast<double>(values.size() - start);
}

int main() {
    std::cout << "Sistema de Analise\n\n";

    const auto data = readCsv("sample_data.csv");
    if (data.empty()) {
        std::cout << "Nao foi possivel ler o arquivo sample_data.csv\n";
        return 1;
    }

    std::vector<double> history;
    for (const auto& item : data) {
        history.push_back(item.value);
        const double avg3 = movingAverage(history, 3);
        const double avg5 = movingAverage(history, 5);

        std::cout << "Momento: " << item.timestamp
                  << " | Valor: " << std::fixed << std::setprecision(2) << item.value
                  << " | Media(3): " << avg3
                  << " | Media(5): " << avg5 << '\n';

        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }

    return 0;
}
