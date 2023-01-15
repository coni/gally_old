#pragma once

typedef struct {
	char* version;
	char* auth_player_name;
	char* auth_access_token;
	char* version_name;
	char* game_directory;
	char* assets_root;
	char* assets_index_name;
	char* auth_uuid;
	char* clientid;
	char* auth_xuid;
	char* user_type;
	char* user_properties;
	char* version_type;
	char* resolution_width;
	char* resolution_height;
} GameArgs;

typedef struct {
	char** classpath;
    char* client;
	char* launcher_name;
	char* launcher_version;
	char* natives_directory;
} JvmArgs;

typedef struct {
    char* java;
    char** jvm;
    char* mainclass;
    char** game;
} CommandArguments;

#include "cjson/cJSON.h"
#include "launcher.h"

char** mc_GetJvmArgs(cJSON* manifest, JvmArgs args, GameArgs gameArgs, GamePath gamePath);
char** mc_GetGameArgs(cJSON* manifest, GameArgs args);

JvmArgs mc_InitJvmArgs();
GameArgs mc_InitGameArgs();
