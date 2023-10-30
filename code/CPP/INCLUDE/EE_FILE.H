void SortFile(void)
{
 int unsort,sort,pos,MaxPos;

 MaxPos=FileMax;
 for (sort=FileMax;sort>0;sort--)
 {
  strcpy(StrA,FileList[0]);pos=0;
  for (unsort=0;unsort<MaxPos;unsort++)
   if (stricmp(StrA,FileList[unsort])<0) strcpy(StrA,FileList[pos=unsort]);
  strcpy(FileList[pos],FileList[--MaxPos]);
  strcpy(FileList[sort-1],StrA);
 }
}

void SortDir(void)
{
 int unsort,sort,pos,MaxPos;

 MaxPos=DirMax;
 for (sort=DirMax;sort>1;sort--)
 {
  strcpy(StrA,DirList[1]);pos=1;
  for (unsort=1;unsort<MaxPos;unsort++)
   if (stricmp(StrA,DirList[unsort])<0) strcpy(StrA,DirList[pos=unsort]);
  strcpy(DirList[pos],DirList[--MaxPos]);
  strcpy(DirList[sort-1],StrA);
 }
}

void  GetFiles(unsigned char *Path,unsigned char *Type)
{
 struct ffblk FFind;
 int done;
 unsigned char tpath[MAXPATH];
 unsigned char drivestr[MAXDRIVE];
 unsigned char dirstr[MAXDIR];
 unsigned char filestr[MAXFILE];
 unsigned char extstr[MAXEXT];
 strcpy(tpath,Path);
 fnsplit(tpath,drivestr,dirstr,filestr,extstr);
 fnmerge(tpath,drivestr,dirstr,Type,"");
 done=findfirst(tpath,&FFind,0xff);
 while (!done)
 {
  if ( ((FFind.ff_attrib&0x08)!=0x08)&&((FFind.ff_attrib&0x10)!=0x10) )
  {
   strcpy(FileList[FileMax],FFind.ff_name);
   FileMax++;
  }
  done = findnext(&FFind);
 }
 SortFile();
 fnmerge(tpath,drivestr,dirstr,"*.*","");
 done=findfirst(tpath,&FFind,0xff);
 while (!done)
 {
  if ( (FFind.ff_attrib&0x10) == 0x10 )
  {
   if (stricmp(FFind.ff_name,".")!=0)
   {
    strcpy(DirList[DirMax],FFind.ff_name);
    DirMax++;
   }
  }
  done = findnext(&FFind);
 }
 SortDir();
}

unsigned char  SeekCFGLine(int File,unsigned char Num)
{
 unsigned char temp,pos;
 int error;
 unsigned char buf[2];

 lseek(File,0,SEEK_SET);
 if (Num==0) Num=1;
 temp=0;
 do {
  error=read(File,buf,1);
  if (error<=0) { pos=0;break; }
  if ( (error>0)&&(buf[0]==CR) || (buf[0]==LF) ) continue;//pominiecie LF lub CR
  if ( (error>0) && (buf[0]==Rem) )	      //pominiecie komentazy
  {
   do {
       error=read(File,buf,1);
       if ((buf[0]==CR) || (buf[0]==LF)) break;
   } while (error!=0);//czytaj do CR lub LF
   continue;
  }
  if ( (error>0) && (buf[0]>=32) )
  {
   pos=0;
   do {
       if (error>0) pos++;
       error=read(File,buf,1);
       if ((buf[0]==CR) || (buf[0]==LF)) break;
   } while (error!=0);
   if (pos>0) temp++;
  }
 } while (temp!=Num);
 if (error!=0) lseek(File,-pos-1,SEEK_CUR);
 else lseek(File,-pos,SEEK_CUR);
 return pos;
}


