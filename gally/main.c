#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minecraft/versions.h"
#include "minecraft/client.h"
#include "minecraft/mainclass.h"
#include "minecraft/classpath.h"
#include "minecraft/arguments.h"
#include "minecraft/java.h"
#include "minecraft/lwjgl.h"
#include "minecraft/assets.h"

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
    /* char* version = "fabric-loader-0.14.12-1.19.3"; */
    /* char* version = "1.19.2"; */
    /* char* version = "1.18"; */
    /* char* version = "1.17"; */
    /* char* version = "1.16"; */
    /* char* version = "1.15"; */
    /* char* version = "1.14"; */
    /* char* version = "1.13"; */
    /* char* version = "1.12"; */
    /* char* version = "1.11"; */
    /* char* version = "1.10"; */
    /* char* version = "1.9"; */
    /* char* version = "1.8"; */
    /* char* version = "1.7.10"; */
    /* char* version = "1.7.1"; */
    /* char* version = "1.6"; */
    /* char* version = "1.5"; */
    /* char* version = "1.4.2"; */
    /* char* version = "1.3.1"; */
    char* version = "1.2.5";
    /* char* version = "1.1"; */
    /* char* gameRoot = "C:\\Users\\coni\\AppData\\Roaming\\.minecraft"; */ 
    size_t len_version = strlen(version);

    char* gameRoot = "/home/coni/.minecraft";
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

    JvmArgs jvmArguments = mc_InitJvmArgs();
    GameArgs gameArguments = mc_InitGameArgs();

    cJSON* mainManifest = mc_GetMainManifest(gameRootVersion);
    cJSON* manifest = mc_GetManifest(mainManifest, gameRootVersion, version);

    /* cJSON* assetsManifest = mc_GetAssetsManifest(manifest, gameRootAssets); */
    /* int a = mc_DownloadAssets(manifest, gameRootAssets); */

    char* clientPath = mc_DownloadClient(manifest, gameRootVersion, version);
    if (clientPath == NULL)
    {
        size_t len_clientPath = len_gameRootVersion + (len_version * 2) + 7;
        clientPath = malloc(sizeof(char) * len_clientPath);
        snprintf(clientPath, len_clientPath, "%s/%s/%s.jar", gameRootVersion, version, version);
    }

    char* javaPath = mc_DownloadJre(manifest, gameRootRuntime);
    char* assets_index = mc_GetAssetIndex(manifest);
    char* classpath = mc_DownloadLibraries(manifest, gameRootLibraries);
    size_t len_classpath = strlen(classpath) + strlen(clientPath) + 1;
    classpath = realloc(classpath, len_classpath);
    strncat(classpath, clientPath, len_classpath);

    char* lwjglVersion = mc_GetLwjglVersion(manifest); 
    if (lwjglVersion != NULL)
    {
        lwjglVersion[5] = '\0';

        if (strcmp(lwjglVersion, "2.9.4") == 0)
            lwjglVersion[4] = '2';
    }

    if (compareLwjglVersion(lwjglVersion, "3.3.1") >= 0)
        jvmArguments.natives_directory = gameRoot;
    else
        jvmArguments.natives_directory = mc_DownloadLwjgl(lwjglVersion, gameRootBin);

    jvmArguments.classpath = classpath;
    gameArguments.version = version;
    gameArguments.game_directory = gameRoot;
    gameArguments.auth_player_name = username;
    gameArguments.assets_root = gameRootAssets;
    gameArguments.assets_index_name = assets_index;

    char** javaArgs = mc_GetJvmArgs(manifest, jvmArguments);
    char** gameArgs = mc_GetGameArgs(manifest, gameArguments);
    for (int i = 0; javaArgs[i] != NULL; i++)
        printf("%s\n", javaArgs[i]);
    return 0;

    printf("%s/bin/java ", javaPath);
    printf("%s ", javaArgs);
    printf("%s ", mc_GetMainclass(manifest));
    printf("%s ", gameArgs); // PROBLEM

    free(lwjglVersion);
    free(gameRootBin);
    free(javaArgs);
    free(gameArgs);
    cJSON_Delete(manifest);
    cJSON_Delete(mainManifest);
    free(gameRootVersion);
    free(gameRootLibraries);
    free(gameRootRuntime);
    free(clientPath);
    free(javaPath);
    /* free(classpath); */

    return 0;
}
