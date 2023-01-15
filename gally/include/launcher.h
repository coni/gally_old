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
    int skipAssets;
    char* username;
} GameSettings;

#include "minecraft/arguments.h"

GamePath mc_DefaultGamePath(char* gameRoot);
CommandArguments mc_GetInheritenceCommandArguments(char* version, GamePath gamePath, GameSettings gameSettings, JvmArgs parentJvmArgs);
CommandArguments mc_GetCommandArguments(char* version, GamePath gamePath, GameSettings gameSettings);
char* mc_CreateCommand(CommandArguments commandArguments);
int mc_GetTotalSizeVersion(char* version, GamePath gamePath);
int mc_GetTotalSize(char* version, GamePath gamePath);
