void FillSpace(unsigned char x1)
{
 unsigned char t;
 for (t=0;t<x1;t++) putch(SPACE);
}

unsigned int MOD(unsigned int In,unsigned long Rot)
{
 In=In%(Rot+1);
 return In;
}

void SetKeyb(void)
{
 asm {
      mov al,0xF3;
      out 0x60,al;
 }
 delay(100);
 asm {
      mov al,0x0D;
      out 0x60,al;
 }
}

void SetPLChar(void)
{
 asm {
      mov ax,0x0003;
      int 0x10;
 }
 RSeg=FP_SEG(PL_1[0]);
 ROfs=FP_OFF(PL_1[0]);
 asm {
      mov ax,RSeg;
      mov es,ax;
      mov ax,ROfs;
      mov bp,ax;
      mov cx,9;
      mov dx,128;
      mov bl,0;
      mov bh,16;
      mov ah,0x11;
      mov al,0;
      int 0x10;
 }
 RSeg=FP_SEG(PL_2[0]);
 ROfs=FP_OFF(PL_2[0]);
 asm {
      mov ax,RSeg;
      mov es,ax;
      mov ax,ROfs;
      mov bp,ax;
      mov cx,9;
      mov dx,138;
      mov bl,0;
      mov bh,16;
      mov ah,0x11;
      mov al,0;
      int 0x10;
 }
 RSeg=FP_SEG(ZBit[0]);
 ROfs=FP_OFF(ZBit[0]);
 asm {
      mov ax,RSeg;
      mov es,ax;
      mov ax,ROfs;
      mov bp,ax;
      mov cx,2;
      mov dx,148;
      mov bl,0;
      mov bh,16;
      mov ah,0x11;
      mov al,0;
      int 0x10;
 }
}

unsigned char ReadKeyb(unsigned char max,unsigned char type)
{
 unsigned char temp,q,w,x,y,ret;
 switch (type)
 {
  case 0: x=wherex();
	  y=wherey();
	  break;
  case 1: x=wherex();
	  y=wherey();
	  for (w=1;w<=max;w++) putch(SPACE);
	  break;
 }
 _setcursortype(_SOLIDCURSOR);
 gotoxy(x,y);
 temp=x;q=0;ret=0;
 do {
  while (!kbhit()) {}
  if (kbhit()) Key=getch();
  if (Key==0) { SKey=1;Key=getch();}
  else SKey=0;
  if (SKey)
  {
   if (Key==15) break;
  }
  if (!SKey)
  {
   if (Key>=32)
   {
    if (q<max)
    {
     if (ret!=1)
     {
      gotoxy(x,y);
      for (w=1;w<=max;w++) putch(SPACE);
      gotoxy(temp,y);
      ret=1;
     }
     putch(Key);
     StrA[q]=Key;q++;StrA[q]=0;
     temp=wherex();
     gotoxy(temp,y);
    }
    if (q==max) break;
   };
   if (Key==8)//backspace
   {
    if (ret!=1)
    {
     gotoxy(x,y);
     for (w=1;w<=max;w++) putch(SPACE);
     gotoxy(temp,y);
     ret=1;
    }
    if (temp>x) temp--;
    if (q>0) q--;
    StrA[q]=0;
    gotoxy(temp,y);putch(SPACE);
    gotoxy(temp,y);
   }
   if (Key==TAB) break;//tab
   if (Key==ENTER) break;//enter
   if (Key==ESC) break;//esc
  }
 } while ( !(Key==45 && SKey) );
 StrA[q]=0;
 _setcursortype(_NOCURSOR);
 return ret;
}

unsigned char ReadKeybHEX(unsigned char max,unsigned char type)
{
 unsigned char temp,q,w,x,y,ret;
 switch (type)
 {
  case 0: x=wherex();
	  y=wherey();
	  break;
  case 1: x=wherex();
	  y=wherey();
	  for (w=1;w<=max;w++) putch(SPACE);
	  break;
 }
 _setcursortype(_SOLIDCURSOR);
 gotoxy(x,y);
 temp=x;q=0;ret=0;
 do {
  while (!kbhit()) {}
  if (kbhit()) Key=getch();
  if (Key==0) { SKey=1;Key=getch();}
  else SKey=0;
  if (SKey)
  {
   if (Key==15) break;
  }
  if (!SKey)
  {
   if ((Key>=48)&&(Key<=57)||(Key>=65)&&(Key<=70)||(Key>=97)&&(Key<=102))
   {
    if (q<max)
    {
     if (ret!=1)
     {
      gotoxy(x,y);
      for (w=1;w<=max;w++) putch(SPACE);
      gotoxy(temp,y);
      ret=1;
     }
     Key=toupper(Key);
     putch(Key);
     StrA[q]=Key;q++;StrA[q]=0;
     temp=wherex();
     gotoxy(temp,y);
    }
    if (q==max) break;
   }
   if (Key==8)//backspace
   {
    if (ret!=1)
    {
     gotoxy(x,y);
     for (w=1;w<=max;w++) putch(SPACE);
     gotoxy(temp,y);
     ret=1;
    }
    if (temp>x) temp--;
    if (q>0) q--;
    StrA[q]=0;
    gotoxy(temp,y);putch(SPACE);
    gotoxy(temp,y);
   }
   if (Key==TAB) break;//tab
   if (Key==ENTER) break;//enter
   if (Key==ESC) { ret=255; break; }//esc
  }
 } while ( !(Key==45 && SKey) );
 StrA[q]=0;
 _setcursortype(_NOCURSOR);
 return ret;
}

