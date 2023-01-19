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
#include "getopt.h"

char* ARCHNAME;
char* OSNAME;

int main(int argc, char* argv[])
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

    ArgOpt argopt = getopt_Parse(argc, argv);
    printf("%s\n", http_get("https://example.com"));
    /* GameSettings gameSetting; */
    /* if (ai.username_arg) */
    /*     gameSetting.username = ai.username_arg; */
    /* else */
    /*     gameSetting.username = "gally"; */
    /* gameSetting.skipAssets = 1; */

    /* GamePath gamePath = mc_DefaultGamePath(NULL); */

    /* CommandArguments commandArguments = mc_GetCommandArguments(ai.version_arg, gamePath, gameSetting); */
    /* char* command = mc_CreateCommand(commandArguments); */
    /* printf("%s\n", command); */

    /* system_Exec(command); */
    /* free(command); */
    return 0;
}
