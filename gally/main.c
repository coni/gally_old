#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "minecraft/libraries.h"
#include "minecraft/java.h"
#include "minecraft/client.h"
#include "minecraft/assets.h"
#include "minecraft/versions.h"
#include "launcher.h"

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

    char* username = "coni";
    char* version = "1.19.3";

    GameSettings gameSetting;
    gameSetting.username = username;
    gameSetting.skipAssets = 1;

    GamePath gamePath = mc_DefaultGamePath(NULL);

    CommandArguments commandArguments = mc_GetCommandArguments(version, gamePath, gameSetting);
    /* char* command = mc_CreateCommand(commandArguments); */
    /* printf("%s\n", command); */

    /* system_Exec(command); */
    return 0;
}
