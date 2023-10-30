#include "include\includes.h"
#include "include\ee_data.h"
#include "include\mouse.h"
#include "include\ee_deasm.h"
#include "include\ee_dspl.h"
#include "include\ee_com.h"
#include "include\ee_sys.h"
#include "include\ee_file.h"
#include "include\ee_dlg2.h"
#include "include\ee_dlg1.h"
#include "include\ee_init.h"

void main(...)
{
 clrscr();
 printf("\n\rEmulator EPROM 1998/99");
 printf("\n\rAutor: Tomasz Nowakowski\n");
 Start();
 SetPLChar();
 SetKeyb();
 directvideo=0;
 _setcursortype(_NOCURSOR);
 WriteStatus("",0);
 window(1,1,80,25);textcolor(7);textbackground(0);clrscr();
 MenuBar(0,1);
 StatusBar("",0);

/*                          petla glowna programu                           */

do {
 MainWindow(MainWindowSel,MainWindowxSel);
 MainWindowSel=MainWindowPos;
 MainWindowxSel=MainWindowxPos;
 if (AboutTime) _About();
 if (SKey) switch(Key)
 {
  case 59:_Help(1,29,"Instrukcja obsçugi");break;//F1
  case 84:_Help(2,34,"Pamiåci");break;//Shift+F1
  case 61:_FileOpen();break;//F3
  case 85:_SaveAs();break;//Shift+F2
  case 60:strcpy(PathStr,CurPath);
	  strcat(PathStr,FileName);
	  switch (CurFile)
	  {
	   case 0: _SaveAs();break;
	   case 1: SaveHEX(PathStr);break;
	   case 2: SaveBIN(PathStr);break;
	   case 3: SaveASM(PathStr);break;
	  }
	  break;//F2
  case 63:ViewType=1;break;//F5
  case 64:ViewType=2;break;//F6
  case 65:ViewType=3;break;//F7
  case 66:ViewType=4;break;//F8
  case 103:_EECommand(0xA0,0x01,0x02);break;//Ctrl+F10
  case 137:_EECommand(0xB0,0x01,0x02);break;//Ctrl+F11
  case 138:_SimReset();break;//Ctrl+F12
  case 25:MenuNumX=1;SKey=1;Key=67;break;//alt+P
  case 18:MenuNumX=2;SKey=1;Key=67;break;//alt+E
  case 17:MenuNumX=3;SKey=1;Key=67;break;//alt+W
  case 31:MenuNumX=4;SKey=1;Key=67;break;//alt+S
  case 22:MenuNumX=5;SKey=1;Key=67;break;//alt+U
  case 24:MenuNumX=6;SKey=1;Key=67;break;//alt+O
  case 45:Koniec(1);Koniec(0);break;//alt+X
 }
 if (!SKey) switch(Key)
 {
  case 4:ClearBuf(0,0,0,0xFFFF);MakeBufferB();CurFile=0;break;//ctrl+D
  case 6:_Fill();break;//ctrl+F
  case 7:_GoTo();break;//ctrl+G
  case 11:_MemEdit("Kopiuj",0);break;//ctrl+K
  case 18:_MemEdit("Przesué",1);break;//ctrl+S
 }
 OK=0;
 if ((SKey)&&(Key==67))
 {
    do {
      MenuNumX=MenuBar(MenuNumX,0);
      if (!SKey && Key==27) break;
      if (OK) switch (MenuNumX)
      {
      case 1: {
		MenuYPos[MenuNumX-1]=
		Select(Begins[MenuNumX-1]-2,2,31,8,FileMenu,MenuYPos[MenuNumX-1]);
		break;
	      }
      case 2: {
		MenuYPos[MenuNumX-1]=
		Select(Begins[MenuNumX-1]-2,2,37,11,EditMenu,MenuYPos[MenuNumX-1]);
		break;
	      }
      case 3: {
		MenuYPos[MenuNumX-1]=
		Select(Begins[MenuNumX-1]-2,2,43,7,ViewMenu,MenuYPos[MenuNumX-1]);
		break;
	      }
      case 4: {
		MenuYPos[MenuNumX-1]=
		Select(Begins[MenuNumX-1]-2,2,70,10,SymMenu,MenuYPos[MenuNumX-1]);
		break;
	      }
      case 5: {
		MenuYPos[MenuNumX-1]=
		Select(Begins[MenuNumX-1]-2,2,73,10,SetupMenu,MenuYPos[MenuNumX-1]);
		break;
	      }
      case 6: {
		MenuYPos[MenuNumX-1]=
		Select(Begins[MenuNumX-1]-25,2,79,7,HelpMenu,MenuYPos[MenuNumX-1]);
		break;
	      }
      }
      if (SKey && Key==45) break;
    } while (!OK);
    MenuNumY=MenuYPos[MenuNumX-1];
    MenuBar(MenuNumX,1);
    StatusBar("",0);
 }
 if (OK)
 {
  if (MenuNumX==1) switch (MenuNumY) //Menu Plik
  {
   case 1:_FileOpen();break;
   case 2:strcpy(PathStr,CurPath);
	  strcat(PathStr,FileName);
	  switch (CurFile)
	  {
	   case 0: _SaveAs();break;
	   case 1: SaveHEX(PathStr);break;
	   case 2: SaveBIN(PathStr);break;
	   case 3: SaveASM(PathStr);break;
	  }
	  break;
   case 3:_SaveAs();break;
   case 5:Koniec(1);Koniec(0);break;
  }
  if (MenuNumX==2) switch (MenuNumY) //Menu Edycja
  {
   case 1:_MemEdit("Kopiuj",0);break;
   case 2:_MemEdit("Przesué",1);break;
   case 3:_Fill();break;
   case 5:ClearBuf(0,0,0,0xFFFF);MakeBufferB();CurFile=0;break;
   case 6:ClearDEFs();LoadDEF(PathDEF);CurFile=0;break;
   case 8:_GoTo();break;
  }
  if (MenuNumX==3) switch (MenuNumY) //Menu Widok
  {
   case 1:ViewType=1;break;
   case 2:ViewType=2;break;
   case 3:ViewType=3;break;
   case 4:ViewType=4;break;
  }
  if (MenuNumX==4) switch (MenuNumY) //Menu Symulacja
  {
   case 1:_WriteRAM();break;
   case 2:_ReadMEM("RAM",0xB0);break;
   case 3:_ReadMEM("ROM",0xBB);break;
   case 5:_EECommand(0xA0,0x01,0x02);break;
   case 6:_EECommand(0xB0,0x01,0x02);break;
   case 7:_SimReset();break;
  }
  if (MenuNumX==5) switch (MenuNumY) //Menu Ustawienia
  {
   case 1:_SetupCOM();break;
   case 2:_SetupTransmision();break;
   case 3:_SetupEPROM();break;
   case 5:_SetupColors();break;
   case 7:_SaveSetup();break;
  }
  if (MenuNumX==6) switch (MenuNumY) //Menu Pomoc
  {
   case 1:_Help(1,29,"Instrukcja obsçugi");break;
   case 2:_Help(2,34,"Pamiåci");break;
   case 4:_About();break;
  }
 }
} while (!(SKey&&Key==45));
Koniec(1);
Koniec(0);
}