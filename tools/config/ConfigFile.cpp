#include <stdio.h>  // FILE fopen
#include <string.h>

#include "ConfigFile.h"

#define ITEM_LINE_SZ 500      // 限制配置项一行最大500字节
#define ITEM_NAME_SZ 50       // 限制配置项的名字最大50字节
#define ITEM_CONTENT_SZ 400   // 限制配置项的内容最大400字节

bool ConfigFile::load(const char * configFileName){
    FILE * fp;
    fp = fopen(configFileName, "r");
    if(!fp){
        return false;
    }

    char lineBuf[ITEM_LINE_SZ];
    while(!feof(fp)){
        fgets(lineBuf, ITEM_LINE_SZ, fp);

        if(strlen(lineBuf) == 0){
            continue;
        }

        if(lineBuf[0] == '#' || lineBuf[0] == '[' || lineBuf[0] == ' ' || lineBuf[0] == '\t' || lineBuf[0] == '\n'){    // 保证了itemName左边没有空格
            continue;
        }

parseString:
        size_t len = strlen(lineBuf);
        if(lineBuf[len-1] == '\r' || lineBuf[len-1] == '\n' || lineBuf[len-1] == ' '){                                  // 保证了itemContent右侧没有空格
            lineBuf[len-1] = 0;
            goto parseString;
        }

        char * pos = strchr(lineBuf, '=');
        if(pos != nullptr){
            char itemName[ITEM_NAME_SZ] = {0};
            char itemContent[ITEM_CONTENT_SZ] = {0};
            strncpy(itemName, lineBuf, (size_t)(pos-lineBuf));
            strcpy(itemContent, pos + 1);

            trim_tail_space(itemName);                                                                                  // 保证了itemName右边没有空格
			trim_head_space(itemContent);                                                                               // 保证了itemContent左侧没有空格

            m_configItemStore.insert({std::string(itemName), std::string(itemContent)});
        }
    }

    return true;
}

void ConfigFile::trim_tail_space(char * string){ 
	if(string == nullptr)   
		return;   

	size_t len = strlen(string);   
	while(len > 0 && string[len-1] == ' '){
        string[len-1] = 0;
		-- len;
    }
}

void ConfigFile::trim_head_space(char * string){
	size_t len = strlen(string);
	char * p_tmp = string;
	if((*p_tmp) != ' ')                             //不是以空格开头
		return;

	while((*p_tmp) != '\0'){
		if((*p_tmp) == ' ')
			++ p_tmp;
		else
			break;
	}

	if((*p_tmp) == '\0'){                           // 字符串string全是空格
		*string = '\0';
		return;
	}

	while((*p_tmp) != '\0'){
		(*string) = (*p_tmp);
		++ p_tmp;
		++ string;
	}
	*string = '\0';
}