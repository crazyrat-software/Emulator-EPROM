#include <stdio.h>
#include <dos.h>
#include <conio.h>

unsigned char MaxCOMNum;
unsigned char COMNum;
unsigned char Copyright[]=
{"Wszystkie prawa zastrzezone. "
 "Detect 1999 "
 "Autor: Tomasz Nowakowski "
 "Tel  : (0-59) 683-08-43" };

void print__HEX(unsigned int BIN,unsigned char *Str)
{
 unsigned char temp;
 unsigned char WordStr[5];
 const unsigned char hextab[16]= {"0123456789ABCDEF"};
 temp=(BIN&0xF000)>>12;WordStr[0]=hextab[temp];
 temp=(BIN&0x0F00)>>8; WordStr[1]=hextab[temp];
 temp=(BIN&0x00F0)>>4; WordStr[2]=hextab[temp];
 temp=(BIN&0x000F);    WordStr[3]=hextab[temp];
 WordStr[4]=0;
 printf("%s%s",WordStr,Str);
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

unsigned char DetectUART(unsigned int pAddr)
{
 const unsigned char Dumb=0xF3;
 unsigned char temp,ret,old;

 ret=0;		  //jakis blad
 outportb(pAddr+7,Dumb);
 temp=inportb(pAddr+7);
 if (Dumb!=temp) ret=1;  //8250
 else                    //czy jest 16450, 16550 lub 16550AFN
 {
  old=inport(pAddr+2);
  old&=0xF0;  old>>=6;
  outportb(pAddr+2,0xC7);//ustaw FIFO
  temp=inportb(pAddr+2);
  temp&=0xF0; temp>>=6;
  if (temp==0) ret=2;    //16450
  if (temp==1) ret=3;	  //16550
  if (temp==3) ret=4;	  //16550AFN
  if (old!=0) outportb(pAddr+2,0xC7);
  if (old==0) outportb(pAddr+2,0xC6);
 }
 return ret;
}

void printUART(unsigned char UARTNum)
{
 unsigned char UARTStr[5][13]={"nie wykryto","8250","16450","16550","16550AFN"};
 printf("%s\n",UARTStr[UARTNum]);
}

int main(void)
{
 clrscr();
 GetUARTAddr(1);
 for (COMNum=1;COMNum<=MaxCOMNum;COMNum++)
 {
  printf(" COM%u => Adres: ",COMNum);
  print__HEX(GetUARTAddr(COMNum),"h     UART: ");
  printUART(DetectUART(GetUARTAddr(COMNum)));
 }
 gotoxy(1,wherey()+1);
return MaxCOMNum;
}
