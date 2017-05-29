#ifndef FORMATTER_H
#define FORMATTER_H

#include <string>
#include <fstream>
#include <map>
#include <cctype>

class Formatter
{
public:
    Formatter(std::string filePath);
    virtual ~Formatter();

    std::map<std::string, int>& getMap() { return m_map; }

    bool exec(std::string replaceValue);

    static std::map<std::string, int> genereWordMap(std::string& data);
    static void formatLigne(std::string& ligne);

private:
    std::ifstream m_fileFlux;
    std::string m_data;
    std::map<std::string, int> m_map;

    static void filtreGuillemet(std::string& ligne, std::string defaultValue);
    static bool findReplace(std::string& ligne, std::string const& val, std::string const& newVal);

    static bool isEmpty(std::string ligne);
    static void cleanLigne(std::string& ligne);

    static bool isInclude(std::string const& ligne);
    static bool isUsingNamespace(std::string const& ligne);
    static bool isComment(std::string const& ligne);
};

#endif // FORMATTER_H
