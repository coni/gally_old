#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minecraft/versions.h"
#include "minecraft/client.h"
#include "minecraft/profil.h"
#include "minecraft/mainclass.h"
#include "minecraft/libraries.h"
#include "minecraft/arguments.h"
#include "minecraft/java.h"
#include "minecraft/lwjgl.h"
#include "minecraft/assets.h"
#include "minecraft/microsoftAuthentification.h"
#include "launcher.h"
#include "utils.h"

void GamePath_free(GamePath* gamePath)
{
    free(gamePath->assets);
    free(gamePath->version);
    free(gamePath->libraries);
    free(gamePath->root);
    free(gamePath->runtime);
    free(gamePath->bin);
}

char* mc_AuthentificationMicrosoft(GamePath gamePath, GameSettings* gameSetting)
{
    cJSON* tmp = NULL;
    cJSON* tmp_i = NULL;
    char* uhs = NULL;
    char* mctoken = NULL;
    char* mstoken = NULL;
    char* xststoken = NULL;
    char* xbtoken = NULL;
    int expire = time(NULL);

    tmp = accessToken();
    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp, "access_token");
    mstoken = tmp_i->valuestring;
    //FREE
    
    tmp = xblToken(mstoken);
    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp, "Token");
    xbtoken = tmp_i->valuestring;
    //FREE

    tmp = xstsToken(xbtoken);
    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp, "Token");
    xststoken = tmp_i->valuestring;
    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp, "DisplayClaims");
    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp_i, "xui");
    tmp_i = cJSON_GetArrayItem(tmp_i, 0); 
    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp_i, "uhs");
    uhs = tmp_i->valuestring;

    tmp = minecraftToken(xststoken, uhs);
    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp, "access_token");
    mctoken = tmp_i->valuestring;

    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp, "expires_in");
    expire += tmp_i->valueint;

    tmp = getProfile(mctoken);
    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp, "name");
    gameSetting->username = tmp_i->valuestring;

    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp, "id");
    gameSetting->uuid = tmp_i->valuestring;

	char buf_input[4];
    printf("Stay signed in? (yes/no) %s : ", gameSetting->username);
	scanf("%s", &buf_input);
    if (strcmp(buf_input, "yes") == 0)
    {
        size_t len_fullpath = strlen(gamePath.root) + strlen("/accounts.json") + 1;
        char* fullpath = malloc(sizeof(char) * len_fullpath);
        snprintf(fullpath, len_fullpath, "%s/accounts.json", gamePath.root);

        tmp = cJSON_CreateObject();
        tmp_i = cJSON_CreateObject();
        cJSON_AddStringToObject(tmp_i, "access_token", mctoken);
        cJSON_AddNumberToObject(tmp_i, "expiration", expire);
        cJSON_AddItemToObject(tmp, gameSetting->username, tmp_i);
        char* raw = cJSON_Print(tmp);
		system_CreateFile(fullpath, raw);

        free(raw);
        free(fullpath);
    }

    return mctoken;
}

