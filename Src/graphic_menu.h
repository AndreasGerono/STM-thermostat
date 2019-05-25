//
// Menu library
//

#include "stm32l0xx_hal.h"




#define SELECTION 0x07
#define NEXT " "
#define MENU_ROWS  7 //number of rows in display
#define MENU_COLUMNS 21 //number of columns in display

typedef void (*menuitemfuncptr)(); //pointer to menu click function (new pointer type)


struct _menuitem {  
  char *text;
  menuitemfuncptr menuitemfunc; //menu click function
  struct _menuitem *parent; //first element of every menu
  struct _menuitem *submenu;  
  struct _menuitem *next; //Menu below, if 0 last Menuitem
};


void Menu_show();
void Menu_selectNext();
void Menu_selectPrev();
void Menu_click();
void Menu_back();
void Menu_init();
void Menu_updateNames();

extern struct _menuitem menu; //new menu struct (forward declaration)
