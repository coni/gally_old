#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/minecraft/versions.h"
#include "src/minecraft/client.h"
#include "src/minecraft/mainclass.h"
#include "src/minecraft/classpath.h"
#include "src/minecraft/arguments.h"
#include "src/minecraft/java.h"

#include "src/utils.h"
#include "src/cjson/cJSON.h"

int main()
{
    char* username = "coni";
    char* version = "1.19.2";
    char* gameRoot = "C:\\Users\\coni\\AppData\\Roaming\\.minecraft";
    size_t len_gameRoot = strlen(gameRoot);

    size_t len_gameRootVersion = len_gameRoot + 10;
    char* gameRootVersion = malloc(len_gameRootVersion);
    
    size_t len_gameRootLibraries= len_gameRoot + 11;
    char* gameRootLibraries = malloc(len_gameRootLibraries);

    size_t len_gameRootRuntime = len_gameRoot + 9;
    char* gameRootRuntime = malloc(len_gameRootRuntime);

    snprintf(gameRootVersion, len_gameRootVersion, "%s/versions", gameRoot);
    snprintf(gameRootLibraries, len_gameRootLibraries, "%s/libraries", gameRoot);
    snprintf(gameRootRuntime, len_gameRootRuntime, "%s/runtime", gameRoot);

    GameArgs gameArguments = mc_InitGameArgs();
    JvmArgs jvmArguments = mc_InitJvmArgs();

    cJSON* mainManifest = mc_GetMainManifest(gameRootVersion);
    cJSON* manifest = mc_GetManifest(mainManifest, gameRootVersion, version);

    char* classpath = mc_DownloadLibraries(manifest, gameRootLibraries);
    char* clientPath = mc_DownloadClient(manifest, gameRootVersion, version);
    size_t len_classpath = strlen(classpath) + strlen(clientPath);

    mc_DownloadJre(manifest, gameRootRuntime);

    classpath = realloc(classpath, len_classpath);
    strncat(classpath, clientPath, len_classpath);
    
    jvmArguments.classpath = classpath;
    jvmArguments.natives_directory = gameRoot;
    gameArguments.version = version;
    gameArguments.game_directory = gameRoot;
    gameArguments.auth_player_name = username;

    printf("%s\n%s\n%s\n", mc_GetMainclass(manifest), mc_GetGameArgs(manifest, gameArguments), mc_GetJvmArgs(manifest, jvmArguments));

    return 0;
}
