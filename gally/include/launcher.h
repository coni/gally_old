#pragma once

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

#include "minecraft/arguments.h"

GamePath mc_DefaultGamePath(char* gameRoot);
CommandArguments mc_DownloadInheritence(char* version, GamePath gamePath, GameSettings gameSettings, JvmArgs parentJvmArgs);
char* mc_CreateCommand(CommandArguments commandArguments);