unsigned char ReadKeybBMP(unsigned char byte)
{
 unsigned char temp,ret,bit,x,y;
 x=wherex();y=wherey();
 _setcursortype(_NORMALCURSOR);
 temp=x;ret=byte;bit=0x80;
 do {
  gotoxy(temp,y);
  while (!kbhit()) {}
  if (kbhit()) Key=getch();
  if (Key==0) { SKey=1;Key=getch();}
  else SKey=0;
  if (SKey)
  {
   if (Key==RIGHT)//right
   {
     if (bit>1){ bit=bit/2;temp+=2; }
   }
   if (Key==LEFT)//left
   {
     if (bit<0x80){ bit=bit*2;temp-=2; }
   }
   if (Key==83)//del
   {
    ret=0;
    gotoxy(x,y);print_xBIN(ret,"");
    gotoxy(temp,y);
   }
   if ((Key==UP)||(Key==DOWN))//up dn
   {
    gotoxy(temp,y);
    if ((ret&bit)==bit){ ret=ret&(255-bit);putch(148); }//ustawia 0
    else { ret=ret|bit;putch(149); }//ustawia 1
    gotoxy(temp,y);
   }
  }
  if (!SKey)
  {
   if (Key==32)
   {
    gotoxy(temp,y);
    if ((ret&bit)==bit){ ret=ret&(255-bit);putch(148); }//ustawia 0
    else { ret=ret|bit;putch(149); }//ustawia 1
    gotoxy(temp,y);
   }
   if (Key==TAB) break;//tab
   if (Key==ENTER) break;//enter
   if (Key==ESC)//esc
   {
    ret=byte;gotoxy(x,y);print_xBIN(ret,"");
    break;
   }
  }
 } while ( !(Key==45 && SKey) );
 _setcursortype(_NOCURSOR);
 return ret;
}

unsigned char ReadKeybPath(unsigned char max,unsigned char type)
{
 unsigned char temp,q,w,x,y,ret;
 switch (type)
 {
  case 0: x=wherex();
	  y=wherey();
	  break;
  case 1: x=wherex();
	  y=wherey();
	  for (w=1;w<=max;w++) putch(SPACE);
	  break;
 }
 _setcursortype(_SOLIDCURSOR);
 gotoxy(x,y);
 temp=x;q=0;ret=0;
 do {
  while (!kbhit()) {}
  if (kbhit()) Key=getch();
  if (Key==0) { SKey=1;Key=getch();}
  else SKey=0;
  if (SKey)
  {
   if (Key==15) break;
  }
  if (!SKey)
  {
   if (Key==92)
   {
    if (q<max)
    {
     if (ret!=1)
     {
      gotoxy(x,y);
      for (w=1;w<=max;w++) putch(SPACE);
      gotoxy(temp,y);
      ret=1;
     }
     putch('\\');
     StrA[q]='\\';q++;StrA[q]=0;
     temp=wherex();
     gotoxy(temp,y);
    }
    if (q==max) break;
   }
   if ((Key>=32)&&(Key!=92))
   {
    if (q<max)
    {
     if (ret!=1)
     {
      gotoxy(x,y);
      for (w=1;w<=max;w++) putch(SPACE);
      gotoxy(temp,y);
      ret=1;
     }
     Key=toupper(Key);putch(Key);
     StrA[q]=Key;q++;StrA[q]=0;
     temp=wherex();
     gotoxy(temp,y);
    }
    if (q==max) break;
   };
   if (Key==8)//backspace
   {
    if (ret!=1)
    {
     gotoxy(x,y);
     for (w=1;w<=max;w++) putch(SPACE);
     gotoxy(temp,y);
     ret=1;
    }
    if (temp>x) temp--;
    if (q>0) q--;
    StrA[q]=0;
    gotoxy(temp,y);putch(SPACE);
    gotoxy(temp,y);
   }
   if (Key==TAB) break;//tab
   if (Key==ENTER) break;//enter
   if (Key==ESC) break;//esc
  }
 } while ( !(Key==45 && SKey) );
 StrA[q]=0;
 _setcursortype(_NOCURSOR);
 return ret;
}

unsigned char KeyBreak(void)
{
 unsigned char ret=0;
 if (SKey) switch(Key)
 {
  case 84:ret=1;break;//Shift+F1
  case 85:ret=1;break;//Shift+F2
  case 59:ret=1;break;//F1
  case 60:ret=1;break;//F2
  case 61:ret=1;break;//F3
  case 63:ret=1;break;//F5
  case 64:ret=1;break;//F6
  case 65:ret=1;break;//F7
  case 66:ret=1;break;//F8
  case 67:ret=1;break;//F8
  case 103:ret=1;break;//Ctrl+F10
  case 137:ret=1;break;//Ctrl+F11
  case 138:ret=1;break;//Ctrl+F12
  case 25:ret=1;break;//alt+P
  case 18:ret=1;break;//alt+E
  case 17:ret=1;break;//alt+W
  case 31:ret=1;break;//alt+S
  case 22:ret=1;break;//alt+U
  case 24:ret=1;break;//alt+O
  case 45:ret=1;break;//alt+X
 }
 if (!SKey) switch(Key)
 {
  case 4:ret=1;break;//ctrl+D
  case 6:ret=1;break;//ctrl+F
  case 7:ret=1;break;//ctrl+G
  case 11:ret=1;break;//ctrl+K
  case 18:ret=1;break;//ctrl+R
 }
 return ret;
}

