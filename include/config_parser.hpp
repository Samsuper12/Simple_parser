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
    ConfigParser();
    ~ConfigParser();

    bool open(const std::string& path);
    void findValue(const std::string& str);
    void replace(const std::string& variable, const std::string& value);
    void clear();

    static void createFile(const std::string& name, const std::string& data);

    template<class... Args>
    void findValue(const std::string& str, Args&... args) {
        variables.push_back(str);
        findValue(args...);
    }

    const std::string& operator [] (const std::string& str) {
        return data[str];
    }

private:
    std::string filePath;
    std::vector<std::string> variables;
    std::map<std::string, std::string> data;

private:
    bool parseText(std::vector<std::string>& text);
    void parseVariable(std::cmatch& match);

    static void log(const char* comment, const int line);
};

#endif
