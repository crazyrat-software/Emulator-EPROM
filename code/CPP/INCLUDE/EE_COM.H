//procedury obslugi przerwan
void interrupt Timer(...)		//timer na przerwaniu 0x1C
{
   TimeOut++;
   Old0x1CProc();
}

void interrupt _ReadCOM(...)		//procedura IRQ obslugi portu COM
{
   unsigned char temp;
   asm cli;
   temp=inportb(COMAddr);
   TimeOut=0;
   switch (DataType)
   {
    case 0:sound(1000);delay(10);nosound();break;
    case 1:TempBuf[_BytePtr]=temp;_BytePtr++;CheckSum+=temp;
	   if (_BytePtr>=PacketSize) { DataType=2;_BytePtr=0;}
	   break;
    case 2:crcH=temp;_BytePtr++;DataType=3;
	   break;
    case 3:crcL=temp;_BytePtr++;DataType=0;
	   break;
   }
   asm sti;
   asm mov al,0x20;
   asm out 0x20,al;
}

void interrupt Dumb(...)		//
{
 sound(1000);
 delay(50);
 nosound();
}


void  WaitEE(unsigned char time)
{
 if (time==0) delay(1);
 else delay(time);
}
//procedury systemowe
unsigned char  InitPIC(unsigned char IRQ,unsigned char Mode)
{
 unsigned char mask,ret;

 mask=inportb(0x21);
 ret=mask;ret=(ret & (0x01<<IRQ));ret>>=IRQ;ret=!ret;
 switch (Mode)
 {
  case 0:mask=(mask | (0x01<<IRQ));
	 outportb(0x21,mask);
	 break;
  case 1:mask=(mask & (0xFE<<IRQ));
	 outportb(0x21,mask);
	 break;
  default:break;
 }
 return (ret);
}

void  SetIRQProc(unsigned char IRQ)
{
 const unsigned char ofs=0x08;
 InitPIC(OldIRQNum+ofs,OldIRQMask);
 setvect(OldIRQNum+ofs,OldIRQProc);
 OldIRQMask=InitPIC(IRQ+ofs,1);
 OldIRQProc=getvect(IRQ+ofs);setvect(IRQ+ofs,_ReadCOM);
 OldIRQNum=IRQ;IRQFlag=1;
}

unsigned int  GetUARTAddr(unsigned char Num)	 //zwraca adres portu
{
 unsigned int temp;

  MaxCOMNum=peekb(0x0,0x0411);	         //pobiera bajt systemowy
  MaxCOMNum&=0x0e;                        //wazne sa bity 1, 2 i 3
  MaxCOMNum>>=1;                          //przesuwa o 1 w prawo
  temp=peek(0x0,0x3FE+Num*2);		 //oblicza adres zmiennej systemowej
  if ((Num<1) || (Num>MaxCOMNum)) temp=0; //zawierajacej adresy portow
  return temp;
}

unsigned char  InitUART(unsigned int pAddr,unsigned int BPS)
{
 unsigned int dzielnik;
 unsigned char Hi,Lo,q;

 if (BPS<300) BPS=300;		                //min predkosc 300 bps
 Wait=1;
 if (BPS<9600) Wait=5;
 if (BPS<2400) Wait=10;
 dzielnik=115200/BPS;
 Hi=(dzielnik&0xFF00)>>8;
 Lo=dzielnik&0x00FF;

 outportb(pAddr+3,(inportb(pAddr+3) | 0x80));       //ustaw bit DLAB
 _DLL=inportb(pAddr); outportb(pAddr,Lo);      //mlodszy bajt dzielnika
 _DLH=inportb(pAddr+1); outportb(pAddr+1,Hi);    //starszy bajt dzielnika
 outportb(pAddr+3,(inportb(pAddr+3) & 0x7F));       //zeruj bit DLAB
 _IER=inportb(pAddr+1); outportb(pAddr+1,0x01);     //przerwanie gdy znak odebrany
 _IID=inportb(pAddr+2);
 _LCR=inportb(pAddr+3); outportb(pAddr+3,0x03);	//ustawienie trybu na 8,n,1
 _MCR=inportb(pAddr+4); outportb(pAddr+4,0x08); //enable OUT2
 return 0;
}

void  ClearB(unsigned int pAddr)
{
 TimeOut=0;
 do {
   Byte=inportb(pAddr);
   COMstatus=inportb( pAddr+0x05 );
   if (TimeOut>550) break;
 } while ((COMstatus&0x40)!=0x40);
}