void ChangeColor(unsigned char x1,unsigned char y1,
		 unsigned char x2,unsigned char Attr)
{
 unsigned int temp;
 if (x1>80) x1=80;
 if (x1<1) x1=1;
 if (x2>80) x2=80;
 if (x2<1) x2=1;
 if (y1>25) y1=25;
 if (y1<1) y1=1;
 for (temp=(y1-1)*160+x1*2+1;temp<=(y1-1)*160+x2*2+1;temp+=2)
  pokeb(0xB800,temp,Attr);
}

void ChangeChar(unsigned char x1,unsigned char y1,unsigned char Char)
{
 unsigned int temp;
 if (x1>80) x1=80;
 if (x1<1) x1=1;
 if (y1>25) y1=25;
 if (y1<1) y1=1;
 temp=(y1-1)*160+(x1-1)*2;
 pokeb(0xB800,temp,Char);
}

void SaveScreen(void)
{
 unsigned int temp;
 for (temp=0;temp!=4000;temp++) Obraz[temp]=peekb(0xB800,temp);
}

void SaveLine(unsigned char ln)
{
 unsigned int temp;
 if (ln>25) ln=25;
 if (ln<1) ln=1;
 for (temp=((ln-1)*160);temp!=(ln*160);temp++) Obraz[temp]=peekb(0xB800,temp);
}

void RestoreScreen(void)
{
 unsigned int temp;
 for (temp=0;temp!=4000;temp++) pokeb(0xB800,temp,Obraz[temp]);
}

void RestoreLine(unsigned char ln)
{
 unsigned int temp;
 if (ln>25) ln=25;
 if (ln<1) ln=1;
 for (temp=((ln-1)*160);temp!=(ln*160);temp++) pokeb(0xB800,temp,Obraz[temp]);
}

void ColTable(unsigned char x1,unsigned char y1)
{
unsigned char q,w,l;
 gotoxy(x1,y1);textattr(Kolory[10]);cprintf("Kolor tekstu");
 for (w=0,l=0;w<=3;w++)
 {
  gotoxy(x1,y1+w+1);
  for (q=0;q<=3;q++,l++)
  {
   textcolor(l);cprintf("ЫЫЫ");
  }
 }
 gotoxy(x1,y1+6);textattr(Kolory[10]);cprintf("Kolor tЌa");
 for (w=0,l=0;w<=1;w++)
 {
  gotoxy(x1,y1+w+7);
  for (q=0;q<=3;q++,l++)
  {
   textcolor(l);cprintf("ЫЫЫ");
  }
 }
}

void DlgWindow(unsigned char x1,unsigned char y1,
	       unsigned char x2,unsigned char y2)
{
 unsigned char q;
 window(x1,y1,x2,y2);textattr(Kolory[11]);clrscr();
 for (q=y1+1;q<=y2;q++) ChangeColor(x2,q,x2+1,0x0);
 ChangeColor(x1+1,y2+1,x2+1,0x07);
 window(1,1,80,25);
 gotoxy(x1,y1);putch(Ramki[1][0]);		     //4 rogi
 gotoxy(x2,y1);putch(Ramki[1][1]);
 gotoxy(x1,y2);putch(Ramki[1][2]);
 gotoxy(x2,y2);putch(Ramki[1][3]);		     //
 gotoxy(x1+1,y1);for (q=x1+1;q<x2;q++) putch(Ramki[1][4]); //top
 gotoxy(x1+1,y2);for (q=x1+1;q<x2;q++) putch(Ramki[1][4]);//bottom
 for (q=y1+1;q<y2;q++)                                 //pionowe linie
 {
  gotoxy(x1,q);putch(Ramki[1][5]);
  gotoxy(x2,q);putch(Ramki[1][5]);
 }
 window(x1+1,y1,x2-1,y2-1);
}

void DlgList(unsigned char x1,unsigned char y1,
	     unsigned char x2,unsigned char y2)
{
 unsigned char q;
 gotoxy(x1,y1);putch(Ramki[0][0]);		     //4 rogi
 gotoxy(x2,y1);putch(Ramki[0][1]);
 gotoxy(x1,y2);putch(Ramki[0][2]);
 gotoxy(x2,y2);putch(Ramki[0][3]);		     //
 gotoxy(x1+1,y1);for (q=x1+1;q<x2;q++) putch(Ramki[0][4]); //top
 gotoxy(x1+1,y2);for (q=x1+1;q<x2;q++) putch(Ramki[0][4]);//bottom
 for (q=y1+1;q<y2;q++)                                 //pionowe linie
 {
  gotoxy(x1,q);putch(Ramki[0][5]);
  gotoxy(x2,q);putch(Ramki[0][5]);
 }
}

