#include <string>
#include <fstream>  // ifstream

#include "ConfigFile.h"

bool ConfigFile::load(const char *path){
    std::ifstream fin;
    fin.open(path);
    if(!fin.is_open()){
        return false;
    }

    std::string lineBuf, itemName, itemContent;
    while(fin.good()){
        std::getline(fin, lineBuf);
        
        if(lineBuf.empty()){
            continue;
        }

        if(lineBuf[0] == '#' || lineBuf[0] == '[' || lineBuf[0] == '\t' || lineBuf[0] == '\r' || lineBuf[0] == '\n'){
            continue;
        }

        size_t index = lineBuf.find('=');
        if(index == std::string::npos){
            continue;
        }

        itemName = lineBuf.substr(0, index);
        itemContent = lineBuf.substr(index+1);
        trim_space(itemName);
        trim_space(itemContent);

        m_configItemStore.insert({itemName, itemContent});
    }

    return true;
}

void ConfigFile::trim_space(std::string& string){
    if(string.empty()){
        return;
    }

    string.erase(0, string.find_first_not_of(" "));
    string.erase(string.find_last_not_of(" ") + 1);
}