#include "graphic_menu.h"
#include "graphicDisplay.h"
#include "ssd1306.h"

//Version without rolling mode

static struct _menuitem *currMenuPtr = &menu; //Pointer to current selected Menu item (current drawed)
static int8_t menuindex;                      //Selected menu
static int8_t menufirstpos;                   //Menu on top

static char emptyLine[MENU_COLUMNS] = "                    ";

static uint8_t Menu_getMenuItemsNo() //counts menu items (from one parent)
{
  struct _menuitem *tmpmenuitem = currMenuPtr;
  uint8_t index = 0;
  while (tmpmenuitem) {
    tmpmenuitem = tmpmenuitem->next;
    index++;
  }
  return index;
}

static struct _menuitem *Menu_getMenuItem(uint8_t index) { //
  struct _menuitem *tmpmenuitem = currMenuPtr;

  while ((tmpmenuitem) && (index > 0)) {
    tmpmenuitem = tmpmenuitem->next;
    index--;
  }
  return tmpmenuitem;
}

static uint8_t Menu_getMenuRows() { //Maximum menu elements on LCD
  return MENU_ROWS;
}

void Menu_show() {
  Menu_updateNames();
  struct _menuitem *tmpmenuitem = Menu_getMenuItem(menufirstpos);
  uint8_t menuitemsno = Menu_getMenuItemsNo();
  display_clear(); 

  for (uint8_t i = 0; i < Menu_getMenuRows(); i++) {
    display_setCursor(0, i);              
    if (menuindex == menufirstpos + i)
      display_setFont(&font1);             
    else
      display_setFont(&font2);   
    display_writeText(emptyLine);
    display_setCursor(0, i);
    display_writeText(tmpmenuitem->text);
    if (tmpmenuitem->submenu)
      display_writeText(NEXT); 
    tmpmenuitem = tmpmenuitem->next;
    if (tmpmenuitem == NULL) 
      break;                 
  }
  ssd1306_display();
}

void Menu_selectNext() {
  uint8_t no = Menu_getMenuItemsNo();
  if (menuindex < no - 1) {
    menuindex++;
    if (no > Menu_getMenuRows()) //check if menu number > LCD lines (to move down first position)
    {
      if (menuindex >= Menu_getMenuRows()) //Move down menu when menu index > LCD lines
        menufirstpos++;
    }
    Menu_show(); //
  }
}

void Menu_selectPrev() {
  if (menuindex) {
    if (menuindex == menufirstpos)
      menufirstpos--;
    menuindex--; 
    Menu_show(); 
  }
}

void Menu_back() {
  menufirstpos = 0;
  menuindex = 0;
  currMenuPtr = currMenuPtr->parent;
}

void Menu_click() {
  struct _menuitem *tmpmenuitem = Menu_getMenuItem(menuindex);
  struct _menuitem *submenu = tmpmenuitem->submenu;
  menuitemfuncptr mfptr = tmpmenuitem->menuitemfunc;

  if (mfptr)
    (*mfptr)();
  if (submenu) {
    currMenuPtr = submenu;
    menuindex = 0;
    menufirstpos = 0;
  }
  Menu_show();
}