void Progress(unsigned char max,unsigned char type)
{
 unsigned char q;
 if (type==0)
 {
  for (q=1;q<=max;q++) putch('°');
 }
 else if (type<=max)
 {
  for (q=1;q<=type;q++) putch('І');
  for (;q<=max;q++) putch('°');
 }
}

unsigned char Button(unsigned char x1,unsigned char y1,unsigned char x2,
		     unsigned char type,const unsigned char *str)
{
 unsigned char q,ret;
 ret=0;q=0;
 if (type==255)
 {
  ChangeColor(x1,y1,x2,Kolory[13]);
  do {
   while (!kbhit()) {}
   if (kbhit()) Key=getch();
   if (Key==0) { SKey=1;Key=getch();}
   else SKey=0;
   if (!SKey)
   {
    if (Key==ENTER) {ret=1;break;}
    if (Key==ESC) {ret=0;break;}
    if (Key==TAB)  {ret=0;break;}
   }
   if (SKey)
   {
    if (Key==15) {ret=0;break;}
   }
  } while ( !(Key==45 && SKey) );
  ChangeColor(x1,y1,x2,Kolory[12]);
 }
 else
 {
  textattr(Kolory[12]);
  gotoxy(x1,y1);for (q=x1;q<=x2;q++) putch(SPACE);
  textattr(Kolory[10]);
  gotoxy(x2+2,y1);putch('Ь');
  gotoxy(x1+1,y1+1);for (q=x1;q<=x2+1;q++) putch('Я');
  textattr(Kolory[12]);
  gotoxy(x1 + (x2-x1)/2 - (strlen(str)/2) + 1 ,y1);cprintf("%s",str);
  switch(type)
  {
   case 0: ChangeColor(x1,y1,x2,Kolory[12]);
	   ret=0;
	   break;
   case 1: ChangeColor(x1,y1,x2,Kolory[13]);
	   ret=1;
	   break;
  }
 }
 return ret;
}

void  _DialogW(const unsigned char *Top,unsigned char *Str1,unsigned char *Str2)
{
 unsigned int temp;
 unsigned char q;
 unsigned char screen[4096];
 for (temp=0;temp!=4000;temp++) screen[temp]=peekb(0xB800,temp);
 DlgWindow(21,10,60,17);
 textattr(Kolory[11]);
 gotoxy(2+(57-24)/2-(strlen(Top)/2) + 1 ,1);cprintf(" %s ",Top);
 textattr(Kolory[10]);
 gotoxy(3+(57-24)/2-(strlen(Str1)/2) + 1,3);cprintf("%s",Str1);
 gotoxy(3+(57-24)/2-(strlen(Str2)/2) + 1,4);cprintf("%s",Str2);
 window(1,1,80,25);
 Button(36,15,44,1,_OK);
 Key=0;SKey=0;
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
 for (temp=0;temp!=4000;temp++) pokeb(0xB800,temp,screen[temp]);
}

void StatusBar(unsigned char *Str,unsigned char Num)
{
 switch (Num)
 {
  case 0:textattr(Kolory[4]);
	 window(1,25,80,25);clrscr();
	 cprintf(" %s ",Status);
	 break;
  case 1:textattr(Kolory[4]);
	 window(1,25,80,25);
	 gotoxy(29,1);print__HEX(Address,"h");
	 break;
  case 2:textattr(Kolory[4]);
	 window(1,25,80,25);
	 ltoa(CodeLine+1,StrA,10);
	 gotoxy(29,1);cprintf("%-5s",StrA);
	 break;
 default:textattr(Kolory[14]);
	 window(1,25,80,25);clrscr();
	 cprintf(" %s ",Str);
	 break;
 }
 window(1,1,80,25);
}

void WriteStatus(unsigned char *Str,unsigned char Num)
{
 unsigned char temp,q;
 unsigned char _Emu[18]=" Emulator EPROM  ";
 unsigned char _Men[12]="  F9 - Menu";
 switch (Num)
 {
  case 0:strcpy(Status,_Emu);
	 Status[17]=Ramki[0][5];Status[18]=0;
	 strcat(Status,"  Adres: ");
	 for (temp=27;temp<65;temp++) Status[temp]=SPACE;
	 for (temp=27,q=0;temp<65;temp++,q++)
	 {
	  if (Str[q]==0) break;
	  Status[temp]=Str[q];
	 }
	 for (;temp<65;temp++) Status[temp]=SPACE;
	 Status[65]=Ramki[0][5];
	 Status[66]=0;
	 strcat(Status,_Men);
	 break;
  case 1:strcpy(Status,_Emu);
	 Status[17]=Ramki[0][5];Status[18]=0;
	 strcat(Status,"  Linia: ");
	 for (temp=27;temp<65;temp++) Status[temp]=SPACE;
	 for (temp=27,q=0;temp<65;temp++,q++)
	 {
	  if (Str[q]==0) break;
	  Status[temp]=Str[q];
	 }
	 for (;temp<65;temp++) Status[temp]=SPACE;
	 Status[65]=Ramki[0][5];
	 Status[66]=0;
	 strcat(Status,_Men);
	 break;
 }
}