char  LoadCFG(unsigned char *Path)
{
 unsigned char a,b,leng;
 int FileCFG,error;

 CFGLineNum=0;
 FileCFG=open(Path,O_BINARY | O_RDONLY,S_IREAD);
 if (FileCFG<0) WriteError(0,0,"");
 else {
       CFGLineNum=1;
       do {
	   leng=SeekCFGLine(FileCFG,CFGLineNum);
	   CFGLineNum++;
	   if (leng==0) break;
	   error=read(FileCFG,CFGLineStr,leng);
	   CFGLineStr[leng]=0;a=0;b=0;
	   do {
	       if (CFGLineStr[a]!=Sep)
	       {
		StrA[b]=CFGLineStr[a];
		a++;b++;
	       }
	       StrA[b]=0;
	   } while ((CFGLineStr[a]!=Sep) && (a!=leng));
	   if (a==leng) WriteError(1,1,CFGLineStr);
	   a++;b=0;
	   for (b=0;a!=leng;b++,a++)
	   {
	    StrB[b]=CFGLineStr[a];
	    StrB[b+1]=0;
	   }
	   a=0;b=0;CFGParamOK=0;
	   for (a=0;a!=MaxCFGParam;a++)
	   {
	    if (stricmp(StrA,CFGParam[a])==0) switch (a)
	    {
	     case 0: CFGParamOK=1;
		     strcpy(StrA,"");strcat(StrA,StrB);
		     COMNum=strtoul(StrA,NULL,10);
		     if (COMNum>4) WriteError(2,1,CFGLineStr);
		     if (COMNum<1) WriteError(2,1,CFGLineStr);
		     break;
	     case 1: CFGParamOK=1;
		     strcpy(StrA,"0x");strcat(StrA,StrB);
		     COMAddr=strtoul(StrA,NULL,16);
		     break;
	     case 2: CFGParamOK=1;
		     strcpy(StrA,"0x");strcat(StrA,StrB);
		     COMIRQ=strtoul(StrA,NULL,16);
		     if (COMIRQ>15) WriteError(2,1,CFGLineStr);
		     break;
	     case 3: CFGParamOK=1;
		     strcpy(StrA,"");strcat(StrA,StrB);
		     Speed_1=strtoul(StrA,NULL,10);
		     if (Speed_1>57600) WriteError(2,1,CFGLineStr);
		     if (Speed_1<300) WriteError(2,1,CFGLineStr);
		     break;
	     case 4: CFGParamOK=1;
		     strcpy(StrA,"");strcat(StrA,StrB);
		     Speed_2=strtoul(StrA,NULL,10);
		     if (Speed_2>57600) WriteError(2,1,CFGLineStr);
		     if (Speed_2<300) WriteError(2,1,CFGLineStr);
		     break;
	     case 5: CFGParamOK=1;
		     strcpy(StrA,"");strcat(StrA,StrB);
		     ScrollTime=strtoul(StrA,NULL,10);
		     if (ScrollTime>150) ScrollTime=150;
		     break;
	     case 6: CFGParamOK=1;
		     strcpy(StrA,"");strcat(StrA,StrB);
		     MaxSize=strtoul(StrA,NULL,10);
		     if (MaxSize>64) WriteError(2,1,CFGLineStr);
		     if (MaxSize<8) WriteError(2,1,CFGLineStr);
		     break;
	     case 7: CFGParamOK=1;
		     strcpy(StrA,"");strcat(StrA,StrB);
		     MaxTimeOut=strtoul(StrA,NULL,10)/55;
		     break;
	     case 8: CFGParamOK=1;
		     strcpy(StrA,"");strcat(StrA,StrB);
		     MaxCRCchecks=strtoul(StrA,NULL,10);
		     break;
	     case 9: CFGParamOK=1;
		     strcpy(StrA,"");strcat(StrA,StrB);
		     ViewType=strtoul(StrA,NULL,10);
		     if ((ViewType>4)||(ViewType==0)) WriteError(2,1,CFGLineStr);
		     break;
	     case 10:CFGParamOK=1;
		     strcpy(StrA,"");strcat(StrA,StrB);
		     Long=strtoul(StrA,NULL,10);
		     if (Long<2048) Long=2048;
		     if (Long>65536) Long=65536;
		     if ((Long%16)>0) Long+=16-(Long%16);
		     Long--;MaxbufferA=Long;
		     break;
	    }
	    if (CFGParamOK==1) break;
	   }
	   if (CFGParamOK==0) WriteError(1,1,CFGLineStr);
	  } while (error!=0);
      }
      close(FileCFG);
      return CFGLineNum;
}

