#include <iostream>
#include <vector>

#include "Level.h"
#include "LevelLoader.h"

#include "LevelPackLoader.h"


#include "SoundPlayer.h"

#include "MenuPage.h"

#include "Console.h"

using std::vector;

static SoundPlayer* g_soundPlayer = NULL;

static void Menu_GameStart()
{
    cout << "\t Game start" << endl;
    WaitForInput();
    MenuNavBack();
}

static void Menu_SelectLevelPack()
{
    cout << "\t Select level pack" << endl;
    WaitForInput();
    MenuNavBack();
}

static void Menu_SettingsAdjMusicVol()
{
    cout << "\t Change music volume" << endl;
    WaitForInput();
    MenuNavBack();
}

static void Menu_SettingsAdjSoundsVol()
{
    cout << "\t Change sounds volume" << endl;
    WaitForInput();
    MenuNavBack();
}

static void Menu_InfoAbout()
{
    cout << "\t About this game" << endl;
    cout << endl;
    cout << "\t   Idea of creating this game" << endl;
    cout << "\t     Some time ago I found a game called Untrusted." << endl;
    cout << "\t     Untrusted game is similar to those and it" << endl;
    cout << "\t     inspired me to create similar game in console" << endl;
    cout << "\t     version." << endl;

    WaitForInput();
    MenuNavBack();
}

static void MenuInfoAuthor()
{
    cout << "\t Author and contact" << endl;
    cout << endl;
    cout << "\t   Game created by shjeff" << endl;
    cout << "\t   Contact: jffs@wp.pl" << endl;
    WaitForInput();
    MenuNavBack();
}

static MenuPage g_mainMenu(
    "Main menu",
    CONSOLE_COLOR_LIGHT_CYAN,
    CONSOLE_COLOR_BLUE, 
    vector<MenuPage>({
        MenuPage("Start game", CONSOLE_COLOR_CYAN, &Menu_GameStart),
        MenuPage("Select level pack", CONSOLE_COLOR_CYAN, &Menu_SelectLevelPack),
        MenuPage("Game settings",
            CONSOLE_COLOR_CYAN,
            CONSOLE_COLOR_RED,
            vector<MenuPage>({
            MenuPage("Adjust music volume", CONSOLE_COLOR_CYAN, &Menu_SettingsAdjMusicVol),
            MenuPage("Adjust sounds volume", CONSOLE_COLOR_CYAN, &Menu_SettingsAdjSoundsVol)
        })),
        MenuPage("Info",
            CONSOLE_COLOR_CYAN,
            CONSOLE_COLOR_BROWN,
            vector<MenuPage>({
            MenuPage("About game", CONSOLE_COLOR_CYAN, &Menu_InfoAbout),
            MenuPage("Author", CONSOLE_COLOR_CYAN, &MenuInfoAuthor)
        }))
    })
);

void InitGame(SoundPlayer& soundPlayer, LevelPackLoader& levelPackLoader)
{
    al_init();
    al_set_standard_fs_interface();
    al_set_standard_file_interface();

    MENU_INIT(g_mainMenu);

    soundPlayer.Initialize(al_get_current_directory());
}

void LoadGame(LevelPackList& levelPackList, SoundPlayer& soundPlayer, LevelPackLoader& levelPackLoader)
{
    cout << "Scanning level pack files..." << endl;
    LevelPackFileList levelPackFiles = levelPackLoader.ScanLevelPackDirectory();
    for (LevelPackFile levelPackFile : levelPackFiles) {
        cout << "\t Checking file: " << levelPackFile.GetFileName();
        if (levelPackLoader.LoadLevelPack(levelPackFile)) {
            cout << " - OK" << endl;
            levelPackList.push_back(levelPackLoader.GetLevelPack());
        } else {
            cout << " - ERROR" << endl;
        }
    }

    //vector<LevelPack> levelPacks = levelPackLoader.GetLevelPackList();

    /* Load game music */
    //for (LevelPack levelPack : levelPacks) {
    //    for (Level level : levelPack.GetLevels()) {
    //        LevelMusic levelMusic = level.GetLevelMusic();
    //        soundPlayer.InstallSound(levelMusic.lm_id, (char *)levelMusic.lm_data, levelMusic.lm_size);
    //    }
    //}
}

void StartGame(SoundPlayer& soundPlayer, LevelPackLoader& levelPackLoader)
{
    //MENU_START(g_mainMenu);
}

int main(int argc, char ** argv)
{
    LevelPackList levelPackList;
    SoundPlayer soundPlayer;
    LevelPackValidator levelPackValidator;
    LevelPackLoader levelPackLoader(levelPackValidator);

    g_soundPlayer = &soundPlayer;

    InitGame(soundPlayer, levelPackLoader);
    LoadGame(levelPackList, soundPlayer, levelPackLoader);
    StartGame(soundPlayer, levelPackLoader);

    WaitForInput();

    return 0;
}
