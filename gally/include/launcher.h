#pragma once

typedef struct {
    char* assets;
    char* bin;
    char* libraries;
    char* root;
    char* runtime;
    char* version;
} GamePath;

GamePath mc_DefaultGamePath(char* gameRoot);
