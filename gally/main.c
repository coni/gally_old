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
    char* version = "1.19.2";
    /* char* gameRoot = "C:\\Users\\coni\\AppData\\Roaming\\.minecraft"; */ 
    char* gameRoot = "/home/coni/.minecraft";
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

    JvmArgs jvmArguments = mc_InitJvmArgs();
    GameArgs gameArguments = mc_InitGameArgs();

    cJSON* mainManifest = mc_GetMainManifest(gameRootVersion);
    cJSON* manifest = mc_GetManifest(mainManifest, gameRootVersion, version);

    char* clientPath = mc_DownloadClient(manifest, gameRootVersion, version);
    char* javaPath = mc_DownloadJre(manifest, gameRootRuntime);

    char* classpath = mc_DownloadLibraries(manifest, gameRootLibraries);
    size_t len_classpath = strlen(classpath) + strlen(clientPath) + 1;
    char* lwjglVersion = mc_GetLwjglVersion(manifest); 


    classpath = realloc(classpath, len_classpath);
    strncat(classpath, clientPath, len_classpath);

    jvmArguments.classpath = classpath;
    jvmArguments.natives_directory = gameRoot;
    gameArguments.version = version;
    gameArguments.game_directory = gameRoot;
    gameArguments.auth_player_name = username;

    char* gameArgs = mc_GetGameArgs(manifest, gameArguments);
    char* javaArgs = mc_GetJvmArgs(manifest, jvmArguments);


    /* printf("%s\n", lwjglVersion); */
    /* printf("%s/bin/java ", javaPath); */
    /* printf("%s", javaArgs); */
    /* printf("%s ", mc_GetMainclass(manifest)); */
    /* printf("%s ", gameArgs); // PROBLEM */

    free(javaArgs);
    free(gameArgs);
    cJSON_Delete(manifest);
    cJSON_Delete(mainManifest);
    free(gameRootVersion);
    free(gameRootLibraries);
    free(gameRootRuntime);
    free(clientPath);
    free(javaPath);
    free(classpath);

    return 0;
}
