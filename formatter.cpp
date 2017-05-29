#include "formatter.h"

using namespace std;

Formatter::Formatter(string filePath)
{
    m_fileFlux = ifstream(filePath);
    if (exec(""))
    {
        formatLigne(m_data);

        m_map = genereWordMap(m_data);
    }
}

Formatter::~Formatter()
{
    //dtor
}

bool Formatter::exec(string replaceValue)
{
    if (m_fileFlux){
        string ligne;
        m_data = "";
        while(getline(m_fileFlux, ligne))
        {
            formatLigne(ligne);
            if (ligne.empty()){
                continue;
            }
            filtreGuillemet(ligne, replaceValue);
            m_data+=ligne;
        }
        return true;
    } else {
        return false;
    }
}

map<string, int> Formatter::genereWordMap(string& data)
{
    map<string, int> myMap;

    string mot;
    for(int i(0); i < data.size(); i++)
    {
        char& c = data.at(i);

        if (isalnum(c)) {
            mot += c;
        } else {
            myMap[mot] += 1;
            mot.clear();
            if (!isspace(c)){
                myMap[string(1,c)] += 1;
            }
        }
    }
    return myMap;
}

void Formatter::filtreGuillemet(string& ligne, string defaultValue)
{
    size_t foundGuillemet = ligne.find("\"");

    while (foundGuillemet != string::npos) {
        size_t relais = foundGuillemet+1;

        size_t foundGuillemetClose = string::npos;

        do{
            foundGuillemetClose = ligne.find("\"", relais);
            if (ligne[foundGuillemetClose-1] == '\\'){
                relais = foundGuillemetClose+1;
                foundGuillemetClose = string::npos;
            }
        }while (foundGuillemetClose == string::npos);
        string currentGuillemet = ligne.substr(foundGuillemet,foundGuillemetClose+1-foundGuillemet);
        ligne.replace(foundGuillemet, currentGuillemet.size(), defaultValue);
        foundGuillemet = ligne.find("\"");
    }
}

void Formatter::formatLigne(string& ligne)
{
    if (!isEmpty(ligne)){
        cleanLigne(ligne);
        if (isInclude(ligne) || isUsingNamespace(ligne) || isComment(ligne)){
            ligne = "";
        }
    } else {
        ligne = "";
    }
}

bool Formatter::isEmpty(string ligne)
{
    if (ligne.empty()){
        return true;
    }

    ligne.erase(remove(ligne.begin(), ligne.end(), ' '), ligne.end());
    ligne.erase(remove(ligne.begin(), ligne.end(), '	'), ligne.end());

    if (ligne.empty()){
        return true;
    }

    return false;
}

void Formatter::cleanLigne(string& ligne)
{
    while (true) {
        if (!findReplace(ligne, "  ", " ") &&
            !findReplace(ligne, "	", " ")) break;
    }

    if (ligne[0] == ' '){
        ligne.replace(0, 1, "");
    }
}

bool Formatter::findReplace(string& ligne, string const& val, string const& newVal)
{
    size_t found = ligne.find(val);
    if (found != string::npos)
    {
        ligne.replace(found, val.size(), newVal);
        return true;
    }
    return false;
}

bool Formatter::isInclude(std::string const& ligne)
{
    return ligne.find("#include ") != string::npos;
}

bool Formatter::isUsingNamespace(std::string const& ligne){
    return ligne.find("using namespace") != string::npos;
}

bool Formatter::isComment(std::string const& ligne){
    return ligne[0] == '/' && ligne[1] == '/';
}
