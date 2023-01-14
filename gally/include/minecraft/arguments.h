#include "cjson/cJSON.h"

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
	char* classpath;
	char* launcher_name;
	char* launcher_version;
	char* natives_directory;
} JvmArgs;

char** mc_GetJvmArgs(cJSON* manifest, JvmArgs args);
char** mc_GetGameArgs(cJSON* manifest, GameArgs args);

JvmArgs mc_InitJvmArgs();
GameArgs mc_InitGameArgs();