unsigned char printView(unsigned char Type)
{
 unsigned char q;
 textattr(Kolory[5]);
 window(1,2,80,24);clrscr();
 window(1,1,80,25);
 switch (Type)
 {
  case 1:textattr(Kolory[6]);
	 gotoxy(1,2);putch(Ramki[1][0]);		     //4 rogi
	 gotoxy(80,2);putch(Ramki[1][1]);
	 gotoxy(1,24);putch(Ramki[1][2]);
	 gotoxy(80,24);putch(Ramki[1][3]);		     //

	 gotoxy(2,2);for (q=1;q!=79;q++) putch(Ramki[1][4]); //top
	 gotoxy(2,24);for (q=1;q!=79;q++) putch(Ramki[1][4]);//bottom
	 for (q=3;q!=24;q++)                                 //pionowe linie
	 {
	  gotoxy(1,q); putch(Ramki[1][5]);
	  gotoxy(11,q); putch(Ramki[0][5]);
	  gotoxy(63,q);putch(Ramki[0][5]);
	  gotoxy(80,q);putch(Ramki[1][5]);
	 }
	 gotoxy(11,2); putch(Ramki[2][0]);//odczep gorny
	 gotoxy(63,2);putch(Ramki[2][0]);
	 gotoxy(11,24); putch(Ramki[2][1]);//odczep dolny
	 gotoxy(63,24);putch(Ramki[2][1]);
	 textattr(Kolory[8]);
	 for (q=3;q!=24;q++){ gotoxy(29,q); putch(':'); }
	 break;
  case 2:textattr(Kolory[6]);
	 gotoxy(1,2);putch(Ramki[1][0]);		     //4 rogi
	 gotoxy(80,2);putch(Ramki[1][1]);
	 gotoxy(1,24);putch(Ramki[1][2]);
	 gotoxy(80,24);putch(Ramki[1][3]);		     //
	 gotoxy(2,2);for (q=1;q!=79;q++) putch(Ramki[1][4]); //top
	 gotoxy(2,24);for (q=1;q!=79;q++) putch(Ramki[1][4]);//bottom
	 for (q=3;q!=24;q++)                                 //pionowe linie
	 {
	  gotoxy(1,q); putch(Ramki[1][5]);
	  gotoxy(61,q);putch(Ramki[0][5]);
	  gotoxy(80,q);putch(Ramki[1][5]);
	 }
	 gotoxy(61,2); putch(Ramki[2][0]);//odczep gorny
	 gotoxy(61,24); putch(Ramki[2][1]);//odczep dolny
	 textattr(Kolory[8]);
	 for (q=3;q!=24;q++){ gotoxy(9,q); putch(':'); }
	 break;
  case 3:textattr(Kolory[6]);
	 gotoxy(1,2);putch(Ramki[1][0]);		     //4 rogi
	 gotoxy(80,2);putch(Ramki[1][1]);
	 gotoxy(1,24);putch(Ramki[1][2]);
	 gotoxy(80,24);putch(Ramki[1][3]);		     //
	 gotoxy(2,2);for (q=1;q!=79;q++) putch(Ramki[1][4]); //top
	 gotoxy(2,24);for (q=1;q!=79;q++) putch(Ramki[1][4]);//bottom
	 for (q=3;q!=24;q++)                                 //pionowe linie
	 {
	  gotoxy(1,q); putch(Ramki[1][5]);
	  gotoxy(47,q); putch(Ramki[0][5]);
	  gotoxy(67,q);putch(Ramki[0][5]);
	  gotoxy(80,q);putch(Ramki[1][5]);
	 }
	 gotoxy(47,2); putch(Ramki[2][0]);//odczep gorny
	 gotoxy(67,2);putch(Ramki[2][0]);
	 gotoxy(47,24); putch(Ramki[2][1]);//odczep dolny
	 gotoxy(67,24);putch(Ramki[2][1]);
	 textattr(Kolory[8]);
	 for (q=3;q!=24;q++){ gotoxy(19,q); putch(':'); }
	 break;
  case 4:textattr(Kolory[6]);
	 gotoxy(1,2);putch(Ramki[1][0]);		     //4 rogi
	 gotoxy(80,2);putch(Ramki[1][1]);
	 gotoxy(1,24);putch(Ramki[1][2]);
	 gotoxy(80,24);putch(Ramki[1][3]);		     //
	 gotoxy(2,2);for (q=1;q!=79;q++) putch(Ramki[1][4]); //top
	 gotoxy(2,24);for (q=1;q!=79;q++) putch(Ramki[1][4]);//bottom
	 for (q=3;q!=24;q++)                                 //pionowe linie
	 {
	  gotoxy(1,q); putch(Ramki[1][5]);
	  gotoxy(27,q); putch(Ramki[0][5]);
	  gotoxy(53,q);putch(Ramki[0][5]);
	  gotoxy(80,q);putch(Ramki[1][5]);
	 }
	 gotoxy(27,2); putch(Ramki[2][0]);//odczep gorny
	 gotoxy(53,2);putch(Ramki[2][0]);
	 gotoxy(27,24); putch(Ramki[2][1]);//odczep dolny
	 gotoxy(53,24);putch(Ramki[2][1]);
	 textattr(Kolory[8]);
	 for (q=3;q!=24;q++)
	 {
	  gotoxy(9,q); putch(':');
	  gotoxy(35,q); putch(':');
	  gotoxy(61,q); putch(':');
	 }
	 break;
 }
 Refresh=1;
 return Type;
}