char  SaveCFG(unsigned char *Path)
{
 unsigned char a,b,leng;
 int FileCFG,error,w;
 long fpos,fsize;
 struct stat FStat;

 CFGLineNum=1;
 do {
  FileCFG=open(Path,O_BINARY|O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
  if (FileCFG<0) return 1;
  fstat(FileCFG,&FStat);fsize=FStat.st_size;w=fsize/64;fpos=0;
  for (a=0;a<w;a++)
  {
   error=read(FileCFG,TempBuf,64);
   for (b=0;b<64;b++,fpos++) bufferB[fpos]=TempBuf[b];
  }
  w=fsize%64;
  if ((w>0)&&(w<64))
  {
   error=read(FileCFG,TempBuf,w);
   for (b=0;b<w;b++,fpos++) bufferB[fpos]=TempBuf[b];
  }
  leng=SeekCFGLine(FileCFG,CFGLineNum);CFGLineNum++;
  if (leng==0) break;
  error=read(FileCFG,CFGLineStr,leng);
  CFGLineStr[leng]=0;a=0;b=0;
  do {
    if (CFGLineStr[a]!=Sep){ StrA[b]=CFGLineStr[a];a++;b++; }
    StrA[b]=0;
  } while ((CFGLineStr[a]!=Sep) && (a!=leng));
  a++;
  for (b=0;a!=leng;b++,a++)
  {
   StrB[b]=CFGLineStr[a];
   StrB[b+1]=0;
  }
  CFGParamOK=0;
  for (a=0;a<MaxCFGParam;a++)
  {
   if (stricmp(StrA,CFGParam[a])==0)
   {
    lseek(FileCFG,-b,SEEK_CUR);fpos=tell(FileCFG);
    switch(a)
    {
     case 0: CFGParamOK=1;ltoa(COMNum,StrA,10);
	     write(FileCFG,StrA,strlen(StrA));break;
     case 1: CFGParamOK=1;HEXStrB(COMAddr);
	     write(FileCFG,StrB,strlen(StrB));break;
     case 2: CFGParamOK=1;ltoa(COMIRQ,StrA,10);
	     write(FileCFG,StrA,strlen(StrA));break;
     case 3: CFGParamOK=1;ltoa(Speed_1,StrA,10);
	     write(FileCFG,StrA,strlen(StrA));break;
     case 4: CFGParamOK=1;ltoa(Speed_2,StrA,10);
	     write(FileCFG,StrA,strlen(StrA));break;
     case 5: CFGParamOK=1;ltoa(ScrollTime,StrA,10);
	     write(FileCFG,StrA,strlen(StrA));break;
     case 6: CFGParamOK=1;ltoa(MaxSize,StrA,10);
	     write(FileCFG,StrA,strlen(StrA));break;
     case 7: CFGParamOK=1;ltoa(MaxTimeOut*55,StrA,10);
	     write(FileCFG,StrA,strlen(StrA));break;
     case 8: CFGParamOK=1;ltoa(MaxCRCchecks,StrA,10);
	     write(FileCFG,StrA,strlen(StrA));break;
     case 9: CFGParamOK=1;ltoa(ViewType,StrA,10);
	     write(FileCFG,StrA,strlen(StrA));break;
     case 10:CFGParamOK=1;ltoa(MaxbufferA+1L,StrA,10);
	     write(FileCFG,StrA,strlen(StrA));break;
    }
    chsize(FileCFG,tell(FileCFG));
    for (;bufferB[fpos]!=CR;fpos++){};
    for (Long=fpos;Long<fsize;Long++)
    {
     TempBuf[0]=bufferB[Long];
     write(FileCFG,TempBuf,1);
    }
   }
   if (CFGParamOK==1) break;
  }
  close(FileCFG);
 } while (error!=0);
 return 0;
}

char  LoadDEF(unsigned char *Path)
{
 unsigned char type;
 unsigned int addr,pos,lab;
 int FileDEF;
 int error;
 unsigned char buf[2];
 lab=0;
 FileDEF=open(Path,O_BINARY | O_RDONLY,S_IREAD);
 if (FileDEF<0) return -3;
 lseek(FileDEF,0,SEEK_SET);

 do {
 error=read(FileDEF,buf,1);
 if ( (error>0)&&(buf[0]==CR) || (buf[0]==LF) ) continue;//pominiecie LF lub CR

 if ( (error>0) && (buf[0]==Rem) )	      //pominiecie komentazy
 {
  do {
      error=read(FileDEF,buf,1);
      if ((buf[0]==CR) || (buf[0]==LF)) break;
  } while (error>0);//czytaj do CR lub LF
  continue;
 }

 if ( (error>0) && (buf[0]>32) )	       //odczyt linii parametru
 {
  type=0;buf[0]=toupper(buf[0]);
  if (buf[0]=='C') type=1;
  if (buf[0]=='D') type=2;
  if (buf[0]=='B') type=3;
  if (type==0) { close(FileDEF);return -4; }//nieznany znak
  do {				//pomin spacje i tab
      error=read(FileDEF,buf,1);
      if ( (buf[0]==32) || (buf[0]==9) ) continue;
      if (buf[0]<32) { close(FileDEF);return -4;} //blad
      else break;
  } while (error>0);
  addr=0;				//zaladuj wartosc argumentu
  do {
      if (buf[0]>32)
      {
       StrB[addr]=buf[0];
       addr++;
       StrB[addr]=0;
      }
      if ( (buf[0]==32) || (buf[0]==9) ) break;//spacja lub tab to koniec
      if (buf[0]<32) { close(FileDEF);return -4; }//jesli inny znak to blad
      error=read(FileDEF,buf,1);
  } while (error>0);
  strcpy(StrA,"0x");
  strcat(StrA,StrB);
  addr=strtoul(StrA,NULL,16);
  if ((type==2)&&(addr>0xFF)) { close(FileDEF);return 5; }
  if ((type==3)&&(addr>0xFF)) { close(FileDEF);return 6; }

  do {				//pomin spacje i tab
      error=read(FileDEF,buf,1);
      if ( (buf[0]==32) || (buf[0]==9) ) continue;
      if (buf[0]<32) { close(FileDEF);return -4; } //blad
      else break;
  } while (error>0);

  if (type==1)			//skasuj powtorzone etykiety adresowe
  for (lab=0;lab<=0xFF;lab++) if (LabelAdr[lab]==addr) LabelStr[lab][0]=0;
  if (buf[0]=='*')
  {
   if (type==2) SFRReg[addr][0]=0;//skasuj etykiete SFR
   if (type==3) SFRBit[addr][0]=0;     //skasuj etykiete bit
   do {
       error=read(FileDEF,buf,1);
       if ((buf[0]==CR) || (buf[0]==LF)) break;
   } while (error>0);			//czytaj do CR lub LF
   continue;
  }
					//znajdz pierwsza wolna etykiete
  for (lab=0;lab<=0xFF;lab++) if (LabelStr[lab][0]==0) break;
  pos=0;
  do {						//zaladuj nazwe argumentu
      if (buf[0]>32)
      {
       switch (type) {
		      case 1:LabelStr[lab][pos]=buf[0];
			     if (pos<15) pos++;
			     LabelStr[lab][pos]=0;
			     break;
		      case 2:SFRReg[addr][pos]=buf[0];
			     if (pos<10) pos++;
			     SFRReg[addr][pos]=0;
			     break;
		      case 3:SFRBit[addr][pos]=buf[0];
			     if (pos<10) pos++;
			     SFRBit[addr][pos]=0;
			     break;
		     }
      }
      else
      {
       do {
	   error=read(FileDEF,buf,1);
	   if ((buf[0]==CR) || (buf[0]==LF)) break;
       } while (error>0);//czytaj do CR lub LF
       break;
      }
      error=read(FileDEF,buf,1);
  } while (error>0);
  if (type==1) LabelAdr[lab]=addr;
 }
 } while (error>0);
 close(FileDEF);
 return 0;
}

unsigned  char LoadDAT(unsigned char *Path)
{
 int FileDAT;
 int error;
 unsigned char A,B;

 FileDAT=open(Path,O_BINARY | O_RDONLY,S_IREAD);
 if (FileDAT<0) WriteError(3,3,Path);
 error=read(FileDAT,PL_1,144); if (error<144) WriteError(4,4,Path);
 error=read(FileDAT,PL_2,144); if (error<144) WriteError(4,4,Path);
 error=read(FileDAT,Kolory,15);if (error<15) WriteError(4,4,Path);
 error=read(FileDAT,Ramki,33); if (error<33) WriteError(4,4,Path);
 Code=0;
 do {
     error=read(FileDAT,TempBuf,3);
     if (error==3)
     {
      A=TempBuf[0];
      B=TempBuf[2];
      CodeLen[A]=TempBuf[1];
      if (A!=Code) WriteError(4,4,Path);
      if (B>0)
      {
       error=read(FileDAT,TempBuf,B);
       TempBuf[B]=0;
       if (error==B) strcpy(MnemStr[A],TempBuf);
       else WriteError(4,4,Path);
      }
      Code++;
     }
     else if (error!=0) WriteError(4,4,Path);
    } while (error!=0);
 close(FileDAT);
 return 0;
}

char  LoadHEX(unsigned char *Path)
{
 int FileHEX;
 int error;
 unsigned char DataLen,RecType,crc,pos,q;
 unsigned int addr;

 FileHEX=open(Path,O_BINARY | O_RDONLY,S_IREAD);
 if (FileHEX<0)
 {
  _DialogW(_Uwaga,ErrorStr[4],"");
  MakeBufferB();Refresh=1;return 1;
 }
 lseek(FileHEX,0,SEEK_SET);

 do {
 error=read(FileHEX,TempBuf,1);
 if ( (error>0) && (TempBuf[0]==CR) || (TempBuf[0]==LF) ) continue;
 if ( (error>0) && (TempBuf[0]==':') )	       //odczyt linii
 {
  error=read(FileHEX,TempBuf,2);
  if (error!=2)
  {
   _DialogW(_Uwaga,ErrorStr[4],"");
   close(FileHEX);MakeBufferB();Refresh=1;return 2;
  }
  TempBuf[2]=0;
  strcpy(StrA,"0x");strcat(StrA,TempBuf);DataLen=strtoul(StrA,NULL,16);
  Byte=DataLen;
  error=read(FileHEX,TempBuf,2);
  if (error!=2)
  {
   _DialogW(_Uwaga,ErrorStr[4],"");
   close(FileHEX);MakeBufferB();Refresh=1;return 2;
  }
  TempBuf[2]=0;
  strcpy(StrA,"0x");strcat(StrA,TempBuf);q=strtoul(StrA,NULL,16);
  addr=256*q;Byte+=q;
  error=read(FileHEX,TempBuf,2);
  if (error!=2)
  {
   _DialogW(_Uwaga,ErrorStr[4],"");
   close(FileHEX);MakeBufferB();Refresh=1;return 2;
  }
  TempBuf[2]=0;
  strcpy(StrA,"0x");strcat(StrA,TempBuf);q=strtoul(StrA,NULL,16);
  addr+=q;Byte+=q;
  error=read(FileHEX,TempBuf,2);
  if (error!=2)
  {
   _DialogW(_Uwaga,ErrorStr[4],"");
   close(FileHEX);MakeBufferB();Refresh=1;return 2;
  }
  TempBuf[2]=0;
  strcpy(StrA,"0x");strcat(StrA,TempBuf);RecType=strtoul(StrA,NULL,16);
  Byte+=RecType;
  if (RecType==0)
  {
   for (pos=0;pos<DataLen;pos++)
   {
    error=read(FileHEX,TempBuf,2);
    if (error!=2)
    {
     _DialogW(_Uwaga,ErrorStr[4],"");
     close(FileHEX);MakeBufferB();Refresh=1;return 2;
    }
    TempBuf[2]=0;strcpy(StrA,"0x");strcat(StrA,TempBuf);
    q=strtoul(StrA,NULL,16);
    StrB[pos]=q;Byte+=q;
   }
  }
  error=read(FileHEX,TempBuf,2);
  if (error!=2)
  {
   _DialogW(_Uwaga,ErrorStr[4],"");
   close(FileHEX);MakeBufferB();Refresh=1;return 2;
  }
  TempBuf[2]=0;strcpy(StrA,"0x");strcat(StrA,TempBuf);
  crc=strtoul(StrA,NULL,16);
  Byte+=crc;
 }
 if ((RecType==0) && (Byte==0))
  for (Long=addr,pos=0;pos<DataLen;pos++,Long++)
  {
   if (Long>MaxbufferA) break;
   bufferA[Long]=StrB[pos];
  }
 if ((RecType==1) && (Byte==0)) break;
 if (Byte!=0) { close(FileHEX);MakeBufferB();Refresh=1;CurFile=1;return 1; }
 } while (error>0);
 close(FileHEX);
 MakeBufferB();Refresh=1;CurFile=1;
 return 0;
}

char  SaveHEX(unsigned char *Path)
{
 int FileHEX;
 int error,q,w;
 unsigned char l,sum;
 unsigned long addr;

 FileHEX=open(Path,O_BINARY | O_CREAT | O_TRUNC | O_RDWR,S_IREAD | S_IWRITE);
 TempBuf[0]=':';
 TempBuf[7]=hextab[0];
 TempBuf[8]=hextab[0];
 if (FileHEX<0){ _DialogW(_Uwaga,ErrorStr[3],"");return 1; }
 Long=MaxbufferA;Long++;
 w=Long/16;addr=0;
 for (q=0;q<w;q++)
 {
  sum=0;
  Byte=(16&0xF0)>>4; TempBuf[1]=hextab[Byte];
  Byte=(16&0x0F);    TempBuf[2]=hextab[Byte];sum+=16;
  Byte=(addr&0xF000)>>12;TempBuf[3]=hextab[Byte];sum+=(addr&0xFF00)>>8;
  Byte=(addr&0x0F00)>>8; TempBuf[4]=hextab[Byte];sum+=(addr&0x00FF);
  Byte=(addr&0x00F0)>>4; TempBuf[5]=hextab[Byte];
  Byte=(addr&0x000F);    TempBuf[6]=hextab[Byte];
  for (l=9;((l-9)/2)<16;l++,addr++)
  {
   Code=bufferA[addr];sum+=Code;
   Byte=(Code&0xF0)>>4; TempBuf[l]=hextab[Byte];l++;
   Byte=(Code&0x0F);    TempBuf[l]=hextab[Byte];
  }
  sum=256-sum;
  Byte=(sum&0xF0)>>4;TempBuf[l]=hextab[Byte];l++;
  Byte=(sum&0x0F);   TempBuf[l]=hextab[Byte];l++;
  TempBuf[l]=CR;l++;
  TempBuf[l]=LF;l++;
  error=write(FileHEX,TempBuf,l);
  if (error<l)
  {
   _DialogW(_Uwaga,ErrorStr[3],ErrorStr[7]);close(FileHEX);return 2;
  }
 }
 w=Long%16;sum=0;
 if ((w>0)&&(w<16))
 {
  Byte=(w&0xF0)>>4; TempBuf[1]=hextab[Byte];
  Byte=(w&0x0F);    TempBuf[2]=hextab[Byte];sum+=w;
  Byte=(addr&0xF000)>>12;TempBuf[3]=hextab[Byte];sum+=(addr&0xFF00)>>8;
  Byte=(addr&0x0F00)>>8; TempBuf[4]=hextab[Byte];sum+=(addr&0x00FF);
  Byte=(addr&0x00F0)>>4; TempBuf[5]=hextab[Byte];
  Byte=(addr&0x000F);    TempBuf[6]=hextab[Byte];
  for (l=9;((l-9)/2)<w;l++,addr++)
  {
   Code=bufferA[addr];sum+=Code;
   Byte=(Code&0xF0)>>4; TempBuf[l]=hextab[Byte];l++;
   Byte=(Code&0x0F);    TempBuf[l]=hextab[Byte];
  }
  sum=256-sum;
  Byte=(sum&0xF0)>>4;TempBuf[l]=hextab[Byte];l++;
  Byte=(sum&0x0F);   TempBuf[l]=hextab[Byte];l++;
  TempBuf[l]=CR;l++;
  TempBuf[l]=LF;l++;
  error=write(FileHEX,TempBuf,l);
  if (error<l)
  {
   _DialogW(_Uwaga,ErrorStr[3],ErrorStr[7]);close(FileHEX);return 2;
  }
 }
 error=write(FileHEX,":00000001FF",11);
 if (error<11)
 {
  _DialogW(_Uwaga,ErrorStr[3],ErrorStr[7]);close(FileHEX);return 2;
 }
 close(FileHEX);
 CurFile=1;
 return 0;
}

char  SaveASM(unsigned char *Path)
{
 int FileASM,error;
 unsigned char l;
 unsigned long addr;

 OutStream=1;
 FileASM=open(Path,O_BINARY | O_CREAT | O_TRUNC | O_RDWR,S_IREAD | S_IWRITE);
 if (FileASM<0){ _DialogW(_Uwaga,ErrorStr[3],"");return 1; }
 for (addr=0;addr<=MaxbufferA;)
 {
  if (!LabelStrB(addr))
  {
   HEXStrB(addr);
   sprintf(StrA,"ADDR_%sh",StrB);
   strcpy(StrB,StrA);
  }
  strcat(StrB,":");
  strcpy(CFGLineStr,StrB);
  addr=printDeASM(addr);
  sprintf(TempBuf,"%-20s%s\r\n",CFGLineStr,StrA);
  error=write(FileASM,TempBuf,strlen(TempBuf));
  if (kbhit())
  {
   Key=getch();
   if (Key==0) { SKey=1;Key=getch();}
   else SKey=0;
   if ((!SKey)&&(Key==ESC)) {close(FileASM);return 0;}
  }
  if (error<strlen(TempBuf))
  {
   _DialogW(_Uwaga,ErrorStr[3],ErrorStr[7]);close(FileASM);return 2;
  }
 }
 close(FileASM);
 CurFile=3;
 return 0;
}


char  LoadBIN(unsigned char *Path,unsigned int Addr)
{
 int FileBIN;
 int error,q,w;
 unsigned char l;
 unsigned long lAddr,temp;
 struct stat FStat;

 FileBIN=open(Path,O_BINARY | O_RDONLY,S_IREAD);
 if (FileBIN<0)
 {
  _DialogW(_Uwaga,ErrorStr[4],"");MakeBufferB();Refresh=1;return 1;
 }
 fstat(FileBIN,&FStat);temp=FStat.st_size;lAddr=Addr;
 if ((lAddr+temp)>MaxbufferA) temp=MaxbufferA-lAddr;
 if (temp<=MaxbufferA)
 {
  w=temp/64;
  for (q=0;q<w;q++)
  {
   error=read(FileBIN,TempBuf,64);
   if (error<64)
   {
    _DialogW(_Uwaga,ErrorStr[4],"");
    close(FileBIN);MakeBufferB();Refresh=1;return 2;
   }
   for (l=0;l<64;l++,lAddr++) bufferA[lAddr]=TempBuf[l];
  }
  w=temp%64;
  if ((w>0)&&(w<64))
  {
   error=read(FileBIN,TempBuf,w);
   if (error<w)
   {
    _DialogW(_Uwaga,ErrorStr[4],"");
    close(FileBIN);MakeBufferB();Refresh=1;return 2;
   }
   for (l=0;l<w;l++,lAddr++) bufferA[lAddr]=TempBuf[l];
  }
 }
 close(FileBIN);
 MakeBufferB();Refresh=1;CurFile=2;
 return 0;
}

char  SaveBIN(unsigned char *Path)
{
 int FileBIN;
 int error,q,w;
 unsigned char l;
 unsigned long addr;

 FileBIN=open(Path,O_BINARY | O_CREAT | O_TRUNC | O_RDWR,S_IREAD | S_IWRITE);
 if (FileBIN<0) { _DialogW(_Uwaga,ErrorStr[3],"");return 1; }
 Long=MaxbufferA;Long++;w=Long/64;addr=0;
 for (q=0;q<w;q++)
 {
  for (l=0;l<64;l++,addr++) TempBuf[l]=bufferA[addr];
  error=write(FileBIN,TempBuf,64);
  if (error<64)
  {
   _DialogW(_Uwaga,ErrorStr[3],ErrorStr[7]);close(FileBIN);return 2;
  }
 }
 w=Long%64;
 if ((w>0)&&(w<64))
 {
  for (l=0;l<w;l++,addr++) TempBuf[l]=bufferA[addr];
  error=write(FileBIN,TempBuf,w);
  if (error<w)
  {
   _DialogW(_Uwaga,ErrorStr[3],ErrorStr[7]);close(FileBIN);return 2;
  }
 }
 close(FileBIN);
 CurFile=2;
 return 0;
}

char  LoadHLP(unsigned char *Path,unsigned char Topic)
{
 int FileHLP,error;
 unsigned int lAddr;
 unsigned char num;

 lAddr=0;num=0;
 FileHLP=open(Path,O_BINARY | O_RDONLY,S_IREAD);
 if (FileHLP<0) { _DialogW(_Uwaga,ErrorStr[2],"");return 1; }
 do {
  error=read(FileHLP,TempBuf,1);
  if (TempBuf[0]=='^') { num++;continue; }
  if (num!=Topic) continue;
  if (TempBuf[0]=='@')
  {
   for (Byte=0;Byte<=75;Byte++)
   {
    bufferB[lAddr]=32;
    bufferB[lAddr+1]=0x07;
    lAddr+=2;
   }
   for (Byte=0;Byte<=34;Byte++)
   {
    bufferB[lAddr]=32;
    bufferB[lAddr+1]=0x07;
    lAddr+=2;
   }
   for (Byte=0;Byte<=3;Byte++)
   {
    bufferB[lAddr]='-';
    bufferB[lAddr+1]=0x07;
    lAddr+=2;
   }
   for (Byte=0;Byte<=38;Byte++)
   {
    bufferB[lAddr]=32;
    bufferB[lAddr+1]=0x07;
    lAddr+=2;
   }
   continue;
  }
  if (TempBuf[0]>=32)
  {
   bufferB[lAddr]=TempBuf[0];
   bufferB[lAddr+1]=0x07;
   lAddr+=2;
   continue;
  }
  if ((TempBuf[0]==LF)||(TempBuf[0]==CR))
  {
   for (;lAddr%152!=0;)
   {
    bufferB[lAddr]=32;
    bufferB[lAddr+1]=0x07;
    lAddr+=2;
   }
  }
 } while (error>0);
 Word=lAddr/152;
 if (lAddr<4096)
 for (;lAddr<=4096;lAddr+=2)
 {
  bufferB[lAddr]=32;
  bufferB[lAddr+1]=0x07;
 }
 close(FileHLP);
 return 0;
}
