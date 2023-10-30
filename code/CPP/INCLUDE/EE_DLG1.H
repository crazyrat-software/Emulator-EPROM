/*                            OKNO GLOWNE                                  */
void  MainWindow(unsigned char pos,unsigned char posx)
{
 unsigned char q,temp,edit;
 unsigned char Sel,old,xSel,xold;
 Sel=pos;old=pos;
 xSel=posx;xold=posx;
 OutStream=0;
 if (ViewType!=CurView)
 {
  CurView=printView(ViewType);
  Sel=1;old=1;
  xSel=1;xold=1;
 }
 window(2,3,79,23);
 switch (CurView)
 {
 case 1:if (Refresh==1)
	{
	 WriteStatus("",1);ASMLine=0;
	 StatusBar("",0);window(2,3,79,23);
	 printDeASMPage(0,3,12,31,64);
	 MakeBufferB();Refresh=0;
	}
	if (Refresh==2)
	{
	 WriteStatus("",1);
	 StatusBar("",0);window(2,3,79,23);
	 printDeASMPage(UpAddr1,3,12,31,64);
	 Refresh=0;
	}
	if (MainWindowPos!=MainWindowSel) ChangeColor(30,MainWindowPos+2,60,Kolory[5]);
	do {
	    ChangeColor(30,Sel+2,60,Kolory[7]);
	    CodeLine=MOD(ASMLine+(Sel-1),MaxbufferB);
	    StatusBar("",2);
	    window(2,3,79,23);
	    ShowMouse();
	    while (!kbhit()) if (AboutTime) break;
	    if (AboutTime) break;
	    if (kbhit()) Key=getch();
	    if (Key==0){ SKey=1;Key=getch(); }
	    else SKey=0;
	    if (KeyBreak()) break;
	    if (SKey)
	    {
	     if (Key==71) Sel=1;//home
	     if (Key==79) Sel=21;//end
	     if ((Key==119)&&(UpAddr1!=0))//ctrl+home
	     {
	      ASMLine=0;UpAddr1=0;
	      printDeASMPage(UpAddr1,3,12,31,64);
	     }
	     if (Key==DOWN)//dn
	     {
	      Sel++;
	      if (Sel==22)
	      {
	       if ((UpAddr1+bufferB[ASMLine])>MaxbufferA)
		UpAddr1=UpAddr1+bufferB[ASMLine]-MaxbufferA-1;
	       else UpAddr1+=bufferB[ASMLine];
	       Sel=21;ASMLine++;
	       if (ASMLine>MaxbufferB) ASMLine=0;
	       printDeASMPage(UpAddr1,3,12,31,64);
	    ShowMouse();
	      }
	     };
	     if (Key==UP)//up
	     {
	      Sel--;
	      if (Sel==0)
	      {
	       Sel=1;
	       if (ASMLine>0) ASMLine--;
	       else ASMLine=MaxbufferB;
	       if (UpAddr1<bufferB[ASMLine])
		UpAddr1=MaxbufferA-bufferB[ASMLine]+1;
	       else UpAddr1-=bufferB[ASMLine];
	       printDeASMPage(UpAddr1,3,12,31,64);
	      }
	     };
	     if (Key==PGDOWN)//pgdn
	     {
	      for (q=0;q!=21;q++)
	      {
	       if ((UpAddr1+bufferB[ASMLine])>MaxbufferA)
	       UpAddr1=UpAddr1+bufferB[ASMLine]-MaxbufferA-1;
	       else UpAddr1+=bufferB[ASMLine];
	       ASMLine++;
	       if (ASMLine>MaxbufferB) ASMLine=0;
	      };
	      printDeASMPage(UpAddr1,3,12,31,64);
	     };
	     if (Key==PGUP)          //pgup
	     {
	      for (q=0;q!=21;q++)
	      {
	       if (ASMLine>0) ASMLine--;
	       else ASMLine=MaxbufferB;
	       if (UpAddr1<bufferB[ASMLine])
	       UpAddr1=MaxbufferA-bufferB[ASMLine]+1;
	       else UpAddr1-=bufferB[ASMLine];
	      };
	      printDeASMPage(UpAddr1,3,12,31,64);
	     };
	     if (Key==118)//ctrl+pgdn
	     {
	      for (q=0;q!=100;q++)
	      {
	       if ((UpAddr1+bufferB[ASMLine])>MaxbufferA)
	       UpAddr1=UpAddr1+bufferB[ASMLine]-MaxbufferA-1;
	       else UpAddr1+=bufferB[ASMLine];
	       ASMLine++;
	       if (ASMLine>MaxbufferB) ASMLine=0;
	      };
	      printDeASMPage(UpAddr1,3,12,31,64);
	     };
	     if (Key==132)//ctrl+pgup
	     {
	      for (q=0;q!=100;q++)
	      {
	       if (ASMLine>0) ASMLine--;
	       else ASMLine=MaxbufferB;
	       if (UpAddr1<bufferB[ASMLine])
	       UpAddr1=MaxbufferA-bufferB[ASMLine]+1;
	       else UpAddr1-=bufferB[ASMLine];
	      };
	      printDeASMPage(UpAddr1,3,12,31,64);
	     };
	    }
	    ChangeColor(30,old+2,60,Kolory[5]);
	    old=Sel;
	} while ( !(Key==45 && SKey) );
	break;
 case 2:if (Refresh)
	{
	 MakeStatus();
	 printHEX16Page(UpAddr2,2,11,62);
	}
	if ((MainWindowPos!=MainWindowSel)||(MainWindowxPos!=MainWindowxSel))
	{
	 if (MainWindowxPos<9)
	  ChangeColor(8+MainWindowxPos*3,MainWindowPos+2,
	  9+MainWindowxPos*3,Kolory[5]);
	 else
	  ChangeColor(9+MainWindowxPos*3,MainWindowPos+2,
	  10+MainWindowxPos*3,Kolory[5]);
	 ChangeColor(61+MainWindowxPos,MainWindowPos+2,
	 61+MainWindowxPos,Kolory[5]);
	}
	do {
	    if (xSel<9) ChangeColor(8+xSel*3,Sel+2,9+xSel*3,Kolory[7]);
	    else ChangeColor(9+xSel*3,Sel+2,10+xSel*3,Kolory[7]);
	    ChangeColor(61+xSel,Sel+2,61+xSel,Kolory[7]);
	    Address=MOD(UpAddr2+xSel-1+(Sel-1)*16,MaxbufferA);
	    StatusBar("",1);
	    window(2,3,79,23);
	    while (!kbhit()) if (AboutTime) break;
	    if (AboutTime) break;
	    if (kbhit()) Key=getch();
	    if (Key==0){ SKey=1;Key=getch(); }
	    else SKey=0;
	    if (KeyBreak()) break;
	    if (SKey)
	    {
	     if (Key==71) {Sel=1;xSel=1;}//home
	     if (Key==79) {Sel=21;xSel=16;}//end
	     if ((Key==119)&&(UpAddr2!=0))//ctrl+home
	     {
	      UpAddr2=0;
	      printHEX16Page(UpAddr2,2,11,62);
	     }
	     if (Key==LEFT)//left
	     {
	      xSel--;
	      if (xSel==0)
	      {
	       xSel=16;Sel--;
	       if (Sel==0)
	       {
		Sel=1;
		if (UpAddr2<0x10)
		UpAddr2=UpAddr4+1+MaxbufferA-0x10;
		else UpAddr2-=0x10;
		printHEX16Page(UpAddr2,2,11,62);
	       }
	      }
	     }
	     if (Key==RIGHT)//right
	     {
	      xSel++;
	      if (xSel==17)
	      {
	       xSel=1;Sel++;
	       if (Sel==22)
	       {
		Sel=21;
		UpAddr2=MOD(UpAddr2+0x10,MaxbufferA);
		printHEX16Page(UpAddr2,2,11,62);
	       }
	      }
	     }
	     if (Key==DOWN)//dn
	     {
	      Sel++;
	      if (Sel==22)
	      {
	       Sel=21;
	       UpAddr2=MOD(UpAddr2+0x10,MaxbufferA);
	       printHEX16Page(UpAddr2,2,11,62);
	      }
	     };
	     if (Key==UP)//up
	     {
	      Sel--;
	      if (Sel==0)
	      {
	       Sel=1;
	       if (UpAddr2<0x10)
	       UpAddr2=1+MaxbufferA-0x10;
	       else UpAddr2-=0x10;
	       printHEX16Page(UpAddr2,2,11,62);
	      }
	     };
	     if (Key==PGDOWN)//pgdn
	     {
	      UpAddr2=MOD(UpAddr2+0x150,MaxbufferA);
	      printHEX16Page(UpAddr2,2,11,62);
	     };
	     if (Key==PGUP)          //pgup
	     {
	      if (UpAddr2<0x150)
	      UpAddr2=UpAddr2+1+MaxbufferA-0x150;
	      else UpAddr2-=0x150;
	      printHEX16Page(UpAddr2,2,11,62);
	     };
	     if (Key==118)//ctrl+pgdn
	     {
	      UpAddr2=MOD(UpAddr2+0x1000,MaxbufferA);
	      printHEX16Page(UpAddr2,2,11,62);
	     };
	     if (Key==132)//ctrl+pgup
	     {
	      if (UpAddr2<0x1000)
	      UpAddr2=UpAddr2+1+MaxbufferA-0x1000;
	      else UpAddr2-=0x1000;
	      printHEX16Page(UpAddr2,2,11,62);
	     };
	    }
	    if (!SKey)
	    {
	     if (Key==ENTER)//enter
	     {
	      textattr(Kolory[7]);
	      if (xSel<9) gotoxy(8+xSel*3,Sel);
	      else gotoxy(9+xSel*3,Sel);
	      Byte=ReadKeybHEX(2,0);
	      if (Byte==1)
	      {
	       edit=strtoul(StrA,NULL,16);
	       if (xSel<9) gotoxy(8+xSel*3,Sel);
	       else gotoxy(9+xSel*3,Sel);
	       print_HEX(edit,"");
	       bufferA[Address]=edit;
	       gotoxy(61+xSel,Sel);
	       if (edit>=32) putch(edit);
	       else putch('.');
	      }
	      if (Byte==255)
	      {
	       if (xSel<9) gotoxy(8+xSel*3,Sel);
	       else gotoxy(9+xSel*3,Sel);
	       print_HEX(bufferA[Address],"");
	      }
	     }
	    }
	    if (xold<9) ChangeColor(8+xold*3,old+2,9+xold*3,Kolory[5]);
	    else ChangeColor(9+xold*3,old+2,10+xold*3,Kolory[5]);
	    ChangeColor(61+xold,old+2,61+xold,Kolory[5]);
	    old=Sel;xold=xSel;
	} while ( !(Key==45 && SKey) );
	break;
 case 3:if (Refresh)
	{
	 MakeStatus();
	 printHEX8Page(UpAddr3,21,69);
	 printHEX8Page2(UpAddr3+xSel-1,2,49);
	}
	if ((MainWindowPos!=MainWindowSel)||(MainWindowxPos!=MainWindowxSel))
	{
	 ChangeColor(18+MainWindowxPos*3,MainWindowPos+2,
	 19+MainWindowxPos*3,Kolory[5]);
	 ChangeColor(48,MainWindowPos+2,64,Kolory[5]);
	 ChangeColor(68+MainWindowxPos,MainWindowPos+2,
	 68+MainWindowxPos,Kolory[5]);
	}
	do {
	    ChangeColor(18+xSel*3,Sel+2,19+xSel*3,Kolory[7]);
	    ChangeColor(48,Sel+2,64,Kolory[7]);
	    ChangeColor(68+xSel,Sel+2,68+xSel,Kolory[7]);
	    Address=MOD(UpAddr3+(xSel-1)+(Sel-1)*0x08,MaxbufferA);
	    StatusBar("",1);
	    window(2,3,79,23);
	    while (!kbhit()) if (AboutTime) break;
	    if (AboutTime) break;
	    if (kbhit()) Key=getch();
	    if (Key==0){ SKey=1;Key=getch(); }
	    else SKey=0;
	    if (KeyBreak()) break;
	    if (SKey)
	    {
	     if (Key==71)
	     {
	      Sel=1;xSel=1;
	      printHEX8Page2(MOD(UpAddr3+xSel-1,MaxbufferA),2,49);
	     }//home
	     if (Key==79)
	     {
	      Sel=21;xSel=8;
	      printHEX8Page2(MOD(UpAddr3+xSel-1,MaxbufferA),2,49);
	     }//end
	     if ((Key==119)&&(UpAddr3!=0))//ctrl+home
	     {
	      UpAddr3=0;Sel=1;xSel=1;
	      printHEX8Page(UpAddr3,21,69);
	      printHEX8Page2(UpAddr3+xSel-1,2,49);
	     }
	     if (Key==LEFT)//left
	     {
	      xSel--;
	      if (xSel==0){ xSel=8;Sel--; }
	      if (Sel==0)
	      {
	       Sel=1;
	       if (UpAddr3<0x08) UpAddr3=UpAddr3+1+MaxbufferA-0x08;
	       else UpAddr3-=0x08;
	       printHEX8Page(UpAddr3,21,69);
	       printHEX8Page2(UpAddr3+xSel-1,2,49);
	      }
	      else printHEX8Page2(UpAddr3+xSel-1,2,49);
	     };
	     if (Key==RIGHT)//right
	     {
	      xSel++;
	      if (xSel==9){ xSel=1;Sel++; }
	      if (Sel==22)
	      {
	       Sel=21;
	       UpAddr3=MOD(UpAddr3+0x08,MaxbufferA);
	       printHEX8Page(UpAddr3,21,69);
	       printHEX8Page2(UpAddr3+xSel-1,2,49);
	      }
	      else printHEX8Page2(UpAddr3+xSel-1,2,49);
	     };
	     if (Key==DOWN)//dn
	     {
	      Sel++;
	      if (Sel==22)
	      {
	       Sel=21;
	       UpAddr3=MOD(UpAddr3+0x08,MaxbufferA);
	       printHEX8Page(UpAddr3,21,69);
	       printHEX8Page2(UpAddr3+xSel-1,2,49);
	      }
	     };
	     if (Key==UP)//up
	     {
	      Sel--;
	      if (Sel==0)
	      {
	       Sel=1;
	       if (UpAddr3<0x08) UpAddr3=UpAddr3+1+MaxbufferA-0x08;
	       else UpAddr3-=0x08;
	       printHEX8Page(UpAddr3,21,69);
	       printHEX8Page2(UpAddr3+xSel-1,2,49);
	      }
	     };
	     if (Key==PGDOWN)//pgdn
	     {
	      UpAddr3=MOD(UpAddr3+0xA8,MaxbufferA);
	      printHEX8Page(UpAddr3,21,69);
	      printHEX8Page2(UpAddr3+xSel-1,2,49);
	     };
	     if (Key==PGUP)          //pgup
	     {
	      if (UpAddr3<0xA8) UpAddr3=UpAddr3+1+MaxbufferA-0xA8;
	      else UpAddr3-=0xA8;
	      printHEX8Page(UpAddr3,21,69);
	      printHEX8Page2(UpAddr3+xSel-1,2,49);
	     };
	     if (Key==118)//ctrl+pgdn
	     {
	      UpAddr3=MOD(UpAddr3+0x1000,MaxbufferA);
	      printHEX8Page(UpAddr3,21,69);
	      printHEX8Page2(UpAddr3+xSel-1,2,49);
	     };
	     if (Key==132)//ctrl+pgup
	     {
	      if (UpAddr3<0x1000) UpAddr3=UpAddr3+1+MaxbufferA-0x1000;
	      else UpAddr3-=0x1000;
	      printHEX8Page(UpAddr3,21,69);
	      printHEX8Page2(UpAddr3+xSel-1,2,49);
	     };
	    }
	    if (!SKey)
	    {
	     if (Key==ENTER)//enter
	     {
	      textattr(Kolory[7]);
	      gotoxy(18+xSel*3,Sel);Byte=ReadKeybHEX(2,0);
	      if (Byte==1)
	      {
	       edit=strtoul(StrA,NULL,16);
	       gotoxy(18+xSel*3,Sel);print_HEX(edit,"");
	       gotoxy(49,Sel);print_BIN(edit,"");
	       gotoxy(68+xSel,Sel);
	       if (edit>=32) putch(edit);
	       else putch('.');
	       bufferA[Address]=edit;
	      }
	      if (Byte==255)
	      {
	       gotoxy(18+xSel*3,Sel);print_HEX(bufferA[Address],"");
	       gotoxy(49,Sel);print_BIN(bufferA[Address],"");
	      }
	     }
	    }
	    ChangeColor(18+xold*3,old+2,19+xold*3,Kolory[5]);
	    ChangeColor(48,old+2,64,Kolory[5]);
	    ChangeColor(68+xold,old+2,68+xold,Kolory[5]);
	    old=Sel;xold=xSel;
	} while ( !(Key==45 && SKey) );
	break;
 case 4:if (Refresh)
	{
	 MakeStatus();
	 printBMPPage(UpAddr4,2,10,28,36,54,62);
	}
	if ((MainWindowPos!=MainWindowSel)||(MainWindowxPos!=MainWindowxSel))
	{
	 ChangeColor(9+(MainWindowxPos-1)*26,MainWindowPos+2,
	 25+(MainWindowxPos-1)*26,Kolory[5]);
	}
	do {
	    ChangeColor(9+(xSel-1)*26,Sel+2,25+(xSel-1)*26,Kolory[7]);
	    Address=MOD(UpAddr4+(xSel-1)*0x15+(Sel-1),MaxbufferA);
	    StatusBar("",1);
	    window(2,3,79,23);
	    while (!kbhit()) if (AboutTime) break;
	    if (AboutTime) break;
	    if (kbhit()) Key=getch();
	    if (Key==0){ SKey=1;Key=getch(); }
	    else SKey=0;
	    if (KeyBreak()) break;
	    if (SKey)
	    {
	     if (Key==71) {Sel=1;xSel=1;}//home
	     if (Key==79) {Sel=21;xSel=3;}//end
	     if ((Key==119)&&(UpAddr4!=0))//ctrl+home
	     {
	      UpAddr4=0;
	      printBMPPage(UpAddr4,2,10,28,36,54,62);
	     }
	     if (Key==LEFT)//left
	     {
	      xSel--;
	      if (xSel==0)
	      {
	       xSel=1;
	       if (UpAddr4<0x15) UpAddr4=UpAddr4+1+MaxbufferA-0x15;
	       else UpAddr4-=0x15;
	       printBMPPage(UpAddr4,2,10,28,36,54,62);
	      }
	     }
	     if (Key==RIGHT)//right
	     {
	      xSel++;
	      if (xSel==4)
	      {
	       xSel=3;
	       UpAddr4=MOD(UpAddr4+0x15,MaxbufferA);
	       printBMPPage(UpAddr4,2,10,28,36,54,62);
	      }
	     }
	     if (Key==DOWN)//dn
	     {
	      Sel++;
	      if (Sel==22)
	      {
	       Sel=1;xSel++;
	       if (xSel==4)
	       {
		xSel=3;Sel=21;
		UpAddr4=MOD(UpAddr4+1,MaxbufferA);
		printBMPPage(UpAddr4,2,10,28,36,54,62);
	       }
	      }
	     };
	     if (Key==UP)//up
	     {
	      Sel--;
	      if (Sel==0)
	      {
	       Sel=21;xSel--;
	       if (xSel==0)
	       {
		xSel=1;Sel=1;
		if (UpAddr4==0)	UpAddr4=MaxbufferA;
		else UpAddr4--;
		printBMPPage(UpAddr4,2,10,28,36,54,62);
	       }
	      }
	     };
	     if (Key==PGDOWN)//pgdn
	     {
	      UpAddr4=MOD(UpAddr4+0x3F,MaxbufferA);
	      printBMPPage(UpAddr4,2,10,28,36,54,62);
	     };
	     if (Key==PGUP)          //pgup
	     {
	      if (UpAddr4<0x3F)
	      UpAddr4=UpAddr4+1+MaxbufferA-0x3F;
	      else UpAddr4-=0x3F;
	      printBMPPage(UpAddr4,2,10,28,36,54,62);
	     };
	     if (Key==118)//ctrl+pgdn
	     {
	      UpAddr4=MOD(UpAddr4+0x1000,MaxbufferA);
	      printBMPPage(UpAddr4,2,10,28,36,54,62);
	     };
	     if (Key==132)//ctrl+pgup
	     {
	      if (UpAddr4<0x1000)
	      UpAddr4=UpAddr4+1+MaxbufferA-0x1000;
	      else UpAddr4-=0x1000;
	      printBMPPage(UpAddr4,2,10,28,36,54,62);
	     };
	    }
	    if (!SKey)
	    {
	     if (Key==ENTER)//enter
	     {
	      textattr(Kolory[7]);
	      gotoxy(10+(xSel-1)*26,Sel);
	      bufferA[Address]=ReadKeybBMP(bufferA[Address]);
	     }
	    }
	    ChangeColor(9+(xold-1)*26,old+2,25+(xold-1)*26,Kolory[5]);
	    old=Sel;xold=xSel;
	} while ( !(Key==45 && SKey) );
	break;
 }
 window(1,1,80,25);
 MainWindowPos=Sel;
 MainWindowxPos=xSel;
}

