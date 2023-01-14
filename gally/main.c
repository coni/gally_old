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
    /* char* version = "fabric-loader-0.14.12-1.19.3"; */
    char* version = "1.19.2";
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
    /* char* version = "1.2.5"; */
    /* char* version = "1.1"; */
    size_t len_version = strlen(version);

    GamePath gamePath = mc_DefaultGamePath("/home/coni/.minecraft"); 

    JvmArgs jvmArguments = mc_InitJvmArgs();
    GameArgs gameArguments = mc_InitGameArgs();

    cJSON* mainManifest = mc_GetMainManifest(gamePath);
    cJSON* manifest = mc_GetManifest(mainManifest, gamePath, version);

    /* cJSON* assetsManifest = mc_GetAssetsManifest(manifest, gamePath); */
    /* int a = mc_DownloadAssets(manifest, gamePath); */

    char* clientPath = mc_DownloadClient(manifest, gamePath, version);

    char* javaPath = mc_DownloadJre(manifest, gamePath);
    char* assets_index = mc_GetAssetIndex(manifest);

    char** classpath = mc_DownloadLibraries(manifest, gamePath);
    char* lwjglVersion = mc_GetLwjglVersion(manifest); 
    if (lwjglVersion != NULL)
    {
        lwjglVersion[5] = '\0';

        if (strcmp(lwjglVersion, "2.9.4") == 0)
            lwjglVersion[4] = '2';
    }

    if (compareLwjglVersion(lwjglVersion, "3.3.1") >= 0)
        jvmArguments.natives_directory = gamePath.root;
    else
        jvmArguments.natives_directory = mc_DownloadLwjgl(lwjglVersion, gamePath.bin);

    jvmArguments.classpath = classpath;
    jvmArguments.client = clientPath;
    gameArguments.version = version;
    gameArguments.game_directory = gamePath.root;
    gameArguments.auth_player_name = username;
    gameArguments.assets_root = gamePath.assets;
    gameArguments.assets_index_name = assets_index;

    char** javaArgs = mc_GetJvmArgs(manifest, jvmArguments);
    char** gameArgs = mc_GetGameArgs(manifest, gameArguments);
    char* mainclass = mc_GetMainclass(manifest);

    int len_command = (strlen(javaPath) + 10 + strlen(mainclass) + 1);
    for (int i = 0; javaArgs[i] != NULL; i++)
    {
        for (int j = 0; javaArgs[i][j] != '\0'; j++)
            len_command++;
        len_command++;
    }
    for (int i = 0; gameArgs[i] != NULL; i++)
    {
        for (int j = 0; gameArgs[i][j] != '\0'; j++)
            len_command++;
        len_command++;
    }

    char* command = malloc(sizeof(char) * len_command);
    snprintf(command, len_command, "%s/bin/java ", javaPath);
    for (int i = 0; javaArgs[i] != NULL; i++)
    {
        strcat(command, javaArgs[i]);
        strcat(command, " ");
    }
    strcat(command, mainclass);
    strcat(command, " ");
    for (int i = 0; gameArgs[i] != NULL; i++)
    {
        printf("%s\n", gameArgs[i]);
        strcat(command, gameArgs[i]);
        strcat(command, " ");
    }
    printf("%s\n", command);
    system(command);

    free(lwjglVersion);
    free(javaArgs);
    free(gameArgs);
    cJSON_Delete(manifest);
    cJSON_Delete(mainManifest);
    free(clientPath);
    free(javaPath);
    /* free(classpath); */

    return 0;
}