unsigned int printDeASMPage(unsigned int Addr,unsigned char x1,
			    unsigned char x2, unsigned char x3,
			    unsigned char x4)
{
 unsigned char temp,q,len;
 unsigned int Addr1;
   CodeLine=ASMLine;
   for (Addr1=Addr,q=1;q!=22;q++)//wypisuje strone listingu
   {
    if (Addr1>MaxbufferA) Addr1=0;
    textattr(Kolory[8]);
    ltoa(CodeLine+1,StrA,10);
    gotoxy(x1,q);cprintf("%-5s",StrA);CodeLine++;
    if ( (CodeLine)>MaxbufferB ) CodeLine=0;
    gotoxy(x2,q);cprintf("%-15s",SPACE);
    if (!printLabelRight(x2+15,q,Addr1))
    {
     gotoxy(x2+10,q);
     print__HEX(Addr1,"h");
    }
    textattr(Kolory[5]);
    gotoxy(x3,q);
    Addr1=printDeASM(Addr1);
    for (temp=wherex();temp!=62;temp++) putch(SPACE);
    len=CodeLen[Code];
    gotoxy(x4,q);print_HEX(Code,"        ");
    gotoxy(x4+4,q);
    if (len==2) {
		 print_HEX(bufferA[Addr1-1],"    ");
		 gotoxy(x4+8,q);
		}
    if (len==3) {
		 print_HEX(bufferA[Addr1-2],"  ");
		 print_HEX(bufferA[Addr1-1],"  ");
		}
   }
   Refresh=0;
   return Addr1;
}

unsigned int printHEX16Page(unsigned int Addr,unsigned char x1,
			    unsigned char x2, unsigned char x3)
{
 unsigned char temp,q,w;
 unsigned int Addr1;
   for (Addr1=Addr,q=1;q!=22;q++)//wypisuje strone listingu
   {
    textattr(Kolory[8]);
    gotoxy(x1,q);print__HEX(Addr1,"h");
    textattr(Kolory[5]);
    w=0;
    for (temp=0;temp!=16;temp++)
    {
     Code=bufferA[Addr1];
     Addr1=MOD(Addr1+1,MaxbufferA);
     gotoxy(x2+temp*3+w,q);print_HEX(Code,"");
     if (temp==7) w=1;
     gotoxy(x3+temp,q);
     if (Code>=32) putch(Code);
     else putch('.');
    }
   }
   Refresh=0;
   return Addr1;
}

unsigned int printHEX8Page(unsigned int Addr,
			   unsigned char x1,unsigned char x2)
{
 unsigned char temp,q;
 unsigned int Addr1;
 textattr(Kolory[5]);
 for (Addr1=Addr,q=1;q!=22;q++)
  for (temp=0;temp!=8;temp++)
  {
   Code=bufferA[Addr1];
   Addr1=MOD(Addr1+1,MaxbufferA);
   gotoxy(x1+temp*3,q);print_HEX(Code,"");
   gotoxy(x2+temp,q);
   if (Code>=32) putch(Code);
   else putch('.');
  }
 Refresh=0;
 return Addr1;
}

unsigned int printHEX8Page2(unsigned int Addr,
			    unsigned char x1,unsigned char x2)
{
 unsigned char temp,q;
 unsigned int Addr1;
 for (Addr1=Addr,q=1;q!=22;q++)
 {
  textattr(Kolory[8]);
  gotoxy(x1,q);cprintf("%-15s",SPACE);
  if (!printLabelRight(x1+15,q,Addr1))
  {
   gotoxy(x1+10,q);print__HEX(Addr1,"h");
  }
  textattr(Kolory[5]);
  gotoxy(x2,q);print_BIN(bufferA[Addr1],"");
  Addr1=MOD(Addr1+0x08,MaxbufferA);
 }
 Refresh=0;
 return Addr1;
}

unsigned int printBMPPage(unsigned int Addr,unsigned char x1,
			  unsigned char x2, unsigned char x3,
			  unsigned char x4, unsigned char x5,
			  unsigned char x6)
{
 unsigned char temp,q,w;
 unsigned int Addr1;
   for (Addr1=Addr,q=1;q!=22;q++)
   {
    textattr(Kolory[8]);
    gotoxy(x1,q);print__HEX(Addr1,"h");
    textattr(Kolory[5]);
    gotoxy(x2,q);print_xBIN(bufferA[Addr1],"");
    Addr1=MOD(Addr1+1,MaxbufferA);
   }
   for (q=1;q!=22;q++)
   {
    textattr(Kolory[8]);
    gotoxy(x3,q);print__HEX(Addr1,"h");
    textattr(Kolory[5]);
    gotoxy(x4,q);print_xBIN(bufferA[Addr1],"");
    Addr1=MOD(Addr1+1,MaxbufferA);
   }
   for (q=1;q!=22;q++)
   {
    textattr(Kolory[8]);
    gotoxy(x5,q);print__HEX(Addr1,"h");
    textattr(Kolory[5]);
    gotoxy(x6,q);print_xBIN(bufferA[Addr1],"");
    Addr1=MOD(Addr1+1,MaxbufferA);
   }
   Refresh=0;
   return Addr1;
}