void  WaitUART(unsigned int pAddr)
{
 TimeOut=0;
 do {
   COMstatus=inportb( pAddr+0x05 );
   if (TimeOut>550) break;
 } while ((COMstatus&0x40)!=0x40);
}

unsigned char  CheckCRC(void)
{
 unsigned int temp;

 temp=(crcH*256)+crcL;
 if (CheckSum==temp){ COMerror=0;return 0;}
 COMerror=5;
 return 1;
}

unsigned char  SendB(unsigned int pAddr,unsigned char Byte)
{
 TimeOut=0;
 do
 {
   COMstatus=inportb( pAddr+0x05 );
   if ( (COMstatus & 0x40) == 0x40)
   {
    outportb(pAddr,Byte);
    CheckSum+=Byte;
    COMerror=0;
    return 0;
   }
 } while (TimeOut<=MaxTimeOut);
 COMerror=1;
 return 1;
}

unsigned char  EE_Speed(unsigned int mode)
{
 unsigned int temp;
 if (mode>57600) temp=57600;
 else temp=mode;
 switch (temp)
 {
  case 300:  temp=0;break;
  case 600:  temp=1;break;
  case 1200: temp=2;break;
  case 2400: temp=3;break;
  case 4800: temp=4;break;
  case 9600: temp=5;break;
  case 19200:temp=6;break;
  case 57600:temp=7;break;
 }
 if (temp>7) temp=7;
 ClearB(COMAddr);CheckSum=0;
 SendB(COMAddr,0xC0);WaitEE(Wait);
 SendB(COMAddr,0x00);WaitEE(Wait);
 SendB(COMAddr,temp);
 WaitUART(COMAddr);
 if (COMerror!=0) return 2;
 switch (temp)
 {
  case 0: InitUART(COMAddr,300);break;
  case 1: InitUART(COMAddr,600);break;
  case 2: InitUART(COMAddr,1200);break;
  case 3: InitUART(COMAddr,2400);break;
  case 4: InitUART(COMAddr,4800);break;
  case 5: InitUART(COMAddr,9600);break;
  case 6: InitUART(COMAddr,19200);break;
  case 7: InitUART(COMAddr,57600);break;
 }
 DataType=2;_BytePtr=0;TimeOut=0;
 do {
     if (TimeOut>20) break;
 } while (DataType!=0);
 if (CheckCRC())
 {
  InitUART(COMAddr,9600);
  return 3;
 }
 return 0;
}

unsigned char  Write_EE(unsigned int mAddr,unsigned int bAddr,
		     unsigned long Leng)
{
 unsigned int addr,pos,reszta,blok,i,b,c,diver;
 unsigned char Lo,Hi,Byte;

 diver=Leng/33;
 blok=Leng/MaxSize;		//ilosc blokow MaxSize bajtowych
 reszta=Leng%MaxSize;		//ilosc bajtow reszty
 pos=bAddr;addr=mAddr;
 COMerror=0;
 for (i=0;i!=blok;i++)
 {
  CRCchecks=0;PacketSize=MaxSize;
  addr=(i*MaxSize)+mAddr;
  Hi=(addr&0xFF00)>>8;Lo=addr&0x00FF;
  for (Byte=0;Byte<PacketSize;Byte++,pos++) TempBuf[Byte]=bufferA[pos];
  do
  {
   CheckSum=0;CRCchecks++;
   SendB(COMAddr,PacketSize);WaitEE(Wait);
   SendB(COMAddr,Hi);WaitEE(Wait);
   SendB(COMAddr,Lo);
   if (COMerror!=0) return 2;
   for (Byte=0;Byte<PacketSize;Byte++)
   {
    WaitEE(0);SendB(COMAddr,TempBuf[Byte]);
   }
   DataType=2;_BytePtr=0;TimeOut=0;
   do {
       if (TimeOut>MaxTimeOut) break;
       if (kbhit())
       {
	Key=getch();
	if (Key==0) { SKey=1;Key=getch();}
	else SKey=0;
       }
   } while (DataType!=0);
   if ((!SKey)&&(Key==27)) return 0;
   textattr(Kolory[10]);gotoxy(24,14);Progress(33,(pos-bAddr)/diver);
  } while (CheckCRC() && (CRCchecks<=MaxCRCchecks));
  if (CRCchecks>MaxCRCchecks) return 4;
 }
 if (COMerror==5) return 2;
 if ((reszta>0)&(reszta<64))
 {
  if (Leng>=64) addr+=MaxSize;
  CRCchecks=0;PacketSize=reszta;
  Hi=(addr&0xFF00)>>8;Lo=addr&0x00FF;
  for (Byte=0;Byte<PacketSize;Byte++,pos++) TempBuf[Byte]=bufferA[pos];
  do
  {
   CheckSum=0;CRCchecks++;
   SendB(COMAddr,PacketSize);WaitEE(Wait);
   SendB(COMAddr,Hi);WaitEE(Wait);
   SendB(COMAddr,Lo);
   if (COMerror!=0) return 2;
   for (Byte=0;Byte<PacketSize;Byte++)
   {
    WaitEE(0);SendB(COMAddr,TempBuf[Byte]);
   }
   DataType=2;_BytePtr=0;TimeOut=0;
   do {
       if (TimeOut>MaxTimeOut) break;
       if (kbhit())
       {
	Key=getch();
	if (Key==0) { SKey=1;Key=getch();}
	else SKey=0;
	if ((!SKey)&&(Key==27)) return 0;
       }
      } while (DataType!=0);
  } while (CheckCRC() && (CRCchecks<=MaxCRCchecks));
  if (CRCchecks>MaxCRCchecks) return 4;
 }
 return 0;
}

