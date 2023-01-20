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
    GameSettings gameSetting;
    
    /* if (argopt.username) */
    /*     gameSetting.username = argopt.username; */
    /* else */
    /*     gameSetting.username = "gally"; */

    /* if (argopt.login_microsoft) */
    /*     gameSetting.token = mc_AuthentificationMicrosoft(); */
    /* else */
    /*     gameSetting.token = "NULL"; */

    /* gameSetting.skipAssets = argopt.skip_assets; */

    GamePath gamePath = mc_DefaultGamePath(NULL);
    printf("%d\n", mc_DoesVersionExist(argopt.version, gamePath));
    /* CommandArguments commandArguments = mc_GetCommandArguments(argopt.version, gamePath, gameSetting); */
    /* char* command = mc_CreateCommand(commandArguments); */

    /* system_Exec(command); */
    /* free(command); */
    return 0;
}