unsigned char MenuBar(unsigned char Sel,unsigned char Clear)
{
 unsigned char i,j,old;
 if (Clear==1)
 {
  if (Sel==0)
  {
   gotoxy(1,1);textattr(Kolory[1]);for (i=1;i<=80;i++) putch(SPACE);
   for (j=0;j!=MaxMenuNumX;j++)
   {
    gotoxy(Begins[j],1);
    for (i=0;(i<MaxMenuLen)&&(MainMenu[j][i]!=0);i++) putch(MainMenu[j][i]);
   }
  }
  else
  {
   if (Sel>MaxMenuNumX) Sel=MaxMenuNumX;
   textattr(Kolory[1]);
   gotoxy(Begins[Sel-1]-2,1);
   for (i=0;i!=2;i++) putch(SPACE);
   gotoxy(Begins[Sel-1],1);
   for (i=0;(i<MaxMenuLen)&&(MainMenu[Sel-1][i]!=0);i++) putch(MainMenu[Sel-1][i]);
   for (i=0;i!=2;i++) putch(SPACE);
  }
 }
 else
 {
  old=Sel;Key=0;SKey=0;OK=0;
  do {
   if (Sel==0) Sel=MaxMenuNumX;
   if (Sel>MaxMenuNumX) Sel=1;
   textattr(Kolory[1]);
   gotoxy(Begins[old-1]-2,1);
   for (i=0;i!=2;i++) putch(SPACE);
   gotoxy(Begins[old-1],1);
   for (i=0;(i<MaxMenuLen)&&(MainMenu[old-1][i]!=0);i++) putch(MainMenu[old-1][i]);
   for (i=0;i!=2;i++) putch(SPACE);
   old=Sel;
   textattr(Kolory[3]);
   gotoxy(Begins[Sel-1]-2,1);
   for (i=0;i!=2;i++) putch(SPACE);
   gotoxy(Begins[Sel-1],1);
   for (i=0;(i<MaxMenuLen)&&(MainMenu[Sel-1][i]!=0);i++) putch(MainMenu[Sel-1][i]);
   for (i=0;i!=2;i++) putch(SPACE);
   StatusBar(StatusMain[Sel-1],255);
   while (!kbhit()) {}
   if (kbhit()) Key=getch();
   if (Key==0){ SKey=1;Key=getch(); }
   else SKey=0;
   if ((SKey)&&(Key==RIGHT)) Sel++;
   if ((SKey)&&(Key==LEFT)) Sel--;
   if ((!SKey && Key==ENTER)||(SKey && Key==DOWN)) OK=1;
  } while ((SKey)&&((Key==RIGHT)||(Key==LEFT)));
}
 return Sel;
}

void RollDown(unsigned char x1,unsigned char y1,
	      unsigned char x2,unsigned char y2,MenuStr Menu)
{
 unsigned char i,j;
 SaveScreen();
 textattr(Kolory[2]);gotoxy(x1,y1);putch(Ramki[0][0]);
 for (i=x1+1;i<x2;i++){ gotoxy(i,y1);putch(Ramki[0][4]); }
 gotoxy(x2,y1);putch(Ramki[0][1]);

 for (j=y1+1;j<y2;j++)
 {
  gotoxy(x1,j);putch(Ramki[0][2]);
  for (i=x1+1;i<x2;i++){ gotoxy(i,j);putch(Ramki[0][4]); }
  gotoxy(x2,j);putch(Ramki[0][3]);

  delay(ScrollTime);
  gotoxy(x1,j);putch(Ramki[0][5]);
  putch(32);
  for (i=x1+2;i<=x2-2;i++)
  {
   gotoxy(i,j);
   if ((i-x1-2>=MaxMenuLen)||(j-y1-1>=MaxMenuNumY)) putch(32);
   else {
    if (Menu[j-y1-1][i-x1-2]>32) putch(Menu[j-y1-1][i-x1-2]);
    else putch(32);
   }
  }
  putch(32);
  gotoxy(x2,j);putch(Ramki[0][5]);
 }

 gotoxy(x1,y2);putch(Ramki[0][2]);
 for (i=x1+1;i<x2;i++){ gotoxy(i,y2);putch(Ramki[0][4]); }
 gotoxy(x2,y2);putch(Ramki[0][3]);
}

void RollUp(unsigned char x1,unsigned char y1,
	    unsigned char x2,unsigned char y2)
{
 unsigned char i,j;
 textattr(Kolory[2]);
 for (i=y2-1;i>y1;i--)
 {
  RestoreLine(i+1);
  gotoxy(x1,i);putch(Ramki[0][2]);
  for (j=x1+1;j<x2;j++){ gotoxy(j,i);putch(Ramki[0][4]); }
  gotoxy(x2,i);putch(Ramki[0][3]);
  delay(ScrollTime);
 }
 RestoreLine(y1+1);
 RestoreLine(y1);
}

