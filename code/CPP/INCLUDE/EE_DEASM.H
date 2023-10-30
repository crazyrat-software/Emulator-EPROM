char _U2(unsigned char BIN)
{
 return BIN;
}

void HEXStrB(unsigned int BIN)
{
 Word=(BIN&0xF000)>>12;StrB[0]=hextab[Word];
 Word=(BIN&0x0F00)>>8; StrB[1]=hextab[Word];
 Word=(BIN&0x00F0)>>4; StrB[2]=hextab[Word];
 Word=(BIN&0x000F);    StrB[3]=hextab[Word];
 StrB[4]=0;
}

void HEX1StrB(unsigned char BIN)
{
 Word=(BIN&0xF0)>>4; StrB[0]=hextab[Word];
 Word=(BIN&0x0F);    StrB[1]=hextab[Word];
 StrB[2]=0;
}

unsigned char LabelStrB(unsigned int Addr)
{
 unsigned int temp;
 unsigned char ret;

 ret=0;
 for (temp=0;temp<=0xff;temp++)
  if ( (LabelAdr[temp]==Addr) && (LabelStr[temp][0]!=0) )
  {
   strcpy(StrB,LabelStr[temp]);
   ret=1;
   break;
  }
 return ret;
}

void print__HEX(unsigned int BIN,unsigned char *Str)
{
 HEXStrB(BIN);
 cprintf("%s%s",StrB,Str);
}

void print_HEX(unsigned char BIN,unsigned char *Str)
{
 HEX1StrB(BIN);
 cprintf("%s%s",StrB,Str);
}

void print_BIN(unsigned char BIN,unsigned char *Str)
{
 unsigned char temp,c;
 for (temp=0,c=128;temp<16;temp+=2,c/=2)
 {
  if ( (BIN&c)==c ) TempBuf[temp]='1';
  else TempBuf[temp]='0';
  TempBuf[temp+1]=SPACE;
 }
 TempBuf[16]=0;
 cprintf("%s%s",TempBuf,Str);
}

void print_xBIN(unsigned char BIN,unsigned char *Str)
{
 unsigned char temp,c;
 for (temp=0,c=128;temp<16;temp+=2,c/=2)
 {
  if ( (BIN&c)==c ) TempBuf[temp]=149;
  else TempBuf[temp]=148;
  TempBuf[temp+1]=SPACE;
 }
 TempBuf[16]=0;
 cprintf("%s%s",TempBuf,Str);
}

unsigned char printLabel(unsigned int Addr)
{
 unsigned int temp;
 unsigned char ret;
 ret=LabelStrB(Addr);
 if (ret) cprintf("%s",StrB);
 return ret;
}

unsigned char printLabelRight(unsigned char x1,unsigned char y1,
			      unsigned int Addr)
{
 unsigned int temp;
 unsigned char ret;

 ret=0;
 for (temp=0;temp<=0xff;temp++)
  if ( (Addr==LabelAdr[temp]) && (LabelStr[temp][0]!=0) )
  {
   textattr(Kolory[9]);
   gotoxy(x1-strlen(LabelStr[temp]),y1);
   cprintf("%s",LabelStr[temp]);
   ret=1;
   break;
  }
 return ret;
}

void _CLabel(unsigned int word)
{
 if (!LabelStrB(word))
 {
  HEXStrB(word);
  strcat(StrB,"h");
 }
 strcat(StrA,StrB);
}

