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
    char* username;
    char* token;
    char* uuid;
    int skipAssets;
} GameSettings;

#include "minecraft/arguments.h"

GamePath mc_DefaultGamePath(char* gameRoot);
CommandArguments mc_GetInheritenceCommandArguments(char* version, GamePath gamePath, GameSettings gameSettings, JvmArgs parentJvmArgs);
CommandArguments mc_GetCommandArguments(char* version, GamePath gamePath, GameSettings gameSettings);
CommandArguments CommandArguments_free(CommandArguments* ca);
char* mc_CreateCommand(CommandArguments commandArguments);
int mc_GetTotalSizeVersion(char* version, GamePath gamePath, GameSettings gameSettings);
int mc_GetTotalSize(char* version, GamePath gamePath, GameSettings gameSettings);
void GamePath_free(GamePath* gamePath);
char* mc_AuthentificationMicrosoft(GamePath gamePath, GameSettings* gameSetting);
