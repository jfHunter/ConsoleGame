#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Console.h"

#define IS_EXIT_KEY(key_)		        ((key_ == 'q') || (key_ == 'Q') || (key_ == 'e') || (key_ == 'E'))
#define IS_MENU_EXIT_KEY(key_, vec_)	((key_) == vec_.size())
#define CONV_INP_TO_INDEX(key_)			((key_) - (char)49);

#define MENU_INIT(root_) \
	root_.Initialize(NULL)

#define MENU_START(root_) \
	root_.DisplayMenu()

using std::cout;
using std::endl;

using std::vector;
using std::string;

typedef void (* MenuLeafHandler) (void);

class MenuPage
{
private:
	string menuTitle;
	vector<MenuPage> subMenus;
	MenuLeafHandler leafHandler;
	MenuPage * parentMenu;
	bool isLeaf;
	ConsoleColor menuTitleColor;
	ConsoleColor menuItemsColor;

	/* Prints specified menu page */
	void PrintMenuPage(MenuPage menuPage);

	/* Enters specified menu */
	void MenuNavigateEnter(MenuPage menuPage);

	/* Validates selected menu option */
	bool ValidateOption(MenuPage menuPage, int menuIndex);

public:
	/**
	 * Create menu page containing other sub-menus.
	 *
	 * Parameters:
	 *     @param _menuTitle:               title of menu page
	 *     @param _subMenus:                vector of sub-menu items
	 */
	MenuPage(string _menuTitle, ConsoleColor _menuTitleColor, ConsoleColor _menuItemsColor, vector<MenuPage> _subMenus);

	/**
	 * Create menu page without sub-menus (leaf).
	 *
	 * Parameters:
	 *     @param _menuTitle:               title of menu page
	 *     @param _leafHandler:             handler for printing custom menu page
	 */
	MenuPage(string _menuTitle, ConsoleColor _menuTitleColor, MenuLeafHandler _leafHandler);

	/**
	 * Initialize menu.
	 * It sets internal parameters and validates submenu items.
	 * Must be called on root menu page.
	 *
	 * Parameters:
	 *     @param menuPage:                 actual parent menu (must be NULL for root menu)
	 */
	void Initialize(MenuPage * _menuPage);

	/**
	 * Checks if this menu page is leaf.
	 *
	 * Returns:
	 *     true  : if this menu page is leaf
	 *     false : if this menu page contains other branches/sub-menus
	 */
	bool IsLeaf();

	/**
	 * Returns sub-menus vector.
	 *
	 * Returns:
	 *     vector of menu page items which are sub-menus
	 */
	vector<MenuPage>& GetSubMenus();

	/**
	 * Displays root menu.
	 * Must be called on root menu page node
	 */
	void DisplayMenu();

	/**
	 * Returns menu page title.
	 *
	 * Returns:
	 *     menu page title as string
	 */
	string GetTitle();

	/**
	 * Function calls leaf handler.
	 * It displays custom leaf menu page.
	 */
	void CallLeaf();

	/**
	 * Function returns parent menu item.
	 */
	MenuPage * GetParent();

	/**
	 * Function returns menu title color.
	 */
	ConsoleColor GetMenuTitleColor();

	/**
	 * Function returns menu items color.
	 */
	ConsoleColor GetMenuItemsColor();
};

/**
 * Function displays last menu page.
 * It should be called from leaf handler when user
 * wants to go back
 */
void MenuNavBack(void);