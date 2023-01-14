#include "launcher.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
