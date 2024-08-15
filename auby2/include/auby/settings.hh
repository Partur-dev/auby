#pragma once

#include "auby/auby.hh"
#include "spdlog/spdlog.h"
#include <cstdio>
#include <fstream>
#include <json.hpp>
#include <optional>
#include <string>
#include <unistd.h>

namespace auby {
struct Settings {
    static Settings* shared();

    template <class T>
    std::optional<T> get(std::string key) {
        if (j.contains(key))
            return j[key].get<T>();
        return std::nullopt;
    }

    template <class T>
    void set(std::string key, T value) {
        j[key] = value;
        save();
    }

private:
    Settings() {
        path = auby::getDocumentsDir() + "/auby.json";
        read();
    }

    ~Settings() {
        fclose(f);
    }

    void read() {
        f = fopen(path.c_str(), "r");
        if (!f) {
            f = fopen(path.c_str(), "w");
            fclose(f);
            f = fopen(path.c_str(), "r");
        }

        fseek(f, 0, SEEK_END);
        long fileSize = ftell(f);
        fseek(f, 0, SEEK_SET);

        std::string content(fileSize, '\0');
        fread(&content[0], 1, fileSize, f);

        if (content == "") {
            j = nlohmann::json {};
        } else {
            j = nlohmann::json::parse(content);
        }
    }

    void save() {
        auto s = j.dump();
        freopen(path.c_str(), "w+", f);
        fseek(f, 0, 0);
        fwrite(s.c_str(), s.length(), 1, f);
        fflush(f);
    }

    std::string path;
    FILE* f;
    nlohmann::json j;
};

template <class T>
struct SettingsValue {
    SettingsValue(std::string key, T default_value) : m_key(key), m_default(default_value) {}

    T operator()() {
        auto v = Settings::shared()->get<T>(m_key);
        if (v.has_value()) {
            spdlog::debug("Settings: {} = {}", m_key, v.value());
            return v.value();
        } else {
            spdlog::debug("Settings: {} = {} (default)", m_key, m_default);
            return m_default;
        }
    }

    void operator=(T value) {
        spdlog::debug("Settings: {} => {}", m_key, value);
        Settings::shared()->set(m_key, value);
    }

private:
    std::string m_key;
    T m_default;
};
} // namespace auby