void  _MemEdit(unsigned char *str,unsigned char comm)
{
 unsigned char Sel,q;
 unsigned int adres1,adres2,adres3;
 unsigned long bajty;

 adres1=0;
 adres2=0;
 adres3=0;
 SaveScreen();
 DlgWindow(21,4,59,16);
 textattr(Kolory[11]);
 gotoxy(14,1);cprintf(" %s ",str);
 textattr(Kolory[10]);
 gotoxy(3,3);cprintf("%s blok",str);
 gotoxy(4,4);cprintf("%s",_AdresP);
 gotoxy(4,5);cprintf("%s",_AdresK);
 gotoxy(4,6);cprintf("%s",_IloscB);
 gotoxy(3,8);cprintf("Do");
 gotoxy(4,9);cprintf("%s",_AdresD);
 gotoxy(29,4);cprintf("h");
 gotoxy(29,5);cprintf("h");
 gotoxy(29,9);cprintf("h");
 textattr(Kolory[7]);
 gotoxy(24,4);print__HEX(adres1,"");
 gotoxy(24,5);print__HEX(adres2,"");
 gotoxy(24,6);FillSpace(5);
 //cprintf("     ");
 gotoxy(24,9);print__HEX(adres3,"");
 bajty=adres2-adres1;bajty+=1;
 ltoa(bajty,StrA,10);
 gotoxy(24,6);cprintf("%s",StrA);
 window(1,1,80,25);
 Button(27,14,35,0,_OK);
 Button(44,14,52,0,_Anuluj);
 window(22,4,58,16);
 Key=0;SKey=0;Sel=1;q=0;
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
	   gotoxy(24,6);FillSpace(5);
	   //cprintf("     ");
	   ltoa(bajty,StrA,10);
	   gotoxy(24,6);cprintf("%s",StrA);
	   q=1;
	  }
	  Sel++;
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
	   gotoxy(24,6);FillSpace(5);
