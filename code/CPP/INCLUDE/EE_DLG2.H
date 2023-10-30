/*                        OKIENKA DIALOGOWE                                 */
void  _FileOpen(void)
{
 unsigned char Sel;
 unsigned int offset=0,fpos,f,fold,dpos,d,dold;
 char err;

 SaveScreen();
 DlgWindow(14,4,67,21);
 textattr(Kolory[11]);
 gotoxy(20,1);cprintf(" Otwèrz plik ");
 textattr(Kolory[10]);
 gotoxy(21,3);cprintf("Nazwa :");
 gotoxy(4,3);cprintf("Dysk :");
 gotoxy(37,10);cprintf("Przesuniåcie :");
 gotoxy(43,11);cprintf("h");
 gotoxy(4,5);cprintf("Pliki :");
 gotoxy(21,5);cprintf("Katalogi :");
 DlgList(20,6,35,17);
 DlgList(3,6,18,17);
 f=1;fold=1;fpos=0;d=1;dold=1;dpos=0;FileMax=0;DirMax=0;
 fnsplit(CurPath,DriveStr,DirStr,FileStr,ExtStr);
 fnmerge(PathStr,DriveStr,DirStr,"","");
 GetFiles(PathStr,CurMask);
 textattr(Kolory[7]);
 gotoxy(30,3);cprintf("%-12s",CurMask);
 gotoxy(12,3);cprintf("%s",DriveStr);
 gotoxy(38,11);print__HEX(offset,"");
 window(1,1,80,25);
 Button(53,16,61,0,"Otwèrz");
 Button(53,19,61,0,_Anuluj);
 window(15,4,66,21);
 Key=0;SKey=0;Sel=1;
 textattr(Kolory[10]);
 printFilepage(fpos);
 printDirpage(dpos);
 do {
 textattr(Kolory[7]);
 switch (Sel)
 {
  case 1: gotoxy(30,3);
	  if (ReadKeybPath(12,0))
	  {
	   if (strpbrk(StrA,"*?"))
	   {
	    FileMax=0;DirMax=0;
	    strcpy(CurMask,StrA);
	    GetFiles(PathStr,CurMask);
	    textattr(Kolory[10]);
	    printFilepage(fpos);
	    printDirpage(dpos);
	    if ((!SKey)&&(Key==ENTER)) Sel=2;
	    if (FileMax==0) Sel=1;
	    if ((!SKey)&&(Key==TAB)) Sel=2;
	    if ((SKey)&&(Key==STAB)) Sel=6;
	    break;
	   }
	   if (StrA[1]==':')
	   {
	    StrA[2]=0;
	    if ( (StrA[0]-'A') <= setdisk(getdisk()) )
	    {
	     PathStr[0]=StrA[0];
	     gotoxy(12,3);cprintf("%s",StrA);
	     gotoxy(30,3);cprintf("%-12s",CurMask);
	     fnmerge(PathStr,StrA,"\\","","");
	     FileMax=0;DirMax=0;
	     GetFiles(PathStr,CurMask);
	     textattr(Kolory[10]);
	     printFilepage(fpos);
	     printDirpage(dpos);
	     if ((!SKey)&&(Key==ENTER)) Sel=3;
	     if (FileMax==0) Sel=1;
	     if ((!SKey)&&(Key==TAB)) Sel=2;
	     if ((SKey)&&(Key==STAB)) Sel=6;
	    }
	    else {
	     gotoxy(30,3);cprintf("%-12s",SPACE);
	     Sel=1;
	    }
	    break;
	   }
	   else
	   {
	    f=1;fold=1;fpos=0;d=1;dold=1;dpos=0;FileMax=0;DirMax=0;
	    strcpy(FileStr,StrA);
	    GetFiles(PathStr,FileStr);
	    textattr(Kolory[10]);
	    printFilepage(fpos);
	    printDirpage(dpos);
	    if ((!SKey)&&(Key==ENTER)) Sel=2;
	    if (FileMax==0) Sel=1;
	    if ((!SKey)&&(Key==TAB)) Sel=2;
	    if ((SKey)&&(Key==STAB)) Sel=6;
	   }
	   break;
	  }
	  if (FileMax==0) Sel=1;
	  if ((!SKey)&&(Key==TAB)) Sel=2;
	  if ((SKey)&&(Key==STAB)) Sel=6;
	  break;
  case 2: if (FileMax==0) { Sel=3;break; }
	  do {
	     ChangeColor(17,f+9,30,Kolory[7]);
	     while (!kbhit()) {}
	     if (kbhit()) Key=getch();
	     if (Key==0) { SKey=1;Key=getch();}
	     else SKey=0;
	     if (!SKey)
	     {
	      if (Key==TAB)
	      {
	       printFileName(fpos,f);
	       Sel=3;break;
	      }//tab
	      if (Key==ENTER)
	      {
	       printFileName(fpos,f);
	       Sel=4;break;
	      }//enter
	      if (Key==ESC) break;//esc
	     }
	     if (SKey)
	     {
	      if (Key==STAB)
	      {
	       printFileName(fpos,f);
	       Sel=1;break;
	      }//Shift+TAB
	      if (Key==PGDOWN)//pgdn
	      {
	       if (FileMax>10)
		if (fpos+10<FileMax-10) fpos+=10;
		else fpos=FileMax-10;
	       textattr(Kolory[10]);
	       printFilepage(fpos);
	      }
	      if (Key==PGUP)//pgup
	      {
	       if (fpos>10) fpos-=10;
	       else fpos=0;
	       textattr(Kolory[10]);
	       printFilepage(fpos);
	      }
	      if (Key==DOWN)//dn
	      {
	       if (f<FileMax) f++;
	       if (f>10) { if (fpos<FileMax-10) fpos++;f=10; }
	       textattr(Kolory[10]);
	       printFilepage(fpos);
	      }
	      if (Key==UP)//up
	      {
	       f--;if (f<1) { if (fpos>0) fpos--;f=1; }
	       textattr(Kolory[10]);
	       printFilepage(fpos);
	      }
	     }
	     ChangeColor(17,fold+9,30,Kolory[10]);
	     fold=f;
	  } while ( !(Key==45 && SKey) );
	  ChangeColor(17,fold+9,30,Kolory[10]);
	  fold=f;
	  break;
  case 3: if (DirMax==0) { Sel=4;break; }
	  do {
	     ChangeColor(34,d+9,47,Kolory[7]);
	     while (!kbhit()) {}
	     if (kbhit()) Key=getch();
	     if (Key==0) { SKey=1;Key=getch();}
	     else SKey=0;
	     if (!SKey)
	     {
	      if (Key==TAB) { Sel=4;break; }//tab
	      if (Key==ENTER)
	      {
	       Sel=3;
	       strcpy(StrA,DirList[(dpos+d)-1]);
	       if (stricmp(StrA,"..")==0)
	       {
		for (Byte=strlen(PathStr)-2;Byte>0;Byte--)
		{
		 if (PathStr[Byte]=='\\') { PathStr[Byte+1]=0;break; }
		}
	       }
	       else {
		fnsplit(PathStr,DriveStr,DirStr,FileStr,ExtStr);
		strcat(DirStr,StrA);
		fnmerge(PathStr,DriveStr,DirStr,"","");
	       }
	       FileMax=0;DirMax=0;
	       ChangeColor(34,dold+9,47,Kolory[10]);
	       GetFiles(PathStr,CurMask);
	       f=1;fold=1;fpos=0;d=1;dold=1;dpos=0;
	       textattr(Kolory[10]);
	       printFilepage(fpos);
	       printDirpage(dpos);
	       break;
	      }//enter
	      if (Key==ESC) break;//esc
	     }
	     if (SKey)
	     {
	      if (Key==STAB) { Sel=2;break; }//Shift+TAB
	      if (Key==PGDOWN)//pgdn
	      {
	       textattr(Kolory[10]);
	       if (DirMax>10)
		if (dpos+10<DirMax-10) dpos+=10;
		else dpos=DirMax-10;
	       printDirpage(dpos);
	      }
	      if (Key==PGUP)//pgup
	      {
	       textattr(Kolory[10]);
	       if (dpos>10) dpos-=10;
	       else dpos=0;
	       printDirpage(dpos);
	      }
	      if (Key==DOWN)//dn
	      {
	       textattr(Kolory[10]);
	       if (d<DirMax) d++;
	       if (d>10) { if (dpos<DirMax-10) dpos++;d=10; }
	       printDirpage(dpos);
	      }
	      if (Key==UP)//up
	      {
	       textattr(Kolory[10]);
	       d--;if (d<1) { if (dpos>0) dpos--;d=1; }
	       printDirpage(dpos);
	      }
	     }
	     ChangeColor(34,dold+9,47,Kolory[10]);
	     dold=d;
	  } while ( !(Key==45 && SKey) );
	  ChangeColor(34,dold+9,47,Kolory[10]);
	  dold=d;
	  break;
  case 4: gotoxy(38,11);
	  if (ReadKeybHEX(4,0))
	  {
	   offset=strtoul(StrA,NULL,16);
	   textattr(Kolory[7]);
	   gotoxy(38,11);print__HEX(offset,"");
	  }
	  if ((!SKey)&&(Key==TAB)) Sel=5;
	  if ((!SKey)&&(Key==ENTER)) Sel=5;
	  if ((SKey)&&(Key==STAB)) Sel=3;
	  break;
  case 5: window(1,1,80,25);
	  if (Button(53,16,61,255,"Otwèrz"))
	  {
	   if (strlen(FileName)==0)
	   {
	    _DialogW("Uwaga !",ErrorStr[5],"");
	    Sel=1;window(15,4,66,21);
	    break;
	   }
	   strcpy(CurPath,PathStr);
	   fnsplit(FileName,DriveStr,DirStr,FileStr,ExtStr);
	   if ( stricmp(ExtStr,".HEX")==0 )
	   {
	    strcpy(PathStr,CurPath);strcat(PathStr,FileName);
	    LoadHEX(PathStr);
	    strcpy(PathStr,CurPath);
	    strcat(PathStr,FileStr);strcat(PathStr,".DEF");
	    err=LoadDEF(PathStr);
	    if ( (err!=0)&&(err!=-3) ) _DialogW(_Uwaga,ErrorStr[6],"");
	   }
	   if ( stricmp(ExtStr,".DEF")==0 )
	   {
	    strcpy(PathStr,CurPath);strcat(PathStr,FileName);
	    if (LoadDEF(PathStr)!=0) _DialogW(_Uwaga,ErrorStr[6],"");
	   }
	   if ( stricmp(ExtStr,".DEF")!=0 && stricmp(ExtStr,".HEX")!=0 )
	   {
	    strcpy(PathStr,CurPath);strcat(PathStr,FileName);
	    LoadBIN(PathStr,offset);
	    strcpy(PathStr,CurPath);
	    strcat(PathStr,FileStr);strcat(PathStr,".DEF");
	    err=LoadDEF(PathStr);
	    if ( (err!=0)&&(err!=-3) ) _DialogW(_Uwaga,ErrorStr[6],"");
	   }
	   Refresh=1;SKey=0;Key=27;
	  }
	  if ((!SKey)&&(Key==TAB)) Sel=6;
	  if ((SKey)&&(Key==STAB)) Sel=4;
	  window(15,4,66,21);
	  break;
  case 6: window(1,1,80,25);
	  if (Button(53,19,61,255,_Anuluj))
	  {
	   strcpy(CurPath,PathStr);
	   SKey=0;Key=27;
	  }
	  if ((!SKey)&&(Key==TAB)) Sel=1;
	  if ((SKey)&&(Key==STAB)) Sel=5;
	  window(15,4,66,21);
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}

void  _SaveAs(void)
{
 unsigned char Sel;
 unsigned int fpos,f,fold,dpos,d,dold;
 char err;

 SaveScreen();
 DlgWindow(14,4,67,21);
 textattr(Kolory[11]);
 gotoxy(17,1);cprintf(" Zachowaj plik jako ");
 textattr(Kolory[10]);
 gotoxy(21,3);cprintf("Nazwa :");
 gotoxy(4,3);cprintf("Dysk :");
 gotoxy(4,5);cprintf("Pliki :");
 gotoxy(21,5);cprintf("Katalogi :");
 DlgList(20,6,35,17);
 DlgList(3,6,18,17);
 f=1;fold=1;fpos=0;d=1;dold=1;dpos=0;FileMax=0;DirMax=0;
 fnsplit(CurPath,DriveStr,DirStr,FileStr,ExtStr);
 fnmerge(PathStr,DriveStr,DirStr,"","");
 GetFiles(PathStr,CurMask);
 textattr(Kolory[7]);
 gotoxy(30,3);cprintf("%-12s",CurMask);
 gotoxy(12,3);cprintf("%s",DriveStr);
 window(1,1,80,25);
 Button(53,16,61,0,"Zapisz");
 Button(53,19,61,0,_Anuluj);
 window(15,4,66,21);
 Key=0;SKey=0;Sel=1;
 textattr(Kolory[10]);
 printFilepage(fpos);
 printDirpage(dpos);
 do {
 textattr(Kolory[7]);
 switch (Sel)
 {
  case 1: gotoxy(30,3);
	  if (ReadKeybPath(12,0))
	  {
	   if (strpbrk(StrA,"*?"))
	   {
	    FileMax=0;DirMax=0;
	    FileName[0]=0;
	    strcpy(CurMask,StrA);
	    GetFiles(PathStr,CurMask);
	    textattr(Kolory[10]);
	    printFilepage(fpos);
	    printDirpage(dpos);
	    if ((!SKey)&&(Key==ENTER)) Sel=2;
	    if (FileMax==0) Sel=1;
	    if ((!SKey)&&(Key==TAB)) Sel=2;
	    if ((SKey)&&(Key==STAB)) Sel=5;
	    break;
	   }
	   if (StrA[1]==':')
	   {
	    StrA[2]=0;
	    if ( (StrA[0]-'A') <= setdisk(getdisk()) )
	    {
	     PathStr[0]=StrA[0];
	     gotoxy(12,3);cprintf("%s",StrA);
	     gotoxy(30,3);cprintf("%-12s",CurMask);
	     fnmerge(PathStr,StrA,"\\","","");
	     FileName[0]=0;
	     FileMax=0;DirMax=0;
	     GetFiles(PathStr,CurMask);
	     textattr(Kolory[10]);
	     printFilepage(fpos);
	     printDirpage(dpos);
	     if ((!SKey)&&(Key==ENTER)) Sel=3;
	     if (FileMax==0) Sel=1;
	     if ((!SKey)&&(Key==TAB)) Sel=2;
	     if ((SKey)&&(Key==STAB)) Sel=5;
	    }
	    else {
	     gotoxy(30,3);cprintf("%-12s",SPACE);
	     Sel=1;
	    }
	    break;
	   }
	   else
	   {
	    f=1;fold=1;fpos=0;d=1;dold=1;dpos=0;FileMax=0;DirMax=0;
	    strcpy(FileStr,StrA);
	    strcpy(FileName,StrA);
	    GetFiles(PathStr,FileStr);
	    textattr(Kolory[10]);
	    printFilepage(fpos);
	    printDirpage(dpos);
	    if ((!SKey)&&(Key==ENTER)) Sel=2;
	    if (FileMax==0) Sel=4;
	    if ((!SKey)&&(Key==TAB)) Sel=2;
	    if ((SKey)&&(Key==STAB)) Sel=5;
	   }
	   break;
	  }
	  if (FileMax==0) Sel=4;
	  if ((!SKey)&&(Key==TAB)) Sel=2;
	  if ((SKey)&&(Key==STAB)) Sel=5;
	  break;
  case 2: if (FileMax==0) { Sel=3;break; }
	  do {
	     ChangeColor(17,f+9,30,Kolory[7]);
	     while (!kbhit()) {}
	     if (kbhit()) Key=getch();
	     if (Key==0) { SKey=1;Key=getch();}
	     else SKey=0;
	     if (!SKey)
	     {
	      if (Key==TAB)
	      {
	       printFileName(fpos,f);
	       Sel=3;break;
	      }//tab
	      if (Key==ENTER)
	      {
	       printFileName(fpos,f);
	       Sel=4;break;
	      }//enter
	      if (Key==ESC) break;//esc
	     }
	     if (SKey)
	     {
	      if (Key==STAB)
	      {
	       printFileName(fpos,f);
	       Sel=1;break;
	      }//Shift+TAB
	      if (Key==PGDOWN)//pgdn
	      {
	       if (FileMax>10)
		if (fpos+10<FileMax-10) fpos+=10;
		else fpos=FileMax-10;
	       textattr(Kolory[10]);
	       printFilepage(fpos);
	      }
	      if (Key==PGUP)//pgup
	      {
	       if (fpos>10) fpos-=10;
	       else fpos=0;
	       textattr(Kolory[10]);
	       printFilepage(fpos);
	      }
	      if (Key==DOWN)//dn
	      {
	       if (f<FileMax) f++;
	       if (f>10) { if (fpos<FileMax-10) fpos++;f=10; }
	       textattr(Kolory[10]);
	       printFilepage(fpos);
	      }
	      if (Key==UP)//up
	      {
	       f--;if (f<1) { if (fpos>0) fpos--;f=1; }
	       textattr(Kolory[10]);
	       printFilepage(fpos);
	      }
	     }
	     ChangeColor(17,fold+9,30,Kolory[10]);
	     fold=f;
	  } while ( !(Key==45 && SKey) );
	  ChangeColor(17,fold+9,30,Kolory[10]);
	  fold=f;
	  break;
  case 3: if (DirMax==0) { Sel=4;break; }
	  do {
	     ChangeColor(34,d+9,47,Kolory[7]);
	     while (!kbhit()) {}
	     if (kbhit()) Key=getch();
	     if (Key==0) { SKey=1;Key=getch();}
	     else SKey=0;
	     if (!SKey)
	     {
	      if (Key==TAB) { Sel=4;break; }//tab
	      if (Key==ENTER)
	      {
	       Sel=3;
	       strcpy(StrA,DirList[(dpos+d)-1]);
	       if (stricmp(StrA,"..")==0)
	       {
		for (Byte=strlen(PathStr)-2;Byte>0;Byte--)
		{
		 if (PathStr[Byte]=='\\') { PathStr[Byte+1]=0;break; }
		}
	       }
	       else {
		fnsplit(PathStr,DriveStr,DirStr,FileStr,ExtStr);
		strcat(DirStr,StrA);
		fnmerge(PathStr,DriveStr,DirStr,"","");
	       }
	       FileMax=0;DirMax=0;
	       ChangeColor(34,dold+9,47,Kolory[10]);
	       GetFiles(PathStr,CurMask);
	       f=1;fold=1;fpos=0;d=1;dold=1;dpos=0;
	       textattr(Kolory[10]);
	       printFilepage(fpos);
	       printDirpage(dpos);
	       break;
	      }//enter
	      if (Key==ESC) break;//esc
	     }
	     if (SKey)
	     {
	      if (Key==STAB) { Sel=2;break; }//Shift+TAB
	      if (Key==PGDOWN)//pgdn
	      {
	       if (DirMax>10)
		if (dpos+10<DirMax-10) dpos+=10;
		else dpos=DirMax-10;
	       textattr(Kolory[10]);
	       printDirpage(dpos);
	      }
	      if (Key==PGUP)//pgup
	      {
	       if (dpos>10) dpos-=10;
	       else dpos=0;
	       textattr(Kolory[10]);
	       printDirpage(dpos);
	      }
	      if (Key==DOWN)//dn
	      {
	       if (d<DirMax) d++;
	       if (d>10) { if (dpos<DirMax-10) dpos++;d=10; }
	       textattr(Kolory[10]);
	       printDirpage(dpos);
	      }
	      if (Key==UP)//up
	      {
	       d--;if (d<1) { if (dpos>0) dpos--;d=1; }
	       textattr(Kolory[10]);
	       printDirpage(dpos);
	      }
	     }
	     ChangeColor(34,dold+9,47,Kolory[10]);
	     dold=d;
	  } while ( !(Key==45 && SKey) );
	  ChangeColor(34,dold+9,47,Kolory[10]);
	  dold=d;
	  break;
  case 4: window(1,1,80,25);
	  if (Button(53,16,61,255,"Zapisz"))
	  {
	   if (strlen(FileName)==0)
	   {
	    _DialogW(_Uwaga,ErrorStr[5],"");
	    Sel=1;window(15,4,66,21);
	    break;
	   }
	   strcpy(CurPath,PathStr);
	   fnsplit(FileName,DriveStr,DirStr,FileStr,ExtStr);
	   if ( stricmp(ExtStr,".HEX")==0 )
	   {
	    strcpy(PathStr,CurPath);strcat(PathStr,FileName);
	    SaveHEX(PathStr);
	   }
	   else if ( stricmp(ExtStr,".ASM")==0 )
	   {
	    strcpy(PathStr,CurPath);strcat(PathStr,FileName);
	    SaveASM(PathStr);
	   }
	   else {
	    strcpy(PathStr,CurPath);strcat(PathStr,FileName);
	    SaveBIN(PathStr);
	   }
	   Refresh=1;SKey=0;Key=27;
	  }
	  if ((!SKey)&&(Key==TAB)) Sel=5;
	  if ((SKey)&&(Key==STAB)) Sel=3;
	  window(15,4,66,21);
	  break;
  case 5: window(1,1,80,25);
	  if (Button(53,19,61,255,_Anuluj))
	  {
	   strcpy(CurPath,PathStr);
	   SKey=0;Key=27;
	  }
	  if ((!SKey)&&(Key==TAB)) Sel=1;
	  if ((SKey)&&(Key==STAB)) Sel=4;
	  window(15,4,66,21);
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}

void  _SetupCOM(void)
{
 unsigned char Sel,q,num,irq;
 unsigned int addr,speed1,speed2;

 SaveScreen();
 DlgWindow(21,5,59,17);
 textattr(Kolory[11]);
 gotoxy(8,1);cprintf(" Ustawienia portu COM ");
 textattr(Kolory[10]);
 gotoxy(3,3);cprintf("Numer portu    :");
 gotoxy(3,4);cprintf("Przerwanie IRQ :");
 gotoxy(3,5);cprintf("Adres portu    :");
 gotoxy(3,7);cprintf("Prådkoêã transmisjii :");
 gotoxy(4,8);cprintf("Maksymalna");
 gotoxy(4,9);cprintf("Minimalna");
 gotoxy(26,5);cprintf("h");
 gotoxy(22,8);cprintf("bps");
 gotoxy(22,9);cprintf("bps");
 textattr(Kolory[7]);
 speed1=Speed_1;speed2=Speed_2;
 num=COMNum;irq=COMIRQ;addr=COMAddr;
 gotoxy(21,3);cprintf("%1u",num);
 gotoxy(21,4);cprintf("%2u",irq);
 gotoxy(21,5);print__HEX(addr,"");
 gotoxy(16,8);cprintf("%5u",speed1);
 gotoxy(16,9);cprintf("%5u",speed2);
 window(1,1,80,25);
 Button(27,15,35,0,_OK);
 Button(44,15,52,0,_Anuluj);
 window(22,5,58,16);
 Key=0;SKey=0;Sel=1;q=0;
 do {
 textattr(Kolory[7]);
 switch (Sel)
 {
  case 1: gotoxy(21,3);
	  if (ReadKeyb(1,0))
	  {
	   num=strtoul(StrA,NULL,10);
	   if (num>4) num=4;
	   if (num==0) num=4;
	   addr=GetUARTAddr(num);
	   if ((num==1)||(num==3)) irq=4;
	   if ((num==2)||(num==4)) irq=3;
	   gotoxy(21,3);cprintf("%1u",num);
	   gotoxy(21,4);cprintf("%2u",irq);
	   gotoxy(21,5);print__HEX(addr,"");
	   q=1;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=2;
	  if ((SKey)&&(Key==STAB)) Sel=7;
	  break;
  case 2: gotoxy(21,4);
	  if (ReadKeyb(2,0))
	  {
	   irq=strtoul(StrA,NULL,10);
	   if (irq>15) irq=15;
	   if (irq==0) irq=3;
	   gotoxy(21,4);cprintf("%2u",irq);
	   q=1;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=3;
	  if ((SKey)&&(Key==STAB)) Sel=1;
	  break;
  case 3: gotoxy(21,5);
	  if (ReadKeybHEX(4,0))
	  {
	   addr=strtoul(StrA,NULL,16);
	   if (addr==0) addr=0x02E8;
	   gotoxy(21,5);print__HEX(addr,"");
	   q=1;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=4;
	  if ((SKey)&&(Key==STAB)) Sel=2;
	  break;
  case 4: gotoxy(16,8);
	  if (ReadKeyb(5,0))
	  {
	   speed1=strtoul(StrA,NULL,10);
	   if (speed1<300) speed1=2400;
	   if (speed1>57600) speed1=57600;
	   if (speed1<speed2) speed2=speed1;
	   gotoxy(16,8);cprintf("%5u",speed1);
	   gotoxy(16,9);cprintf("%5u",speed2);
	   q=1;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=5;
	  if ((SKey)&&(Key==STAB)) Sel=3;
	  break;
  case 5: gotoxy(16,9);
	  if (ReadKeyb(5,0))
	  {
	   speed2=strtoul(StrA,NULL,10);
	   if (speed2<300) speed2=2400;
	   if (speed2>57600) speed2=57600;
	   if (speed2>speed1) speed1=speed2;
	   gotoxy(16,8);cprintf("%5u",speed1);
	   gotoxy(16,9);cprintf("%5u",speed2);
	   q=1;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=6;
	  if ((SKey)&&(Key==STAB)) Sel=4;
	  break;
  case 6: window(1,1,80,25);
	  if (Button(27,15,35,255,_OK))
	  {
	   if (q==1)
	   {
	    COMNum=num;COMIRQ=irq;COMAddr=addr;
	    Speed_2=speed2;Speed_1=speed1;
	    SetIRQProc(COMIRQ);InitUART(COMNum,9600);
	   }
	   SKey=0;Key=27;
	  }
	  window(22,5,58,16);
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=7;
	  if ((SKey)&&(Key==STAB)) Sel=5;
	  break;
  case 7: window(1,1,80,25);
	  if (Button(44,15,52,255,_Anuluj)) { SKey=0;Key=27;}
	  window(22,5,58,16);
	  Sel=1;
	  if ((!SKey)&&(Key==TAB)) Sel=1;
	  if ((SKey)&&(Key==STAB)) Sel=6;
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}

void  _SetupTransmision(void)
{
 unsigned char Sel,q,num,irq;
 unsigned char maxsize,rep;
 unsigned int time;

 SaveScreen();
 DlgWindow(19,7,61,16);
 textattr(Kolory[11]);
 gotoxy(9,1);cprintf(" Ustawienia transmisji ");
 textattr(Kolory[10]);
 gotoxy(3,3);cprintf("Rozmiar pakietu danych    :");
 gotoxy(3,4);cprintf("Ponawiaj przy bçådzie CRC :");
 gotoxy(3,5);cprintf("Czas oczekiwania na dane  :");
 gotoxy(35,3);cprintf("B");
 gotoxy(37,5);cprintf("ms");
 textattr(Kolory[7]);
 time=MaxTimeOut*55;rep=MaxCRCchecks;maxsize=MaxSize;
 gotoxy(32,3);cprintf("%2u",maxsize);
 gotoxy(32,4);cprintf("%2u",rep);
 gotoxy(32,5);cprintf("%4u",time);
 window(1,1,80,25);
 Button(27,14,35,0,_OK);
 Button(44,14,52,0,_Anuluj);
 window(20,7,60,15);
 Key=0;SKey=0;Sel=1;q=0;
 do {
 textattr(Kolory[7]);
 switch (Sel)
 {
  case 1: gotoxy(32,3);
	  if (ReadKeyb(2,0))
	  {
	   maxsize=strtoul(StrA,NULL,10);
	   if (maxsize>64) maxsize=64;
	   if (maxsize==0) maxsize=8;
	   gotoxy(32,3);cprintf("%2u",maxsize);
	   q=1;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=2;
	  if ((SKey)&&(Key==STAB)) Sel=5;
	  break;
  case 2: gotoxy(32,4);
	  if (ReadKeyb(2,0))
	  {
	   rep=strtoul(StrA,NULL,10);
	   if (rep>50) rep=50;
	   if (rep==0) rep=5;
	   gotoxy(32,4);cprintf("%2u",rep);
	   q=1;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=3;
	  if ((SKey)&&(Key==STAB)) Sel=1;
	  break;
  case 3: gotoxy(32,5);
	  if (ReadKeyb(4,0))
	  {
	   time=strtoul(StrA,NULL,10);
	   if (time>5500) time=5500;
	   if (time==0) time=110;
	   gotoxy(32,5);cprintf("%4u",time);
	   q=1;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=4;
	  if ((SKey)&&(Key==STAB)) Sel=2;
	  break;
  case 4: window(1,1,80,25);
	  if (Button(27,14,35,255,_OK))
	  {
	   if (q==1)
	   {
	    MaxSize=maxsize;MaxCRCchecks=rep;MaxTimeOut=time/55;
	   }
	   SKey=0;Key=27;
	  }
	  window(20,7,60,15);
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=5;
	  if ((SKey)&&(Key==STAB)) Sel=3;
	  break;
  case 5: window(1,1,80,25);
	  if (Button(44,14,52,255,_Anuluj)) { SKey=0;Key=27;}
	  window(20,7,60,15);
	  Sel=1;
	  if ((!SKey)&&(Key==TAB)) Sel=1;
	  if ((SKey)&&(Key==STAB)) Sel=4;
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}

void  _SetupEPROM(void)
{
 unsigned char Sel,q;
 unsigned long size;

 SaveScreen();
 DlgWindow(21,7,59,14);
 textattr(Kolory[11]);
 gotoxy(6,1);cprintf(" Ustawienia pamiåci EPROM ");
 textattr(Kolory[10]);
 gotoxy(3,3);cprintf("Wielkoêã pamiåci EPROM :");
 gotoxy(35,3);cprintf("B");
 textattr(Kolory[7]);
 size=MaxbufferA;
 ltoa(size+1,StrA,10);
 gotoxy(29,3);cprintf("%5s",StrA);
 window(1,1,80,25);
 Button(27,12,35,0,_OK);
 Button(44,12,52,0,_Anuluj);
 window(22,7,58,13);
 Key=0;SKey=0;Sel=1;q=0;
 do {
 textattr(Kolory[7]);
 switch (Sel)
 {
  case 1: gotoxy(29,3);
	  if (ReadKeyb(5,0))
	  {
	   size=strtoul(StrA,NULL,10);
	   if (size<2048) size=2048;
	   if (size>65536) size=65536;
	   if ((size%16)>0) size+=16-(size%16);
	   ltoa(size,StrA,10);
	   gotoxy(29,3);cprintf("%5s",StrA);
	   q=1;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=2;
	  if ((SKey)&&(Key==STAB)) Sel=3;
	  break;
  case 2: window(1,1,80,25);
	  if (Button(27,12,35,255,_OK))
	  {
	   if (q==1)
	   {
	    size--;MaxbufferA=size;
	    MakeBufferB();
	    UpAddr2=0;UpAddr3=0;UpAddr4=0;
	   }
	   SKey=0;Key=27;
	  }
	  window(22,7,58,13);
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=3;
	  if ((SKey)&&(Key==STAB)) Sel=1;
	  break;
  case 3: window(1,1,80,25);
	  if (Button(44,12,52,255,_Anuluj)) { SKey=0;Key=27;}
	  window(22,7,58,13);
	  Sel=1;
	  if ((!SKey)&&(Key==TAB)) Sel=1;
	  if ((SKey)&&(Key==STAB)) Sel=2;
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}

void  _SetupColors(void)
{
 unsigned char Sel,q,y,yold,c,cold,b,bold,elem;

 SaveScreen();
 DlgWindow(17,3,63,23);
 if (NewKolory[0]!=255) for (q=0;q<=15;q++) NewKolory[q]=Kolory[q];
 textattr(Kolory[11]);
 gotoxy(13,1);cprintf(" Ustawienia kolorèw ");
 textattr(Kolory[10]);
 DlgList(3,2,30,17);
 gotoxy(5,3); cprintf("Menu gçèwne");
 gotoxy(5,4); cprintf("Menu rozwijane");
 gotoxy(5,5); cprintf("Menu zaznaczone");
 gotoxy(5,6); cprintf("Ramka okna gçèwnego");
 gotoxy(5,7); cprintf("Tekst w oknie gçèwnym");
 gotoxy(5,8); cprintf("Adres w oknie gçèwnym");
 gotoxy(5,9); cprintf("Etykieta w oknie gçèwnym");
 gotoxy(5,10);cprintf("Ramka okna dialogowego");
 gotoxy(5,11);cprintf("Tekst w oknie dialogowym");
 gotoxy(5,12);cprintf("Zaznaczenie");
 gotoxy(5,13);cprintf("Pasek statusu nieaktywny");
 gotoxy(5,14);cprintf("Pasek statusu aktywny");
 gotoxy(5,15);cprintf("Przycisk aktywny");
 gotoxy(5,16);cprintf("Przycisk nieaktywny");
 ColTable(32,3);
 window(1,1,80,25);
 Button(27,21,35,0,_OK);
 Button(44,21,52,0,_Anuluj);
 window(18,3,62,22);
 Key=0;SKey=0;Sel=1;q=0;y=1;yold=1;
 c=0;cold=0;b=0;bold=0;elem=0;
 do {
 textattr(Kolory[7]);
 switch (Sel)
 {
  case 1:
	 do {
	     ChangeColor(20,y+4,45,Kolory[7]);
	     switch (y)
	     {
	      case  1:textattr(NewKolory[1]);elem=1;break;
	      case  2:textattr(NewKolory[2]);elem=2;break;
	      case  3:textattr(NewKolory[3]);elem=3;break;
	      case  4:textattr(NewKolory[6]);elem=6;break;
	      case  5:textattr(NewKolory[5]);elem=5;break;
	      case  6:textattr(NewKolory[8]);elem=8;break;
	      case  7:textattr(NewKolory[9]);elem=9;break;
	      case  8:textattr(NewKolory[11]);elem=11;break;
	      case  9:textattr(NewKolory[10]);elem=10;break;
	      case 10:textattr(NewKolory[7]);elem=7;break;
	      case 11:textattr(NewKolory[4]);elem=4;break;
	      case 12:textattr(NewKolory[14]);elem=14;break;
	      case 13:textattr(NewKolory[13]);elem=13;break;
	      case 14:textattr(NewKolory[12]);elem=12;break;
	     }
	     gotoxy(32,13);cprintf(" Przykçad ");
	     c=(NewKolory[elem]&0x0F);b=(NewKolory[elem]&0xF0)>>4;
	     bold=b;cold=c;
	     while (!kbhit()) {}
	     if (kbhit()) Key=getch();
	     if (Key==0) { SKey=1;Key=getch();}
	     else SKey=0;
	     if (!SKey)
	     {
	      if (Key==TAB) break;//tab
	      if (Key==ENTER) break;//enter
	      if (Key==ESC) break;//esc
	     }
	     if (SKey)
	     {
	      if (Key==STAB) { Sel=4;break; }
	      if (Key==DOWN)//dn
	      {
	       y++;if (y>14) y=14;
	      }
	      if (Key==UP)//up
	      {
	       y--;if (y<1) y=1;
	      }
	     }
	     ChangeColor(20,yold+4,45,Kolory[10]);
	     yold=y;
	  } while ( !(Key==45 && SKey) );
	  ChangeColor(20,yold+4,45,Kolory[10]);
	  yold=y;Sel++;
	  break;
  case 2:
	 do {
	     Byte=(b<<4)|c;NewKolory[elem]=Byte;
	     textattr(Byte);gotoxy(32,13);cprintf(" Przykçad ");
	     textbackground(c);textcolor(c+8);
	     switch (c)
	     {
	      case  0:gotoxy(33,4);break;
	      case  1:gotoxy(36,4);break;
	      case  2:gotoxy(39,4);break;
	      case  3:gotoxy(42,4);break;
	      case  4:gotoxy(33,5);break;
	      case  5:gotoxy(36,5);break;
	      case  6:gotoxy(39,5);break;
	      case  7:gotoxy(42,5);break;
	      case  8:gotoxy(33,6);break;
	      case  9:gotoxy(36,6);break;
	      case 10:gotoxy(39,6);break;
	      case 11:gotoxy(42,6);break;
	      case 12:gotoxy(33,7);break;
	      case 13:gotoxy(36,7);break;
	      case 14:gotoxy(39,7);break;
	      case 15:gotoxy(42,7);break;
	     }
	     putch(149);
	     while (!kbhit()) {}
	     if (kbhit()) Key=getch();
	     if (Key==0) { SKey=1;Key=getch();}
	     else SKey=0;
	     textcolor(cold);
	     switch (cold)
	     {
	      case  0:gotoxy(33,4);break;
	      case  1:gotoxy(36,4);break;
	      case  2:gotoxy(39,4);break;
	      case  3:gotoxy(42,4);break;
	      case  4:gotoxy(33,5);break;
	      case  5:gotoxy(36,5);break;
	      case  6:gotoxy(39,5);break;
	      case  7:gotoxy(42,5);break;
	      case  8:gotoxy(33,6);break;
	      case  9:gotoxy(36,6);break;
	      case 10:gotoxy(39,6);break;
	      case 11:gotoxy(42,6);break;
	      case 12:gotoxy(33,7);break;
	      case 13:gotoxy(36,7);break;
	      case 14:gotoxy(39,7);break;
	      case 15:gotoxy(42,7);break;
	     }
	     putch(219);
	     if (SKey)
	     {
	      if (Key==STAB) { Sel=0;break; }
	      if (Key==DOWN)//dn
	      {
	       if (c<12) c+=4;q=1;
	      }
	      if (Key==UP)//up
	      {
	       if (c>=4) c-=4;q=1;
	      }
	      if (Key==LEFT)//left
	      {
	       if (c>0) c--;q=1;
	      }
	      if (Key==RIGHT)//right
	      {
	       c++;if (c>15) c=15;q=1;
	      }
	     }
	     if (!SKey)
	     {
	      if (Key==TAB) break;//tab
	      if (Key==ENTER) break;//enter
	      if (Key==ESC) break;//esc
	     }
	     cold=c;
	  } while ( !(Key==45 && SKey) );
	  Sel++;
	  break;
  case 3:
	 do {
	     Byte=(b<<4)|c;NewKolory[elem]=Byte;
	     textattr(Byte);gotoxy(32,13);cprintf(" Przykçad ");
	     textbackground(b);
	     textcolor(b+8);
	     switch (b)
	     {
	      case  0:gotoxy(33,10);break;
	      case  1:gotoxy(36,10);break;
	      case  2:gotoxy(39,10);break;
	      case  3:gotoxy(42,10);break;
	      case  4:gotoxy(33,11);break;
	      case  5:gotoxy(36,11);break;
	      case  6:gotoxy(39,11);break;
	      case  7:gotoxy(42,11);break;
	     }
	     putch(149);
	     while (!kbhit()) {}
	     if (kbhit()) Key=getch();
	     if (Key==0) { SKey=1;Key=getch();}
	     else SKey=0;
	     textcolor(bold);
	     switch (bold)
	     {
	      case  0:gotoxy(33,10);break;
	      case  1:gotoxy(36,10);break;
	      case  2:gotoxy(39,10);break;
	      case  3:gotoxy(42,10);break;
	      case  4:gotoxy(33,11);break;
	      case  5:gotoxy(36,11);break;
	      case  6:gotoxy(39,11);break;
	      case  7:gotoxy(42,11);break;
	     }
	     putch(219);
	     if (SKey)
	     {
	      if (Key==STAB) { Sel=1;break; }
	      if (Key==DOWN)//dn
	      {
	       if (b<4) b+=4;q=1;
	      }
	      if (Key==UP)//up
	      {
	       if (b>=4) b-=4;q=1;
	      }
	      if (Key==LEFT)//left
	      {
	       if (b>0) b--;q=1;
	      }
	      if (Key==RIGHT)//right
	      {
	       b++;if (b>7) b=7;q=1;
	      }
	     }
	     if (!SKey)
	     {
	      if (Key==TAB) break;//tab
	      if (Key==ENTER) break;//enter
	      if (Key==ESC) break;//esc
	     }
	     bold=b;
	  } while ( !(Key==45 && SKey) );
	  Sel++;
	  break;
  case 4: window(1,1,80,25);
	  if (Button(27,21,35,255,_OK))
	  {
	   if (q==1)
	   {
	    NewKolory[0]=255;
	    Kolory[0]=255;
	   }
	   SKey=0;Key=27;
	  }
	  if ((!SKey)&&(Key==TAB)) Sel=5;
	  if ((SKey)&&(Key==STAB)) Sel=3;
	  window(18,3,62,22);
	  break;
  case 5: window(1,1,80,25);
	  if (Button(44,21,52,255,_Anuluj)) { SKey=0;Key=27;}
	  window(18,3,62,22);
	  if ((!SKey)&&(Key==TAB)) Sel=1;
	  if ((SKey)&&(Key==STAB)) Sel=4;
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}

void  _SaveSetup(void)
{
 int FileDAT;
 Byte=SaveCFG(PathCFG);
 if (Byte>0) _DialogW(_Uwaga,ErrorStr[3],ErrorStr[7]);
 else {
  FileDAT=open(PathDAT,O_BINARY|O_RDWR,S_IREAD|S_IWRITE);
  if (FileDAT>=0)
  {
   lseek(FileDAT,288,SEEK_SET);
   if ((Kolory[0]==255)&&(NewKolory[0]==255))
   {
    NewKolory[0]=0;Word=write(FileDAT,NewKolory,15);
   }
   else { Kolory[0]=0;Word=write(FileDAT,Kolory,15); }
  }
  if ((FileDAT<0)||(Word!=15)) _DialogW(_Uwaga,ErrorStr[3],ErrorStr[7]);
  else _DialogW(_Uwaga,ErrorStr[0],ErrorStr[1]);
  close(FileDAT);
 }
}

void far _WriteRAM(void)
{
 unsigned char Sel;
 unsigned int adres1,adres2,adres3;
 unsigned long bajty;

 adres1=0;adres2=0;adres3=0;
 SaveScreen();
 DlgWindow(21,4,59,18);
 textattr(Kolory[11]);
 gotoxy(12,1);cprintf(" Zapis pamiåci ");
 textattr(Kolory[10]);
 gotoxy(3,3);cprintf("Odczytaj blok z bufora");
 gotoxy(4,4);cprintf("%s",_AdresP);
 gotoxy(4,5);cprintf("%s",_AdresK);
 gotoxy(4,6);cprintf("%s",_IloscB);
 gotoxy(3,8);cprintf("I zapisz w pamiåci RAM");
 gotoxy(4,9);cprintf("%s",_AdresD);

 gotoxy(29,4);cprintf("h");
 gotoxy(29,5);cprintf("h");
 gotoxy(29,9);cprintf("h");
 textattr(Kolory[7]);
 gotoxy(24,4);print__HEX(adres1,"");
 gotoxy(24,5);print__HEX(adres2,"");
 gotoxy(24,6);cprintf("     ");
 gotoxy(24,9);print__HEX(adres3,"");
 bajty=adres2-adres1;bajty+=1;
 ltoa(bajty,StrA,10);
 gotoxy(24,6);cprintf("%s",StrA);
 textattr(Kolory[10]);gotoxy(3,11);Progress(33,0);
 window(1,1,80,25);
 Button(27,16,35,0,_OK);
 Button(44,16,52,0,_Anuluj);
 window(22,4,58,18);
 Key=0;SKey=0;Sel=1;
 do {
 textattr(Kolory[7]);
 switch (Sel)
 {
  case 1: gotoxy(24,4);
	  if (ReadKeybHEX(4,0))
	  {
	   strcpy(StrB,"0x");
	   strcat(StrB,StrA);
	   adres1=strtoul(StrB,NULL,16);
	   if (adres1>adres2) adres2=adres1;
	   gotoxy(24,4);print__HEX(adres1,"");
	   gotoxy(24,5);print__HEX(adres2,"");
	   bajty=adres2-adres1;bajty+=1;
	   gotoxy(24,6);cprintf("     ");
	   ltoa(bajty,StrA,10);
	   gotoxy(24,6);cprintf("%s",StrA);
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=2;
	  if ((SKey)&&(Key==STAB)) Sel=6;
	  break;
  case 2: gotoxy(24,5);
	  if (ReadKeybHEX(4,0))
	  {
	   strcpy(StrB,"0x");
	   strcat(StrB,StrA);
	   adres2=strtoul(StrB,NULL,16);
	   if (adres1>adres2){ adres2=adres1;Sel=0; }
	   gotoxy(24,5);print__HEX(adres2,"");
	   bajty=adres2-adres1;bajty+=1;
	   gotoxy(24,6);cprintf("     ");
	   ltoa(bajty,StrA,10);
	   gotoxy(24,6);cprintf("%s",StrA);
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=3;
	  if ((SKey)&&(Key==STAB)) Sel=1;
	  break;
  case 3: gotoxy(24,6);
	  if (ReadKeyb(5,0))
	  {
	   bajty=strtoul(StrA,NULL,10);
	   if (bajty==0) bajty=1;
	   if ((adres1+bajty)>65536) bajty=65536-adres1;
	   ltoa(bajty,StrA,10);
	   gotoxy(24,6);cprintf("%s",StrA);
	   adres2=adres1+bajty-1;
	   gotoxy(24,5);print__HEX(adres2,"");
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=4;
	  if ((SKey)&&(Key==STAB)) Sel=2;
	  break;
  case 4: gotoxy(24,9);
	  if (ReadKeybHEX(4,0))
	  {
	   strcpy(StrB,"0x");
	   strcat(StrB,StrA);
	   adres3=strtoul(StrB,NULL,16);
	   gotoxy(24,9);print__HEX(adres3,"");
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=5;
	  if ((SKey)&&(Key==STAB)) Sel=3;
	  break;
  case 5: window(1,1,80,25);
	  if (Button(27,16,35,255,_OK))
	  {
	   if (bajty>0)
	   {
	    if (EE_Speed(Speed_1))
	    {
	     _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
	      Sel=5;
	      break;
	    }
	    WaitUART(COMAddr);CheckSum=0;
	    SendB(COMAddr,0xB0);WaitEE(Wait);
	    SendB(COMAddr,0x00);WaitEE(Wait);
	    SendB(COMAddr,0x00);
	    DataType=2;_BytePtr=0;TimeOut=0;
	    do {
		if (TimeOut>MaxTimeOut) break;
	    } while (DataType!=0);
	    if (CheckCRC())
	    {
	     _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
	     Sel=5;
	     break;
	    }
	    WaitUART(COMAddr);
	    Byte=Write_EE(adres3,adres1,bajty);
	    if (Byte>1)
	    {
	     _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
	     Sel=5;
	     break;
	    }
	    if (EE_Speed(9600))
	    {
	     _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
	      Sel=5;
	      break;
	    }
	   }
	   Refresh=1;SKey=0;Key=27;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=6;
	  if ((SKey)&&(Key==STAB)) Sel=4;
	  window(22,4,58,18);
	  break;
  case 6: window(1,1,80,25);
	  if (Button(44,16,52,255,_Anuluj)) { SKey=0;Key=27;}
	  window(22,4,58,18);
	  Sel=1;
	  if ((!SKey)&&(Key==TAB)) Sel=1;
	  if ((SKey)&&(Key==STAB)) Sel=5;
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}

void far _ReadMEM(unsigned char *Str1,unsigned char Comm)
{
 unsigned char Sel;
 unsigned int adres1,adres2,adres3;
 unsigned long bajty;

 adres1=0;adres2=0;adres3=0;
 SaveScreen();
 DlgWindow(21,4,59,18);
 textattr(Kolory[11]);
 gotoxy(9,1);cprintf(" Odczyt pamiåci %s ",Str1);
 textattr(Kolory[10]);
 gotoxy(3,3);cprintf("Odczytaj blok %s",Str1);
 gotoxy(4,4);cprintf("%s",_AdresP);
 gotoxy(4,5);cprintf("%s",_AdresK);
 gotoxy(4,6);cprintf("%s",_IloscB);
 gotoxy(3,8);cprintf("I zapisz w buforze");
 gotoxy(4,9);cprintf("%s",_AdresD);

 gotoxy(29,4);cprintf("h");
 gotoxy(29,5);cprintf("h");
 gotoxy(29,9);cprintf("h");
 textattr(Kolory[7]);
 gotoxy(24,4);print__HEX(adres1,"");
 gotoxy(24,5);print__HEX(adres2,"");
 gotoxy(24,6);cprintf("     ");
 gotoxy(24,9);print__HEX(adres3,"");
 bajty=adres2-adres1;bajty+=1;
 ltoa(bajty,StrA,10);
 gotoxy(24,6);cprintf("%s",StrA);
 textattr(Kolory[10]);gotoxy(3,11);Progress(33,0);
 window(1,1,80,25);
 Button(27,16,35,0,_OK);
 Button(44,16,52,0,_Anuluj);
 window(22,4,58,18);
 Key=0;SKey=0;Sel=1;
 do {
 textattr(Kolory[7]);
 switch (Sel)
 {
  case 1: gotoxy(24,4);
	  if (ReadKeybHEX(4,0))
	  {
	   strcpy(StrB,"0x");
	   strcat(StrB,StrA);
	   adres1=strtoul(StrB,NULL,16);
	   if (adres1>adres2) adres2=adres1;
	   gotoxy(24,4);print__HEX(adres1,"");
	   gotoxy(24,5);print__HEX(adres2,"");
	   bajty=adres2-adres1;bajty+=1;
	   gotoxy(24,6);cprintf("     ");
	   ltoa(bajty,StrA,10);
	   gotoxy(24,6);cprintf("%s",StrA);
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=2;
	  if ((SKey)&&(Key==STAB)) Sel=6;
	  break;
  case 2: gotoxy(24,5);
	  if (ReadKeybHEX(4,0))
	  {
	   strcpy(StrB,"0x");
	   strcat(StrB,StrA);
	   adres2=strtoul(StrB,NULL,16);
	   if (adres1>adres2){ adres2=adres1;Sel=0; }
	   gotoxy(24,5);print__HEX(adres2,"");
	   bajty=adres2-adres1;bajty+=1;
	   gotoxy(24,6);cprintf("     ");
	   ltoa(bajty,StrA,10);
	   gotoxy(24,6);cprintf("%s",StrA);
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=3;
	  if ((SKey)&&(Key==STAB)) Sel=1;
	  break;
  case 3: gotoxy(24,6);
	  if (ReadKeyb(5,0))
	  {
	   bajty=strtoul(StrA,NULL,10);
	   if (bajty==0) bajty=1;
	   if ((adres1+bajty)>65536) bajty=65536-adres1;
	   ltoa(bajty,StrA,10);
	   gotoxy(24,6);cprintf("%s",StrA);
	   adres2=adres1+bajty-1;
	   gotoxy(24,5);print__HEX(adres2,"");
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=4;
	  if ((SKey)&&(Key==STAB)) Sel=2;
	  break;
  case 4: gotoxy(24,9);
	  if (ReadKeybHEX(4,0))
	  {
	   strcpy(StrB,"0x");
	   strcat(StrB,StrA);
	   adres3=strtoul(StrB,NULL,16);
	   gotoxy(24,9);print__HEX(adres3,"");
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=5;
	  if ((SKey)&&(Key==STAB)) Sel=3;
	  break;
  case 5: window(1,1,80,25);
	  if (Button(27,16,35,255,_OK))
	  {
	   if (bajty>0)
	   {
	    if (EE_Speed(Speed_1))
	    {
	     _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
	      Sel=5;
	      break;
	    }
	    WaitUART(COMAddr);CheckSum=0;
	    SendB(COMAddr,Comm);WaitEE(Wait);
	    SendB(COMAddr,0x01);WaitEE(Wait);
	    SendB(COMAddr,0x02);
	    DataType=2;_BytePtr=0;TimeOut=0;
	    do {
		if (TimeOut>MaxTimeOut) break;
	    } while (DataType!=0);
	    if (CheckCRC())
	    {
	     _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
	     Sel=5;
	     break;
	    }
	    WaitUART(COMAddr);
	    Byte=Read_EE(adres1,adres3,bajty);
	    if (Byte>1)
	    {
	     _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
	     Sel=5;
	     break;
	    }
	    if (EE_Speed(9600))
	    {
	     _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
	     Sel=5;
	     break;
	    }
	    MakeBufferB();
	   }
	   Refresh=1;SKey=0;Key=27;
	  }
	  Sel++;
	  if ((!SKey)&&(Key==TAB)) Sel=6;
	  if ((SKey)&&(Key==STAB)) Sel=4;
	  window(22,4,58,18);
	  break;
  case 6: window(1,1,80,25);
	  if (Button(44,16,52,255,_Anuluj)) { SKey=0;Key=27;}
	  window(22,4,58,18);
	  Sel=1;
	  if ((!SKey)&&(Key==TAB)) Sel=1;
	  if ((SKey)&&(Key==STAB)) Sel=5;
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}

void far _EECommand(unsigned char Code,unsigned char Dat1,unsigned char Dat2)
{
 if (EE_Speed(Speed_1)) _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
 else {
  ClearB(COMAddr);CheckSum=0;
  SendB(COMAddr,Code);WaitEE(Wait);
  SendB(COMAddr,Dat1);WaitEE(Wait);
  SendB(COMAddr,Dat2);
  DataType=2;_BytePtr=0;TimeOut=0;
  do {
      if (TimeOut>MaxTimeOut) break;
  } while (DataType!=0);
  if (CheckCRC()) _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
  if (EE_Speed(9600)) _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
 }
}

void far _SimReset(void)
{
 if (EE_Speed(Speed_1)) _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
 else {
  ClearB(COMAddr);CheckSum=0;
  SendB(COMAddr,0xB0);WaitEE(Wait);
  SendB(COMAddr,0x01);WaitEE(Wait);
  SendB(COMAddr,0x02);
  DataType=2;_BytePtr=0;TimeOut=0;
  do {
      if (TimeOut>MaxTimeOut) break;
  } while (DataType!=0);
  if (CheckCRC()) _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
  else {
   ClearB(COMAddr);CheckSum=0;WaitEE(50);
   SendB(COMAddr,0xA0);WaitEE(Wait);
   SendB(COMAddr,0x01);WaitEE(Wait);
   SendB(COMAddr,0x02);
   DataType=2;_BytePtr=0;TimeOut=0;
   do {
       if (TimeOut>MaxTimeOut) break;
   } while (DataType!=0);
   if (CheckCRC()) _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
  }
  if (EE_Speed(9600)) _DialogW(_Uwaga,ErrorStr[8],ErrorStr[9]);
 }
}

void  _Help(unsigned char Sec,unsigned char x,unsigned char *Title)
{
 SaveScreen();
 HlpWindow(1,2,80,24);
 textattr(Kolory[5]);gotoxy(x,1);cprintf(" %s ",Title);
 if (LoadHLP(PathHLP,Sec)==0)
 {
  Byte=1;printHLPPage(Byte);
  do {
  while (!kbhit()) {}
  if (kbhit()) Key=getch();
  if (Key==0) { SKey=1;Key=getch();}
  else SKey=0;
  if (SKey)
  {
   if (Key==71)//home
   {
    Byte=1;
    printHLPPage(Byte);
   }
   if (Key==79)//end
   {
    if (Word>20)
    {
     printHLPPage(Word-20);
     Byte=Word-20;
    }
   }
   if (Key==DOWN)//dn
   {
    if ((Byte<(Word-20))&&(Word>20))
    {
     Byte++;
     printHLPPage(Byte);
    }
   }
   if (Key==UP)//up
   {
    if (Byte>1)
    {
     Byte--;
     printHLPPage(Byte);
    }
   }
   if (Key==PGDOWN)//pgdn
   {
    if ((Byte+20<(Word-20))&&(Word>20))
    {
     Byte+=20;
     printHLPPage(Byte);
    }
    else if (Word>20)
    {
     printHLPPage(Word-20);
     Byte=Word-20;
    }
   }
   if (Key==PGUP)//pgup
   {
    if (Byte-20>1)
    {
     Byte-=20;
     printHLPPage(Byte);
    }
    else
    {
     printHLPPage(1);
     Byte=1;
    }
   }
  }
  if (!SKey)
  {
   if (Key==ESC) break;
  }
  } while ( !(Key==45 && SKey) );
 }
 RestoreScreen();
 Refresh=1;
}

void  _About(void)
{
 SaveScreen();
 DlgWindow(21,6,60,20);
 textattr(Kolory[11]);
 gotoxy(12,1);cprintf(" O programie... ");
 textattr(Kolory[11]);
 gotoxy(9,3);cprintf("Emulator Pamiåci EPROM");
 gotoxy(8,6);cprintf("Autor: Tomasz Nowakowski");
 gotoxy(8,7);cprintf("Tel  : (0-58) 683-08-43");
 textattr(Kolory[10]);
 gotoxy(10,4);cprintf("wersja 2.27  (04.04)");
 gotoxy(6,9);cprintf("Wszystkie prawa zastrzeíone.");
 gotoxy(3,10);cprintf("Kopiowanie w caçoêci lub czåêciach");
 gotoxy(6,11);cprintf("bez zgody autora zabronione.");
 window(1,1,80,25);
 Button(36,18,44,1,_OK);
 Key=0;SKey=0;AboutTime=0;
 do {
 while (!kbhit()) {}
 if (kbhit()) Key=getch();
 if (Key==0) { SKey=1;Key=getch();}
 else SKey=0;
 if (!SKey)
 {
  if (Key==ENTER) break;
  if (Key==ESC) break;
 }
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}