#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minecraft/versions.h"
#include "minecraft/client.h"
#include "minecraft/mainclass.h"
#include "minecraft/libraries.h"
#include "minecraft/arguments.h"
#include "minecraft/java.h"
#include "minecraft/lwjgl.h"
#include "minecraft/assets.h"
#include "launcher.h"

#include "utils.h"
#include "cjson/cJSON.h"


char* ARCHNAME;
char* OSNAME;

int main()
{
    #ifdef __amd64__ 
        ARCHNAME = "x64";
    #elif _M_AMD64
        ARCHNAME = "x64";
    #elif  _M_IX86
        ARCHNAME = "x86";
    #elif i386
        ARCHNAME = "i386";
    #endif

    #ifdef __unix__
        OSNAME = "linux";
    #elif _WIN32
        OSNAME = "windows";
    #endif

    ARCHNAME = "x64";

    char* username = "coni";
    char* version = "fabric-loader-0.14.12-1.19.3";
    GameSettings gameSetting;
    gameSetting.username = username;
    gameSetting.downloadAssets = 0;

    GamePath gamePath = mc_DefaultGamePath("/home/coni/.minecraft"); 
    CommandArguments commandArguments = mc_DownloadInheritence(version, gamePath, gameSetting, NULL);

    char* command = mc_CreateCommand(commandArguments);
    printf("%s\n", command);

    /* system(command); */
    return 0;
}