//	   cprintf("     ");
	   ltoa(bajty,StrA,10);
	   gotoxy(24,6);cprintf("%s",StrA);
	   q=1;
	  }
	  Sel++;
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
	   q=1;
	  }
	  Sel++;
	  break;
  case 4: gotoxy(24,9);
	  if (ReadKeybHEX(4,0))
	  {
	   strcpy(StrB,"0x");
	   strcat(StrB,StrA);
	   adres3=strtoul(StrB,NULL,16);
	   gotoxy(24,9);print__HEX(adres3,"");
	   q=1;
	  }
	  Sel++;
	  break;
  case 5: window(1,1,80,25);
	  if (Button(27,14,35,255,_OK))
	  {
	   if ( (bajty>0)&&(q==1) )
	   {
	    for (Long=adres1;Long<=adres2;Long++)
	    {
	     bufferA[adres3]=bufferA[Long];
	     adres3++;
	    }
	    if (comm) ClearBuf(0,0,adres1,adres2);
	    MakeBufferB();
	   }
	   Refresh=1;SKey=0;Key=27;
	  }
	  Sel++;
	  window(22,4,58,16);
	  break;
  case 6: window(1,1,80,25);
	  if (Button(44,14,52,255,_Anuluj)) { SKey=0;Key=27;}
	  window(22,4,58,16);
	  Sel=1;
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}

