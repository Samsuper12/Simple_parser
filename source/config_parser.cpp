#include "config_parser.hpp"


ConfigParser::ConfigParser()
{
}

ConfigParser::~ConfigParser()
{
}

bool ConfigParser::open(const std::string& path) noexcept
{
    std::vector<std::string> lines;
    this->filePath = path;

    try {
        std::ifstream reader(path.c_str(), std::ifstream::in | std::ifstream::binary);
        std::string buf;

        if (!reader.is_open()) {
            log("Cannot open the file.", __LINE__);
            return false;
        }

        while (std::getline(reader, buf)) {
            if (!buf.empty()) {
                lines.push_back(buf);
            }
        }

        reader.close();

    } catch (const std::exception& e) {
        log(e.what(), __LINE__);
        return false;
    }

    if (lines.empty()) {
        log("Zero lines found.", __LINE__);
        return false;
    }

    if (!parseText(lines)) {
        return false;
    }

    return true;
}

void ConfigParser::findValue(const std::string &str) noexcept
{
    variables.push_back(str);
}

void ConfigParser::replace(const std::string &variable, const std::string &value) noexcept
{
    std::ifstream readStream(this->filePath, std::ifstream::in | std::ifstream::binary);
    std::ofstream writeStream;
    std::vector<std::string> stack;
    std::string buf;

    if (!readStream.is_open()) {
         log("Read stream is not open.", __LINE__);
        return;
    }

    while (std::getline(readStream, buf)) {
        stack.push_back(buf);
    }

    for (auto itt = stack.begin(); itt != stack.end(); ++itt) {
        if (((*itt).find(variable)+1) > 0) {
            *itt = variable + '=' + value;
            readStream.close();

            writeStream.open(this->filePath, std::ostream::out | std::ostream::trunc);

            if (!writeStream.is_open()) {
                log("Write stream is not open.", __LINE__);
                return;
            }

            for (auto itt = stack.begin(); itt != stack.end(); ++itt) {
                writeStream << *itt << std::endl;
            }

            writeStream.close();
            return;
        }
    }
}

void ConfigParser::clear() noexcept 
{
    data.clear();
}

const std::vector<std::string>& ConfigParser::getArrayRef(const std::string& str) const
{
    try {
        return data.at(str);
    } catch(const std::exception& e) {
        log(e.what(), __LINE__);
        throw std::exception();
    }
}

std::vector<std::string> ConfigParser::getArray(const std::string& str) const noexcept
{
    try {
        return data.at(str);
    } catch(const std::exception& e) {
        log(e.what(), __LINE__);
        return std::vector<std::string>();
    }
}

void ConfigParser::createFile(const std::string &name, const std::string &data) noexcept
{
    std::ofstream writeStream;

    writeStream.open(name, std::ostream::out | std::ostream::trunc);

    if (!writeStream.is_open()) {
        log("Write stream is not open.", __LINE__);
        return;
    }

    writeStream << data;
    writeStream.close();
}

bool ConfigParser::parseText(std::vector<std::string>& text) noexcept
{
    try {
        std::regex regular("^([^\\#].+?\\S)(?:\\[)?(?:\\])?(?:\\=)+?(?:\")?(.+\\b)");
        std::cmatch result;

        for (auto itt = text.cbegin(); itt != text.cend(); ++itt) {
            if (std::regex_search(itt->c_str(), result, regular)) {
                parseVariable(result);
            }
        }

    } catch (const std::regex_error& e) {
        log("Regex error.", __LINE__);
        return false;
    }

    return true;
}

void ConfigParser::parseVariable(std::cmatch &match) noexcept
{
  auto it = std::find(variables.cbegin(), variables.cend(), match[1]);

  if (it != variables.cend()) {
      data[*it].push_back(match[2]);
  }
}

void ConfigParser::log(const char *comment, const int line) noexcept
{
    std::cout << line << "\t"
              << "ConfigParser: " << comment
              << "\n" << std::flush;
}
