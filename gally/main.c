#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    ARCHNAME = "x64";

    char* username = "coni";
    char* version = "fabric-loader-0.14.12-1.19.3";
    /* char* version = "1.19.3-forge-44.1.5"; */
    /* char* version = "1.19.2-OptiFine_HD_U_H9"; */
    /* char* version = "1.6"; */
    GameSettings gameSetting;
    gameSetting.username = username;
    gameSetting.skipAssets = 1;


    GamePath gamePath = mc_DefaultGamePath(NULL); 
    CommandArguments commandArguments = mc_GetCommandArguments(version, gamePath, gameSetting);

    char* command = mc_CreateCommand(commandArguments);
    printf("%s\n", command);

    system(command);
    return 0;
}