unsigned char Select(unsigned char x1,unsigned char y1,
		     unsigned char x2,unsigned char y2,
		     MenuStr Menu,unsigned char Sel)
{
 unsigned char i,old;
 RollDown(x1,y1,x2,y2,Menu);
 Key=0;SKey=0;OK=0;
 if (Sel>y2-y1-1) Sel=1;
 if (Sel>MaxMenuNumY) Sel=1;
 if (Sel==0) Sel=y2-y1-1;
 old=Sel;
 do {
  textattr(Kolory[3]);
  gotoxy(x1+1,y1+Sel);
  putch(32);
  for (i=x1+2;i<=x2-2;i++)
  {
   if ((i-x1-2>=MaxMenuLen)||(Sel-y1-1>=MaxMenuNumY)) putch(32);
   else {
    if (Menu[Sel-1][i-x1-2]>32) putch(Menu[Sel-1][i-x1-2]);
    else putch(32);
   }
  }
  putch(32);
  switch (MenuNumX)
  {
   case 1:StatusBar(StatusFile[Sel-1],255);break;
   case 2:StatusBar(StatusEdit[Sel-1],255);break;
   case 3:StatusBar(StatusView[Sel-1],255);break;
   case 4:StatusBar(StatusSym[Sel-1],255);break;
   case 5:StatusBar(StatusSetup[Sel-1],255);break;
   case 6:StatusBar(StatusHelp[Sel-1],255);break;
  }
  while (!kbhit()) {}
  if (kbhit()) Key=getch();
  if (Key==0){ SKey=1;Key=getch(); }
  else SKey=0;
  if ((SKey)&&(Key==DOWN))
     {
       Sel++;
       if (Menu[Sel-1][0]==0) Sel++;
       if (Sel>y2-y1-1) Sel=1;
       if (Sel>MaxMenuNumY) Sel=1;
     }
  if ((SKey)&&(Key==UP))
     {
       Sel--;
       if (Sel==0) Sel=y2-y1-1;
       if (Menu[Sel-1][0]==0) Sel--;
     }
  if (!SKey && Key==ENTER) OK=1;
  textattr(Kolory[2]);
  gotoxy(x1+1,y1+old);
  putch(32);
  for (i=x1+2;i<=x2-2;i++)
  {
   if ((i-x1-2>=MaxMenuLen)||(old-y1-1>=MaxMenuNumY)) putch(32);
   else {
    if (Menu[old-1][i-x1-2]>32) putch(Menu[old-1][i-x1-2]);
    else putch(32);
   }
  }
  putch(32);
  old=Sel;
 } while (SKey && ( Key==DOWN || Key==UP ));
 RollUp(x1,y1,x2,y2);
 return Sel;
}

void MakeStatus(void)
{
 WriteStatus("",0);
 StatusBar("",0);
 window(2,3,79,23);
}

void printFilepage(unsigned int Fpos)
{
 unsigned int w,q;
 for (w=0,q=Fpos;w<10;w++,q++)
 {
  if (q>=FileMax) { gotoxy(5,w+7);cprintf("%-12s",SPACE); }
  else { gotoxy(5,w+7);cprintf("%-12s",FileList[q]); }
 }

}

void printDirpage(unsigned int Dpos)
{
 unsigned int w,q;
 for (w=0,q=Dpos;w<10;w++,q++)
 {
  if (q>=DirMax) { gotoxy(22,w+7);cprintf("%-12s",SPACE); }
  else { gotoxy(22,w+7);cprintf("%-12s",DirList[q]); }
 }
}

void printFileName(unsigned int fpos,unsigned int f)
{
 textattr(Kolory[7]);
 gotoxy(30,3);cprintf("%-12s",FileList[(fpos+f)-1]);
 strcpy(FileName,FileList[(fpos+f)-1]);
}

void printHLPPage(unsigned int Line)
{
 unsigned int temp,pos;
 unsigned char ln;
 pos=(Line-1)*152;
 for (ln=3;ln<=23;ln++)
 for (temp=((ln-1)*160)+4;temp!=(ln*160)-4;temp++,pos++)
 pokeb(0xB800,temp,bufferB[pos]);
}

void HlpWindow(unsigned char x1,unsigned char y1,
	       unsigned char x2,unsigned char y2)
{
 unsigned char q;
 window(x1,y1,x2,y2);textattr(Kolory[5]);clrscr();
 window(1,1,80,25);
 gotoxy(x1,y1);putch(Ramki[1][0]);		     //4 rogi
 gotoxy(x2,y1);putch(Ramki[1][1]);
 gotoxy(x1,y2);putch(Ramki[1][2]);
 gotoxy(x2,y2);putch(Ramki[1][3]);		     //
 gotoxy(x1+1,y1);for (q=x1+1;q<x2;q++) putch(Ramki[1][4]); //top
 gotoxy(x1+1,y2);for (q=x1+1;q<x2;q++) putch(Ramki[1][4]);//bottom
 for (q=y1+1;q<y2;q++)                                 //pionowe linie
 {
  gotoxy(x1,q);putch(Ramki[1][5]);
  gotoxy(x2,q);putch(Ramki[1][5]);
 }
 window(x1+1,y1,x2-1,y2-1);
}