unsigned int printDeASM(unsigned int Addr)
{
 unsigned char temp,byte;
 char U2;
 unsigned int word;

 temp=bufferA[Addr];
 Code=temp;
 strcpy(StrA,MnemStr[temp]);
 if (CodeLen[temp]!=1)
 {
  switch(temp)
  {
   case 0x10://jbc bit,d
   case 0x20://jb bit,d
   case 0x30://jnb bit,d
	     byte=bufferA[Addr+1];
	     if (SFRBit[byte][0]!=0)
	     {
	      sprintf(StrB,"%s,",SFRBit[byte]);
	      strcat(StrA,StrB);
	     }
	     else
	     {
	      sprintf(StrB,"%i,",byte);
	      strcat(StrA,StrB);
	     }
	     word=_U2(bufferA[Addr+2])+Addr+3;
	     _CLabel(word);
	     break;
   case 0x02://ljmp
   case 0x12://lcall
	     word=bufferA[Addr+1]*256+bufferA[Addr+2];
	     _CLabel(word);
	     break;
   case 0x01://ajmp
   case 0x11://acall
   case 0x21://ajmp
   case 0x31://acall
   case 0x41://ajmp
   case 0x51://acall
   case 0x61://ajmp
   case 0x71://acall
   case 0x81://ajmp
   case 0x91://acall
   case 0xA1://ajmp
   case 0xB1://acall
   case 0xC1://ajmp
   case 0xD1://acall
   case 0xE1://ajmp
   case 0xF1://acall
	     word=((temp&0xE0)>>5)*256+bufferA[Addr+1]+((Addr+2)&0xF800);
	     _CLabel(word);
	     break;
   case 0x40://jc d
   case 0x50://jnc d
   case 0x60://jz d
   case 0x70://jnz d
   case 0x80://sjmp d
   case 0xD8://djnz r0
   case 0xD9://djnz r1
   case 0xDA://djnz r2
   case 0xDB://djnz r3
   case 0xDC://djnz r4
   case 0xDD://djnz r5
   case 0xDE://djnz r6
   case 0xDF://djnz r7
	     word=_U2(bufferA[Addr+1])+Addr+2;
	     _CLabel(word);
	     break;
   case 0xB4://cjne a,#n,d
   case 0xB6://cjne @r0,#n,d
   case 0xB7://cjne @r1,#n,d
   case 0xB8://cjne r0,#n,d
   case 0xB9://cjne r1,#n,d
   case 0xBA://cjne r2,#n,d
   case 0xBB://cjne r3,#n,d
   case 0xBC://cjne r4,#n,d
   case 0xBD://cjne r5,#n,d
   case 0xBE://cjne r6,#n,d
   case 0xBF://cjne r7,#n,d
	     HEX1StrB(bufferA[Addr+1]);
	     strcat(StrB,"h,");
	     strcat(StrA,StrB);
	     word=_U2(bufferA[Addr+2])+Addr+3;
	     _CLabel(word);
	     break;
   case 0xD5://djnz ad,d
   case 0xB5://cjne a,ad,d
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0)
	     {
	      sprintf(StrB,"%s,",SFRReg[byte]);
	      strcat(StrA,StrB);
	     }
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,");
	      strcat(StrA,StrB);
	     }
	     word=_U2(bufferA[Addr+2])+Addr+3;
	     _CLabel(word);
	     break;
   case 0x24://add a,#n
   case 0x34://addc a,#n
   case 0x44://orl a,#n
   case 0x54://anl a,#n
   case 0x64://xrl a,#n
   case 0x74://mov a,#n
   case 0x76://mov @r0,#n
   case 0x77://mov @r1,#n
   case 0x78://mov r0,#n
   case 0x79://mov r1,#n
   case 0x7A://mov r2,#n
   case 0x7B://mov r3,#n
   case 0x7C://mov r4,#n
   case 0x7D://mov r5,#n
   case 0x7E://mov r6,#n
   case 0x7F://mov r7,#n
   case 0x94://subb a,#n
	     HEX1StrB(bufferA[Addr+1]);
	     strcat(StrB,"h");
	     strcat(StrA,StrB);
	     break;
   case 0x43://orl ad,#n
   case 0x53://anl ad,#n
   case 0x63://xrl ad,#n
   case 0x75://mov ad,#n
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0)
	     {
	      sprintf(StrB,"%s,#",SFRReg[byte]);
	      strcat(StrA,StrB);
	     }
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,#");
	      strcat(StrA,StrB);
	     }
	     HEX1StrB(bufferA[Addr+2]);
	     strcat(StrB,"h");
	     strcat(StrA,StrB);
	     break;
   case 0x05://inc ad
   case 0x15://dec ad
   case 0x25://add a,ad
   case 0x35://addc a,ad
   case 0x45://orl a,ad
   case 0x55://anl a,ad
   case 0x65://xrl a,ad
   case 0x95://subb a,ad
   case 0xA6://mov @r0,ad
   case 0xA7://mov @r1,ad
   case 0xA8://mov r0,ad
   case 0xA9://mov r1,ad
   case 0xAA://mov r2,ad
   case 0xAB://mov r3,ad
   case 0xAC://mov r4,ad
   case 0xAD://mov r5,ad
   case 0xAE://mov r6,ad
   case 0xAF://mov r7,ad
   case 0xC0://push ad
   case 0xC5://xch a,ad
   case 0xD0://pop ad
   case 0xE5://mov a,ad
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x42://orl ad,a
   case 0x52://xrl ad,a
   case 0x62://xrl ad,a
   case 0xF5://mov ad,a
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,A",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,A");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x72://orl c,bit
   case 0x82://anl c,bit
   case 0xA0://orl c,/bit
   case 0xA2://mov c,bit
   case 0xB0://anl c,/bit
   case 0xB2://cpl bit
   case 0xC2://clr bit
   case 0xD2://setb bit
	     byte=bufferA[Addr+1];
	     if (SFRBit[byte][0]!=0) sprintf(StrB,"%s",SFRBit[byte]);
	     else sprintf(StrB,"%i",byte);
	     strcat(StrA,StrB);
	     break;
   case 0x92://mov bit,C
	     byte=bufferA[Addr+1];
	     if (SFRBit[byte][0]!=0) sprintf(StrB,"%s,C",SFRBit[byte]);
	     else sprintf(StrB,"%i,C",byte);
	     strcat(StrA,StrB);
	     break;
   case 0x85://mov ad,ad
	     byte=bufferA[Addr+2];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,");
	     }
	     strcat(StrA,StrB);
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x86://mov ad,@r0
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,@R0",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,@R0");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x87://mov ad,@r1
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,@R1",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,@R1");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x88://mov ad,R0
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,R0",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,R0");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x89://mov ad,@r1
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,R1",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,R1");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x8A://mov ad,@r2
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,R2",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,R2");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x8B://mov ad,@r3
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,R3",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,R3");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x8C://mov ad,r4
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,R4",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,R4");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x8D://mov ad,r5
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,R5",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,R5");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x8E://mov ad,r6
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,R6",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,R6");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x8F://mov ad,r7
	     byte=bufferA[Addr+1];
	     if (SFRReg[byte][0]!=0) sprintf(StrB,"%s,R7",SFRReg[byte]);
	     else
	     {
	      HEX1StrB(byte);
	      strcat(StrB,"h,R7");
	     }
	     strcat(StrA,StrB);
	     break;
   case 0x90://mov dptr,#nn
	     word=bufferA[Addr+1]*256+bufferA[Addr+2];
	     HEXStrB(word);
	     strcat(StrB,"h");
	     strcat(StrA,StrB);
	     break;
  }
 }
 if (OutStream==0) cprintf("%s",StrA);
 Addr+=CodeLen[temp];
 return Addr;
}

