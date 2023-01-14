#include <stddef.h>
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

GamePath mc_DefaultGamePath(char* gameRoot)
{
    GamePath gamePath;

    size_t len_gameRoot = strlen(gameRoot);

    size_t len_gameRootVersion = len_gameRoot + 10;
    char* gameRootVersion = malloc(len_gameRootVersion);
    
    size_t len_gameRootLibraries = len_gameRoot + 11;
    char* gameRootLibraries = malloc(len_gameRootLibraries);

    size_t len_gameRootRuntime = len_gameRoot + 9;
    char* gameRootRuntime = malloc(len_gameRootRuntime);

    size_t len_gameRootBin = len_gameRoot + 5;
    char* gameRootBin = malloc(len_gameRootBin);

    size_t len_gameRootAssets = len_gameRoot + 8;
    char* gameRootAssets = malloc(len_gameRootAssets);

    snprintf(gameRootAssets, len_gameRootAssets, "%s/assets", gameRoot);
    snprintf(gameRootVersion, len_gameRootVersion, "%s/versions", gameRoot);
    snprintf(gameRootLibraries, len_gameRootLibraries, "%s/libraries", gameRoot);
    snprintf(gameRootRuntime, len_gameRootRuntime, "%s/runtime", gameRoot);
    snprintf(gameRootBin, len_gameRootBin, "%s/bin", gameRoot);
    
    gamePath.version = gameRootVersion;
    gamePath.libraries = gameRootLibraries;
    gamePath.bin = gameRootBin;
    gamePath.assets = gameRootAssets;
    gamePath.runtime = gameRootRuntime;
    gamePath.root = gameRoot;

    return gamePath; 
}

CommandArguments mc_GetInheritence(char* version, GamePath gamePath, GameSettings gameSettings)
{
    CommandArguments commandArguments;

    cJSON* mainManifest = mc_GetMainManifest(gamePath);
    cJSON* manifest = mc_GetManifest(mainManifest, gamePath, version);

    if (gameSettings.downloadAssets)
    {
        cJSON* assetsManifest = mc_GetAssetsManifest(manifest, gamePath);
        mc_DownloadAssets(assetsManifest, gamePath);
    }

    char* clientPath = mc_DownloadClient(manifest, gamePath, version);

    char* javaPath = mc_DownloadJre(manifest, gamePath);
    char* assets_index = mc_GetAssetIndex(manifest);

    char* mainclass = mc_GetMainclass(manifest);

    char** classpath = mc_DownloadLibraries(manifest, gamePath);
    char* lwjglVersion = mc_GetLwjglVersion(manifest); 

    JvmArgs jvmArguments = mc_InitJvmArgs();
    GameArgs gameArguments = mc_InitGameArgs();

    if (compareLwjglVersion(lwjglVersion, "3.3.1") >= 0)
        jvmArguments.natives_directory = gamePath.root;
    else
        jvmArguments.natives_directory = mc_DownloadLwjgl(lwjglVersion, gamePath.bin);

    jvmArguments.classpath = classpath;
    jvmArguments.client = clientPath;

    gameArguments.version = version;
    gameArguments.game_directory = gamePath.root;
    gameArguments.auth_player_name = gameSettings.username;
    gameArguments.assets_root = gamePath.assets;
    gameArguments.assets_index_name = assets_index;

    char** javaArgs = mc_GetJvmArgs(manifest, jvmArguments);
    char** gameArgs = mc_GetGameArgs(manifest, gameArguments);

    commandArguments.java = javaPath;
    commandArguments.game = gameArgs;
    commandArguments.mainclass = mainclass;
    commandArguments.jvm = javaArgs;

    return commandArguments;
}

char* mc_CreateCommand(CommandArguments commandArguments)
{
    int len_command = (strlen(commandArguments.java) + 10 + strlen(commandArguments.mainclass) + 1);
    char* command = NULL;
    for (int i = 0; commandArguments.jvm[i] != NULL; i++)
    {
        for (int j = 0; commandArguments.jvm[i][j] != '\0'; j++)
            len_command++;
        len_command++;
    }
    for (int i = 0; commandArguments.game[i] != NULL; i++)
    {
        for (int j = 0; commandArguments.game[i][j] != '\0'; j++)
            len_command++;
        len_command++;
    }

    command = malloc(sizeof(char) * len_command);
    snprintf(command, len_command, "%s/bin/java ", commandArguments.java);
    for (int i = 0; commandArguments.jvm[i] != NULL; i++)
    {
        strcat(command, commandArguments.jvm[i]);
        strcat(command, " ");
    }
    strcat(command, commandArguments.mainclass);
    strcat(command, " ");
    for (int i = 0; commandArguments.game[i] != NULL; i++)
    {
        printf("%s\n", commandArguments.game[i]);
        strcat(command, commandArguments.game[i]);
        strcat(command, " ");
    }
    return command;
}