unsigned char  Read_EE(unsigned int mAddr,unsigned int bAddr,
		     unsigned long Leng)
{
 unsigned int addr,pos,reszta,blok,i,b,c,diver;
 unsigned char Lo,Hi,Byte;

 diver=Leng/33;
 blok=Leng/MaxSize;		//ilosc blokow MaxSize bajtowych
 reszta=Leng%MaxSize;		//ilosc bajtow reszty
 pos=bAddr;addr=mAddr;
 PacketSize=MaxSize;COMerror=0;
 for (i=0;i!=blok;i++)
 {
  addr=(i*MaxSize)+mAddr;CRCchecks=0;
  Hi=(addr&0xFF00)>>8;Lo=addr&0x00FF;
  do
  {
   CheckSum=0;CRCchecks++;
   SendB(COMAddr,0x40+PacketSize);WaitEE(0);
   SendB(COMAddr,Hi);WaitEE(0);
   SendB(COMAddr,Lo);
   if (COMerror!=0) return 2;
   DataType=1;_BytePtr=0;TimeOut=0;
   do {
       if (TimeOut>MaxTimeOut) break;
       if (kbhit())
       {
	Key=getch();
	if (Key==0) { SKey=1;Key=getch();}
	else SKey=0;
       }
       textattr(Kolory[10]);gotoxy(24,14);
       Progress(33,(pos-bAddr)/diver);
   } while (DataType!=0);
   if ((!SKey)&&(Key==27)) return 0;
  } while (CheckCRC() && (CRCchecks<=MaxCRCchecks));
  if (CheckCRC()==0) for (Byte=0;Byte<PacketSize;Byte++,pos++) bufferA[pos]=TempBuf[Byte];
  if (CRCchecks>MaxCRCchecks) return 4;
 }
 if (COMerror==5) return 2;
 if ((reszta>0)&(reszta<64))
 {
  if (Leng>=64) addr+=MaxSize;
  CRCchecks=0;PacketSize=reszta;
  Hi=(addr&0xFF00)>>8;Lo=addr&0x00FF;
  do
  {
   CheckSum=0;CRCchecks++;
   SendB(COMAddr,0x40+PacketSize);WaitEE(0);
   SendB(COMAddr,Hi);WaitEE(0);
   SendB(COMAddr,Lo);
   if (COMerror!=0) return 2;
   DataType=1;_BytePtr=0;TimeOut=0;
   do {
       if (TimeOut>MaxTimeOut) break;
       if (kbhit())
       {
	Key=getch();
	if (Key==0) { SKey=1;Key=getch();}
	else SKey=0;
       }
   } while (DataType!=0);
   if ((!SKey)&&(Key==27)) return 0;
  } while (CheckCRC() && (CRCchecks<=MaxCRCchecks));
  if (CheckCRC()==0)
   for (Byte=0;Byte<PacketSize;Byte++,pos++) bufferA[pos]=TempBuf[Byte];
  if (CRCchecks>MaxCRCchecks) return 4;
 }
 return 0;
}