GamePath mc_DefaultGamePath(char* gameRoot)
{
    if (gameRoot == NULL)
    {
        char* app_directory = NULL;
        #ifdef __unix__
            app_directory = getenv("HOME");
        #elif _WIN32
            app_directory = getenv("APPDATA");
        #endif

        size_t len_gameRoot = strlen(app_directory) + 12;
        gameRoot = malloc(sizeof(char) * (len_gameRoot));
        snprintf(gameRoot, len_gameRoot, "%s/.minecraft", app_directory);
    }
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

int mc_GetTotalSizeVersion(char* version, GamePath gamePath, GameSettings gameSettings)
{
    int total_size = 0;
    total_size += mc_GetLibrariesSizeVersion(version, gamePath);
    total_size += mc_GetClientSizeVersion(version, gamePath);
    if (gameSettings.skipAssets == 0)
        total_size += mc_GetAssetsSizeVersion(version, gamePath);
    total_size += mc_GetJreSizeVersion(version, gamePath);
    return total_size;
}

int mc_GetTotalSize(char* version, GamePath gamePath, GameSettings gameSettings)
{
    int total_size = 0;
    cJSON* mainManifest = mc_GetMainManifest(gamePath);
    cJSON* manifest = mc_GetManifest(mainManifest, gamePath, version);
    total_size += mc_GetTotalSizeVersion(version, gamePath, gameSettings);
    char* inherit =  mc_GetInherit(manifest);

    total_size = inherit != NULL ? total_size + mc_GetTotalSize(inherit, gamePath, gameSettings) : total_size;

    cJSON_Delete(mainManifest);
    cJSON_Delete(manifest);
    return total_size;
}

CommandArguments mc_GetCommandArguments(char* version, GamePath gamePath, GameSettings gameSettings)
{
    if (mc_DoesVersionExist(version, gamePath) == 0)
        system_Error(1, "The specified version does not exist");
    else if (mc_IsUsernameCorrect(gameSettings.username) == 0)
        system_Error(1, "The specified username is incorrect");

    DOWNLOAD_TOTAL = mc_GetTotalSize(version, gamePath, gameSettings);
    JvmArgs jvmArgs = mc_InitJvmArgs();
    CommandArguments commandArguments = mc_GetInheritenceCommandArguments(version, gamePath, gameSettings, jvmArgs);
    
    if (commandArguments.java == NULL)
    {
        commandArguments.java = mc_DownloadJreComponent("jre-legacy", gamePath);
    }
    return commandArguments;
}

CommandArguments mc_GetInheritenceCommandArguments(char* version, GamePath gamePath, GameSettings gameSettings, JvmArgs parentJvmArgs)
{
    CommandArguments commandArguments;

    cJSON* mainManifest = mc_GetMainManifest(gamePath);
    cJSON* manifest = mc_GetManifest(mainManifest, gamePath, version);

    if (gameSettings.skipAssets == 0)
    {
        cJSON* assetsManifest = mc_GetAssetsManifest(manifest, gamePath);
        mc_DownloadAssets(assetsManifest, gamePath);
    }
    char* clientPath = NULL;
    if (parentJvmArgs.client == NULL)
        clientPath = mc_DownloadClient(manifest, gamePath, version);
    else
    {
        clientPath = malloc(sizeof(char) * strlen(1 + (parentJvmArgs.client)));
        strcpy(clientPath, parentJvmArgs.client);
    }

    char* javaPath = mc_DownloadJre(manifest, gamePath);
    char* assets_index = mc_GetAssetIndex(manifest);

    char* mainclass = mc_GetMainclass(manifest);

    char** classpath = mc_DownloadLibraries(manifest, gamePath);
    char* lwjglVersion = mc_GetLwjglVersion(manifest); 

    JvmArgs jvmArguments = mc_InitJvmArgs();
    GameArgs gameArguments = mc_InitGameArgs();

    if (compareLwjglVersion(lwjglVersion, "3.3.1") >= 0)
        jvmArguments.natives_directory = gamePath.root;
    else
        jvmArguments.natives_directory = mc_DownloadLwjgl(lwjglVersion, gamePath.bin);

    if (parentJvmArgs.classpath)
    {
        char** parentClasspath = parentJvmArgs.classpath;
        size_t len_parentClasspath;
        size_t len_classpath;
        for (len_parentClasspath = 0; parentClasspath[len_parentClasspath] != NULL; len_parentClasspath++);
        for (len_classpath = 0; classpath[len_classpath] != NULL; len_classpath++);

        len_classpath += len_parentClasspath;
        classpath = realloc(classpath, sizeof(char*) * (len_classpath + 1));

        int j = 0;
        size_t i;
        for (i = len_classpath - len_parentClasspath; i < len_classpath; i++)
        {
            classpath[i] = malloc(sizeof(char) * (strlen(parentClasspath[j]) + 1));
            strcpy(classpath[i], parentClasspath[j++]);
        }
        classpath[i] = NULL;
    }

    jvmArguments.classpath = classpath;
    jvmArguments.client = clientPath;

    gameArguments.auth_player_name = gameSettings.username;
    gameArguments.version = version;
    if (gameSettings.token == NULL || strcmp(gameSettings.token, "NULL") == 0)
    {
        size_t len_fullpath = strlen(gamePath.root) + strlen("/accounts.json") + 1;
        char* fullpath = malloc(sizeof(char) * len_fullpath);
        snprintf(fullpath, len_fullpath, "%s/accounts.json", gamePath.root);
        if (system_FileExist(fullpath) == 0)
        {
            cJSON* tmp = json_ParseFile(fullpath);
            cJSON* tmp_i = NULL;
            cJSON_ArrayForEach(tmp_i, tmp)
            {
                if (strcmp(tmp_i->string, gameSettings.username) == 0)
                {
                    tmp_i = cJSON_GetObjectItemCaseSensitive(tmp_i, "access_token");
                    if (tmp_i)
                    {
                        gameArguments.auth_access_token = tmp_i->valuestring;
                        break;
                    }
                }

            }

            cJSON_Delete(tmp);
        }
        else
            gameArguments.auth_access_token = "NULL";
    }
    else
        gameArguments.auth_access_token = gameSettings.token;

    gameArguments.game_directory = gamePath.root;
    gameArguments.assets_root = gamePath.assets;
    gameArguments.assets_index_name = assets_index;
    if (gameArguments.auth_uuid == NULL)
    {
        if (gameSettings.uuid)
            gameArguments.auth_uuid = gameSettings.uuid;
        else
            gameArguments.auth_uuid = mc_GetUUID(gameArguments.auth_player_name);
    }

    char** jvmArgs = mc_GetJvmArgs(manifest, jvmArguments, gameArguments, gamePath);
    char** gameArgs = mc_GetGameArgs(manifest, gameArguments);

    commandArguments.java = javaPath;
    commandArguments.jvm = jvmArgs;
    commandArguments.mainclass = mainclass;
    commandArguments.game = gameArgs;

    char* inherit = mc_GetInherit(manifest);
    if (inherit)
    {
        CommandArguments inheritArgument = mc_GetInheritenceCommandArguments(inherit, gamePath, gameSettings, jvmArguments);
        if (commandArguments.java == NULL)
            commandArguments.java = inheritArgument.java;

        if (commandArguments.mainclass == NULL)
            commandArguments.mainclass = inheritArgument.mainclass;

        if (jvmArgs != NULL && jvmArgs[0] != NULL)
        {
            size_t len_inheritJvm;
            for (len_inheritJvm = 0; inheritArgument.jvm[len_inheritJvm] != NULL; len_inheritJvm++);
            size_t len_jvm;
            for (len_jvm = 0; jvmArgs[len_jvm] != NULL; len_jvm++);
            len_inheritJvm += len_jvm;
            inheritArgument.jvm = realloc(inheritArgument.jvm, sizeof(char*) * (len_inheritJvm + 1));
            size_t i;
            int j = 0;
            for (i = len_inheritJvm - len_jvm; i < len_inheritJvm; i++)
            {
                inheritArgument.jvm[i] = malloc(sizeof(char) * (strlen(jvmArgs[j]) + 1));
                strcpy(inheritArgument.jvm[i], jvmArgs[j]);
                free(jvmArgs[j++]);
            }
            inheritArgument.jvm[i] = NULL;
            free(jvmArgs);
            commandArguments.jvm = inheritArgument.jvm;
        }
        else
            commandArguments.jvm = inheritArgument.jvm;

        if (gameArgs[0] != NULL)
        {
            size_t len_game;
            for (len_game = 0; gameArgs[len_game] != NULL; len_game++);

            size_t len_inheritGame;
            for (len_inheritGame = 0; inheritArgument.game[len_inheritGame] != NULL; len_inheritGame++);

            len_inheritGame += len_game;
            inheritArgument.game = realloc(inheritArgument.game, sizeof(char*) * (len_inheritGame + 1));
            int j = 0;
            size_t i;
            for (i = len_inheritGame - len_game; i < len_inheritGame; i++)
            {
                inheritArgument.game[i] = malloc(sizeof(char) * (strlen(gameArgs[j]) + 1));
                strcpy(inheritArgument.game[i], gameArgs[j]);
                free(gameArgs[j++]);
            }
            inheritArgument.game[i] = NULL;
            free(gameArgs);
            free(inherit);
            commandArguments.game = inheritArgument.game;
        }
        else
            commandArguments.game = inheritArgument.game;
    }

    cJSON_Delete(manifest);
    cJSON_Delete(mainManifest);
    return commandArguments;
}

char* mc_CreateCommand(CommandArguments commandArguments)
{
    int len_command = (strlen(commandArguments.java) + 10 + strlen(commandArguments.mainclass) + 1);
    char* command = NULL;
    for (int i = 0; commandArguments.jvm[i] != NULL; i++)
    {
        for (int j = 0; commandArguments.jvm[i][j] != '\0'; j++)
            len_command++;
        len_command++;
    }
    for (int i = 0; commandArguments.game[i] != NULL; i++)
    {
        for (int j = 0; commandArguments.game[i][j] != '\0'; j++)
            len_command++;
        len_command++;
    }

    command = malloc(sizeof(char) * len_command);
    snprintf(command, len_command, "%s/bin/java ", commandArguments.java);
    for (int i = 0; commandArguments.jvm[i] != NULL; i++)
    {
        strcat(command, commandArguments.jvm[i]);
        strcat(command, " ");
    }
    strcat(command, commandArguments.mainclass);
    strcat(command, " ");
    for (int i = 0; commandArguments.game[i] != NULL; i++)
    {
        strcat(command, commandArguments.game[i]);
        strcat(command, " ");
    }
    return command;
}
