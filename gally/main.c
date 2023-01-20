#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "minecraft/libraries.h"
#include "minecraft/versions.h"
#include "minecraft/java.h"
#include "minecraft/client.h"
#include "minecraft/assets.h"
#include "minecraft/versions.h"
#include "minecraft/microsoftAuthentification.h"
#include "launcher.h"
#include "getopt.h"
 
#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"


int main(int argc, char* argv[])
{
    ArgOpt argopt = getopt_Parse(argc, argv);
    GamePath gamePath = mc_DefaultGamePath(NULL);
    GameSettings gameSetting;

    if (argopt.show_installed)
    {
        mc_ListInstalledVersion(gamePath);
        return 0;
    }
    
    if (argopt.username)
        gameSetting.username = argopt.username;
    else
        gameSetting.username = "gally";

    if (argopt.login_microsoft)
        gameSetting.token = mc_AuthentificationMicrosoft();
    else
        gameSetting.token = "NULL";

    gameSetting.skipAssets = argopt.skip_assets;

    printf("%d\n", mc_DoesVersionExist(argopt.version, gamePath));
    CommandArguments commandArguments = mc_GetCommandArguments(argopt.version, gamePath, gameSetting);
    char* command = mc_CreateCommand(commandArguments);

    system_Exec(command);
    free(command);
    return 0;
}