void  _Fill(void)
{
 unsigned char Sel,q;
 unsigned int adres1,adres2;
 unsigned char wartosc;
 unsigned long bajty;

 adres1=0;
 adres2=0;
 wartosc=0;
 SaveScreen();
 DlgWindow(21,4,59,16);
 textattr(Kolory[11]);
 gotoxy(14,1);cprintf(" Wypenij ");
 textattr(Kolory[10]);
 gotoxy(3,3);cprintf("Wypenij blok");
 gotoxy(4,4);cprintf("%s",_AdresP);
 gotoxy(4,5);cprintf("%s",_AdresK);
 gotoxy(4,6);cprintf("%s",_IloscB);
 gotoxy(3,8);cprintf("WartociŠ");
 gotoxy(4,9);cprintf("DEC :           HEX :");
 gotoxy(29,4);cprintf("h");
 gotoxy(29,5);cprintf("h");
 gotoxy(30,9);cprintf("h");
 textattr(Kolory[7]);
 gotoxy(24,4);print__HEX(adres1,"");
 gotoxy(24,5);print__HEX(adres2,"");
 gotoxy(24,6);FillSpace(5);
 //cprintf("     ");
 gotoxy(11,9);FillSpace(3);
 //cprintf("   ");
 gotoxy(11,9);cprintf("%u",wartosc);
 gotoxy(27,9);print_HEX(wartosc,"");
 bajty=adres2-adres1;bajty+=1;
 ltoa(bajty,StrA,10);
 gotoxy(24,6);cprintf("%s",StrA);
 window(1,1,80,25);
 Button(27,14,35,0,_OK);
 Button(44,14,52,0,_Anuluj);
 window(22,4,58,16);
 Key=0;SKey=0;Sel=1;q=0;
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
	   gotoxy(24,6);FillSpace(5);
	   //cprintf("     ");
	   ltoa(bajty,StrA,10);
	   gotoxy(24,6);cprintf("%s",StrA);
	   q=1;
	  }
	  Sel++;
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
	   gotoxy(24,6);FillSpace(5);
	   //cprintf("     ");
	   ltoa(bajty,StrA,10);
	   gotoxy(24,6);cprintf("%s",StrA);
	   q=1;
	  }
	  Sel++;
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
	   q=1;
	  }
	  Sel++;
	  break;
  case 4: gotoxy(11,9);
	  if (ReadKeyb(3,0))
	  {
	   wartosc=strtoul(StrA,NULL,10);
	   gotoxy(11,9);FillSpace(3);
	   //cprintf("   ");
	   gotoxy(11,9);cprintf("%u",wartosc);
	   gotoxy(27,9);print_HEX(wartosc,"");
	   q=1;
	  }
	  Sel++;
	  break;
  case 5: gotoxy(27,9);
	  if (ReadKeybHEX(2,0))
	  {
	   strcpy(StrB,"0x");
	   strcat(StrB,StrA);
	   wartosc=strtoul(StrB,NULL,16);
	   gotoxy(11,9);FillSpace(3);
	   //cprintf("   ");
	   gotoxy(11,9);cprintf("%u",wartosc);
	   gotoxy(27,9);print_HEX(wartosc,"");
	   q=1;
	  }
	  Sel++;
	  break;
  case 6: window(1,1,80,25);
	  if (Button(27,14,35,255,_OK))
	  {
	   if ( (bajty>0)&&(q==1) )
	   {
	    ClearBuf(1,wartosc,adres1,adres2);
	    MakeBufferB();
	   }
	   Refresh=1;SKey=0;Key=27;
	  }
	  Sel++;
	  window(22,4,58,16);
	  break;
  case 7: window(1,1,80,25);
	  if (Button(44,14,52,255,_Anuluj)) { SKey=0;Key=27;}
	  window(22,4,58,16);
	  Sel=1;
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}

