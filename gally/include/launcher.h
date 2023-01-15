#pragma once
#include "minecraft/arguments.h"

typedef struct {
    char* assets;
    char* bin;
    char* libraries;
    char* root;
    char* runtime;
    char* version;
} GamePath;

typedef struct {
    int downloadAssets;
    char* username;
} GameSettings;

GamePath mc_DefaultGamePath(char* gameRoot);
CommandArguments mc_DownloadInheritence(char* version, GamePath gamePath, GameSettings gameSettings, char** parentClasspath);
char* mc_CreateCommand(CommandArguments commandArguments);
