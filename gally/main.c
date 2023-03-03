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

int main(int argc, char* argv[])
{
    ArgOpt argopt = getopt_Parse(argc, argv);
    GamePath gamePath = mc_DefaultGamePath(NULL);
    GameSettings gameSetting = {NULL, NULL, NULL, 0} ;

    if (argopt.list_installed)
    {
        mc_ListInstalledVersion(gamePath);
        return 0;
    }

    getopt_CheckRequired(argopt);
    if (argopt.username)
        gameSetting.username = argopt.username;
    else
        gameSetting.username = "gally";

    if (argopt.login_microsoft)
        gameSetting.token = mc_AuthentificationMicrosoft(gamePath, &gameSetting);

    gameSetting.skipAssets = argopt.skip_assets;

    CommandArguments commandArguments = mc_GetCommandArguments(argopt.version, gamePath, gameSetting);
    char* command = mc_CreateCommand(commandArguments);

    /* printf("%s\n", command); */
    system_Exec(command);
    CommandArguments_free(&commandArguments);
    GamePath_free(&gamePath);
    free(command);
    return 0;
}
