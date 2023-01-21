#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "minecraft/libraries.h"
#include "minecraft/versions.h"
#include "minecraft/java.h"
#include "minecraft/client.h"
#include "minecraft/assets.h"
#include "minecraft/versions.h"
#include "minecraft/microsoftAuthentification.h"
#include "launcher.h"
#include "getopt.h"

#include <shlwapi.h>

int ListDirectoryContents(char* dir)
{
    size_t len_sDir = strlen(dir) + 1;
    wchar_t* sDir = malloc(sizeof(wchar_t) * len_sDir);
    swprintf(sDir, len_sDir, L"%hs", dir);

    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    wchar_t sPath[2048];

    //Specify a file mask. *.* = We want everything! 
    wsprintf(sPath, L"%s\\*.*", sDir);

    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        wprintf(L"Path not found: [%s]\n", sDir);
        return 0;
    }

    do
    {
        if (wcscmp(fdFile.cFileName, L".") != 0
            && wcscmp(fdFile.cFileName, L"..") != 0)
        {
            wsprintf(sPath, L"%s\\%s\\%s.json", sDir, fdFile.cFileName, fdFile.cFileName);

            if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                size_t len_vout = wcslen(sPath) + 1;
                char* vOut = malloc(sizeof(char) * len_vout);
                wcstombs_s(NULL, vOut, len_vout, sPath, len_vout);
                if (system_FileExist(vOut) == 0)
                    wprintf(L"Directory: %s\n", sPath);
            }
        }
    } while (FindNextFile(hFind, &fdFile));
    FindClose(hFind);

    return 1;
}

int main(int argc, char* argv[])
{
    ArgOpt argopt = getopt_Parse(argc, argv);
    GamePath gamePath = mc_DefaultGamePath(NULL);
    GameSettings gameSetting = {NULL, NULL, NULL, 0} ;

    if (argopt.list_installed)
    {
        mc_ListInstalledVersion(gamePath);
        return 0;
    }

    getopt_CheckRequired(argopt);
    if (argopt.username)
        gameSetting.username = argopt.username;
    else
        gameSetting.username = "gally";

    if (argopt.login_microsoft)
        gameSetting.token = mc_AuthentificationMicrosoft(gamePath, &gameSetting);

    gameSetting.skipAssets = argopt.skip_assets;

    CommandArguments commandArguments = mc_GetCommandArguments(argopt.version, gamePath, gameSetting);
    char* command = mc_CreateCommand(commandArguments);

    system_Exec(command);
    GamePath_free(&gamePath);
    free(command);
    return 0;
}