void  _GoTo(void)
{
 unsigned char Sel,q;
 unsigned int adres;
 unsigned long nrlinii;
 unsigned char etykieta[16];

 SaveScreen();
 DlgWindow(21,5,59,13);
 textattr(Kolory[11]);
 gotoxy(15,1);cprintf(" Id‘ do ");
 textattr(Kolory[10]);
 gotoxy(3,3);cprintf("Adres    :");
 gotoxy(3,4);cprintf("Nr linii :");
 gotoxy(3,5);cprintf("Etykieta :");
 gotoxy(20,3);cprintf("h");
 textattr(Kolory[7]);
 gotoxy(15,3);cprintf("%-4s",SPACE);
 gotoxy(15,4);cprintf("%-5s",SPACE);
 gotoxy(15,5);FillSpace(15);
 //cprintf("               ");
 if (CurView!=1) {gotoxy(15,4);cprintf("*****");}
 window(1,1,80,25);
 Button(27,11,35,0,_OK);
 Button(44,11,52,0,_Anuluj);
 window(22,5,58,16);
 Key=0;SKey=0;Sel=1;q=0;
 do {
 textattr(Kolory[7]);
 switch (Sel)
 {
  case 1: gotoxy(15,3);
	  if (ReadKeybHEX(4,0))
	  {
	   if (CurView==1) {gotoxy(15,4);cprintf("%-5s",SPACE);}
	   gotoxy(15,5);FillSpace(15);
	   //cprintf("               ");
	   strcpy(StrB,"0x");
	   strcat(StrB,StrA);
	   Word=strtoul(StrB,NULL,16);
	   if (Word>MaxbufferA) adres=MaxbufferA;
	   else adres=Word;
	   gotoxy(15,3);print__HEX(adres,"");
	   q=1;
	  }
	  Sel=4;
	  if ((!SKey)&&(Key==TAB)) Sel=2;
	  if ((SKey)&&(Key==STAB)) Sel=5;
	  break;
  case 2: if (CurView!=1)
	  {
	   if ((!SKey)&&(Key==TAB)) Sel=3;
	   if ((SKey)&&(Key==STAB)) Sel=1;
	   break;
	  }
	  gotoxy(15,4);
	  if (ReadKeyb(5,0))
	  {
	   gotoxy(15,3);cprintf("%-4s",SPACE);
	   gotoxy(15,5);FillSpace(15);
	   //cprintf("               ");
	   Long=strtoul(StrA,NULL,10);
	   if (Long==0) Long=1;
	   if (Long<=MaxbufferB) nrlinii=Long-1;
	   else nrlinii=MaxbufferB;
	   ltoa(nrlinii+1,StrA,10);
	   gotoxy(15,4);cprintf("%-5s",StrA);
	   q=2;
	  }
	  Sel=4;
	  if ((!SKey)&&(Key==TAB)) Sel=3;
	  if ((SKey)&&(Key==STAB)) Sel=1;
	  break;
  case 3: gotoxy(15,5);
	  if (ReadKeyb(15,0))
	  {
	   gotoxy(15,3);cprintf("%-4s",SPACE);
	   if (CurView==1) {gotoxy(15,4);cprintf("%-5s",SPACE);}
	   strcpy(etykieta,strupr(StrA));
	   gotoxy(15,5);cprintf("%s",etykieta);
	   q=3;
	  }
	  Sel=4;
	  if ((!SKey)&&(Key==TAB)) Sel=4;
	  if ((SKey)&&(Key==STAB)) Sel=2;
	  break;
  case 4: window(1,1,80,25);
	  if (Button(27,11,35,255,_OK))
	  {
	   switch (q)
	   {
	    case 0:SKey=0;Key=27;break;
	    case 1:for (UpAddr1=0,ASMLine=0;UpAddr1<=adres;ASMLine++)
		    UpAddr1+=CodeLen[bufferA[UpAddr1]];
		   if (UpAddr1>adres)
		   {
		    ASMLine--;UpAddr1-=bufferB[ASMLine];
		   }
		   UpAddr2=(adres/0x10)*0x10;
		   UpAddr3=(adres/0x08)*0x08;
		   UpAddr4=adres;
		   MainWindowSel=1;MainWindowxSel=1;
		   if (CurView==2) MainWindowxSel=(adres%0x10)+1;
		   if (CurView==3) MainWindowxSel=(adres%0x08)+1;
		   Refresh=2;SKey=0;Key=27;
		   break;
	    case 2:for (UpAddr1=0,ASMLine=0;ASMLine<nrlinii;ASMLine++)
		    UpAddr1+=CodeLen[bufferA[UpAddr1]];
		   if (CurView==1) MainWindowSel=1;
		   Refresh=2;SKey=0;Key=27;
		   break;
	    case 3:for (Word=0;Word<=0xFF;Word++)
		   {
		    Byte=1;
		    if (stricmp(etykieta,LabelStr[Word])==0)
		    {
		     adres=LabelAdr[Word];
		     if (adres>MaxbufferA) adres=MaxbufferA;
		     for (UpAddr1=0,ASMLine=0;UpAddr1<=adres;ASMLine++)
		      UpAddr1+=CodeLen[bufferA[UpAddr1]];
		     if (UpAddr1>adres)
		     {
		      ASMLine--;UpAddr1-=bufferB[ASMLine];
		     }
		     UpAddr2=(adres/0x10)*0x10;
		     UpAddr3=(adres/0x08)*0x08;
		     UpAddr4=adres;
		     MainWindowSel=1;MainWindowxSel=1;
		     if (CurView==2) MainWindowxSel=(adres%0x10)+1;
		     if (CurView==3) MainWindowxSel=(adres%0x08)+1;
		     Refresh=2;SKey=0;Key=27;Byte=0;
		     break;
		    }
		   }
		   if (Byte)
		   {
		    _DialogW(_Uwaga,"Nie znaleziono etykiety.","");
		    Sel=3;
		   }
		   break;
	   }
	  }
	  if ((!SKey)&&(Key==TAB)) Sel=5;
	  if ((SKey)&&(Key==STAB)) Sel=3;
	  window(22,5,58,16);
	  break;
  case 5: window(1,1,80,25);
	  if (Button(44,11,52,255,_Anuluj)) { SKey=0;Key=27;}
	  window(22,5,58,16);
	  if ((!SKey)&&(Key==TAB)) Sel=1;
	  if ((SKey)&&(Key==STAB)) Sel=4;
	  break;
 }
 if ( (!SKey)&&(Key==ESC) ) break;
 } while ( !(Key==45 && SKey) );
 RestoreScreen();
}
