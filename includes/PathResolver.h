#pragma once

#include <iostream>
#include <filesystem>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

class PathResolver {
public:
    // Retrieve the path to the executable
    static fs::path getExecutablePath() {
        try {
#ifdef _WIN32
            char buffer[MAX_PATH];
            if (GetModuleFileNameA(nullptr, buffer, MAX_PATH) == 0) {
                throw std::runtime_error("Unable to get executable path");
            }
            return fs::canonical(buffer);
#else
            return fs::canonical("/proc/self/exe");
#endif
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error retrieving executable path: " + std::string(e.what()));
        }
    }

    // Resolve a relative path to an absolute path based on the executable's location
    static fs::path resolvePath(const std::string& relativePath) {
        try {
            fs::path execPath = getExecutablePath().parent_path(); // Get the directory of the executable
            fs::path fullPath = execPath / relativePath; // Append the relative path
            return fs::canonical(fullPath); // Resolve and canonicalize the full path
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error resolving path: " + std::string(e.what()));
        }
    }
};