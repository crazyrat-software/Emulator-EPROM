void Koniec(unsigned char num)
{
 if (num!=0)
 {
  window(1,1,80,25);
  textcolor(7);textbackground(0);
  clrscr();
  asm {
       mov ax,0x0003;
       int 0x10;
       mov al,0xF3;
       out 0x60,al;
  }
  delay(100);
  asm {
       mov al,0x24;
       out 0x60,al;
  }
  farfree(bufferA);
  farfree(bufferB);
 }
 if (num==0)
 {
  InitPIC(OldIRQNum+0x08,OldIRQMask);
  setvect(OldIRQNum+0x08,OldIRQProc);
  setvect(0x1C,Old0x1CProc);
  setvect(0x23,Old0x23Proc);
  setvect(0x1B,Old0x1BProc);
  printf("\n\rKoniec...\n");
  exit(0);
 }
}

void WriteError(unsigned char Num,
		unsigned char Fatal,unsigned char *ErrorStr)
{
 if (Fatal!=0) Koniec(1);
 printf("\n\r");
 switch (Num) {
	       case 0: printf("\r\nBrak pliku EEPROM.CFG !");
		       printf("\r\nZostana uzyte ustawienia domyslne.");
		       break;
	       case 1: printf("\r\nBlad w pliku EEPROM.CFG !");
		       printf("\r\nNieznany parametr: ' %s '.",ErrorStr);
		       break;
	       case 2: printf("\r\nBlad w pliku EEPROM.CFG !");
		       printf("\r\nWartosc parametru poza zakresem: ' %s '.",ErrorStr);
		       break;
	       case 3: printf("\r\nBrak pliku %s !",ErrorStr);
		       break;
	       case 4: printf("\r\nBlad w pliku %s !",ErrorStr);
		       break;
	       case 5: printf("\r\nUwaga !");
		       printf("\r\nNieprawidlowy adres SFR.");
		       break;
	       case 6: printf("\r\nUwaga !");
		       printf("\r\nNieprawidlowy adres bitu.");
		       break;
	      }
 printf("\r\n\a");
 if (Fatal==0) delay(700);
 if (Fatal!=0) exit(Fatal);
}