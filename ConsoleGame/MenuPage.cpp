#include <vector>

#include "Console.h"

#include "MenuPage.h"

using std::vector;

static MenuPage * g_currentViewingMenuPage = NULL;

MenuPage::MenuPage(string _menuTitle, ConsoleColor _menuTitleColor, ConsoleColor _menuItemsColor, vector<MenuPage> _subMenus)
{
	this->menuTitle = _menuTitle;
	this->isLeaf = false;
	this->subMenus = _subMenus;
	this->menuTitleColor = _menuTitleColor;
	this->menuItemsColor = _menuItemsColor;
}

MenuPage::MenuPage(string _menuTitle, ConsoleColor _menuTitleColor, MenuLeafHandler _leafHandler)
{
	this->menuTitle = _menuTitle;
	this->isLeaf = true;
	this->leafHandler = _leafHandler;
	this->menuTitleColor = _menuTitleColor;
	this->menuItemsColor = (ConsoleColor)0;
}

void MenuPage::Initialize(MenuPage * menuPage)
{
	this->parentMenu = menuPage;

	for (size_t i = 0; i < this->subMenus.size(); i++) {
		this->subMenus.at(i).Initialize(this);
	}
}

bool MenuPage::IsLeaf()
{
	return this->isLeaf;
}

vector<MenuPage>& MenuPage::GetSubMenus()
{
	return this->subMenus;
}

string MenuPage::GetTitle()
{
	return this->menuTitle;
}

void MenuPage::PrintMenuPage(MenuPage menuPage)
{
	vector<MenuPage>& menus = menuPage.GetSubMenus();
	int menuOption;

	SetConsoleColor(CONSOLE_COLOR_BLACK, menuPage.GetMenuTitleColor());
	cout << "\t" << menuPage.GetTitle() << "\t" << endl;
	cout << endl;

	SetConsoleColor(CONSOLE_COLOR_BLACK, menuPage.GetMenuItemsColor());
	for (size_t i = 0; i < menus.size(); i++) {
		menuOption = i + 1;
		cout << "\t    " << menuOption << ") " << menus.at(i).GetTitle() << endl;
	}
	menuOption++;
	cout << "\t    " << menuOption << ") Back" << endl;

	SetConsoleColor(CONSOLE_COLOR_BLACK, CONSOLE_COLOR_WHITE);
}

bool MenuPage::ValidateOption(MenuPage menuPage, int menuIndex)
{
	int numOfSubMenus = (int)menuPage.GetSubMenus().size();

	/*
	 * Condition `menuIndex <= numOfSubMenus` is used
	 * because we check also exit/back option
	 */
	return ((menuIndex >= 0) && (menuIndex <= numOfSubMenus));
}

void MenuPage::MenuNavigateEnter(MenuPage menuPage)
{
	vector<MenuPage>& subMenuVec = menuPage.GetSubMenus();
	char consoleInpChr;
	int  consoleInpIdx;
	bool consoleInpErr = false;
	bool menuExitFlag = false;

	g_currentViewingMenuPage = &menuPage;

	do {
		ConsoleClear();
		PrintMenuPage(menuPage);
		consoleInpErr = false;
		menuExitFlag = false;
		consoleInpChr = ConsoleReadKey();
		consoleInpIdx = CONV_INP_TO_INDEX(consoleInpChr);

		if (IS_EXIT_KEY(consoleInpChr)) {
			menuExitFlag = true;
			break;
		}

		if (!ValidateOption(menuPage, consoleInpIdx)) {
			cout << "Invalid menu option, try again" << endl;
			WaitForInput();
			consoleInpErr = true;
		}

	} while (consoleInpErr);

	if (menuExitFlag || (IS_MENU_EXIT_KEY(consoleInpIdx, subMenuVec))) {
		MenuPage * parentMenu = menuPage.GetParent();
		if (parentMenu) {
			MenuNavigateEnter(*parentMenu);
		} else {
			cout << "Closing game..." << endl;
			WaitForInput();
			return;
		}
	} else {
		MenuPage selectedMenu = subMenuVec.at(consoleInpIdx);

		if (selectedMenu.IsLeaf()) {
			ConsoleClear();
			selectedMenu.CallLeaf();
		}
		else {
			MenuNavigateEnter(selectedMenu);
		}
	}
}

void MenuPage::CallLeaf()
{
	this->leafHandler();
}

void MenuPage::DisplayMenu()
{
	MenuNavigateEnter(*this);
}

MenuPage * MenuPage::GetParent()
{
	return this->parentMenu;
}

ConsoleColor MenuPage::GetMenuTitleColor()
{
	return this->menuTitleColor;
}

ConsoleColor MenuPage::GetMenuItemsColor()
{
	return this->menuItemsColor;
}

void MenuNavBack(void)
{
	g_currentViewingMenuPage->DisplayMenu();
}