void  MakeBufferB(void)
{
 unsigned long lAddr;

 for (lAddr=0,MaxbufferB=0;lAddr<=MaxbufferA;MaxbufferB++)
 {
  Code=bufferA[lAddr];
  bufferB[MaxbufferB]=CodeLen[Code];
  lAddr+=CodeLen[Code];
 };
 MaxbufferB--;
 Refresh=1;ASMLine=0;UpAddr1=0;
}

void  ClearBuf(unsigned char Sel,unsigned char byte,
	      unsigned int from,unsigned int to)
{
 unsigned long temp;
 switch (Sel)
 {
 case 0:for (temp=from;temp<=to;temp++) bufferA[temp]=0;
	for (temp=from;temp<=to;temp++) bufferB[temp]=1;break;
 case 1:for (temp=from;temp<=to;temp++) bufferA[temp]=byte;break;
 case 2:for (temp=from;temp<=to;temp++) bufferB[temp]=byte;break;
 }
 Refresh=1;
}

void ClearDEFs(void)
{
 for (Word=0;Word<=0xFF;Word++) SFRBit[Word][0]=0;
 for (Word=0;Word<=0xFF;Word++) SFRReg[Word][0]=0;
 for (Word=0;Word<=0xFF;Word++) LabelStr[Word][0]=0;
 Refresh=2;
}