#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <utility>

struct JournalEntry {
    std::int64_t tick;
    std::string description;
};


class EventJournal {
    private:
    std::vector<JournalEntry> entries;

    public:
    void record(std::int64_t tick, const std::string& desc) {
        JournalEntry ent{tick, desc};
        entries.push_back(ent);
    }

    const std::vector<JournalEntry>& getEntries() const {
        return entries;
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);

        if (!file) {
            throw std::runtime_error("Не удалось открыть файл для записи");
        }
        for (const JournalEntry& entry : entries) {
            file << entry.tick << '\t' << entry.description << '\n';
        }
        if (!file) {
            throw std::runtime_error("Ошибка записи журнала в файл");
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);

        if (!file) {
            throw std::runtime_error("Не удалось открыть файл для чтения");
        }

        std::vector<JournalEntry> loadedEntries;
        std::string line;
        while (std::getline(file, line)) {
            std::size_t sep = line.find('\t');

            std::int64_t tick = std::stoll(line.substr(0, sep));
            std::string description = line.substr(sep+1);

            loadedEntries.push_back({tick, description});
        }
        if (file.bad()) {
            throw std::runtime_error("Ошибка чтения журнала");
        }
        entries = std::move(loadedEntries);
    }
};