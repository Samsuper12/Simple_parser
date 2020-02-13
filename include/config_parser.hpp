#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>

class ConfigParser
{
public:
    ConfigParser() = default;
    ~ConfigParser();
    
    bool open(const std::string& path) noexcept;
    void findValue(const std::string& str) noexcept;
    void replace(const std::string& variable, const std::string& value) noexcept;
    void clear() noexcept;
    
    const std::vector<std::string>& getArrayRef(const std::string& str) const;
    std::vector<std::string> getArray(const std::string& str) const noexcept;

    static void createFile(const std::string& name, const std::string& data) noexcept;

    template<class... Args>
    void findValue(const std::string& str, Args&... args) noexcept {
        variables.push_back(str);
        findValue(args...);
    }

    const std::string& operator [] (const std::string& str) {
        return data[str][0];
    }
    
private:
    std::string filePath;
    std::vector<std::string> variables;
    std::map<std::string, std::vector<std::string> > data;

private:
    bool parseText(std::vector<std::string>& text) noexcept;
    void parseVariable(std::cmatch& match) noexcept;

    static void log(const char* comment, int line) noexcept;
};

#endif
