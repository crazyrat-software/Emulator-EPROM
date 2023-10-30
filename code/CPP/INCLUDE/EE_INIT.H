void Start(void)
{
 unsigned char y;
 char err;
 InitMouse();
 ultoa(coreleft()/1024,StrA,10);
 printf("\n\rPamiec RAM: %s kB\n",StrA);
 if (((bufferB=(unsigned char far *)farmalloc(0x10000) ) == NULL)||
     ((bufferA=(unsigned char far *)farmalloc(0x10000) ) == NULL))
 {
  printf("\n\r%s",ErrorStr[10]);exit(0);
 }
 strcpy(PathStr,strupr(_argv[0]));
 fnsplit(PathStr,DriveStr,DirStr,FileStr,ExtStr);
 fnmerge(PathDAT,DriveStr,DirStr,FileStr,"DAT");
 fnmerge(PathDEF,DriveStr,DirStr,FileStr,"DEF");
 fnmerge(PathCFG,DriveStr,DirStr,FileStr,"CFG");
 fnmerge(PathHLP,DriveStr,DirStr,FileStr,"HLP");
// fnmerge(CurPath,DriveStr,DirStr,"","");
 getcwd(CurPath,MAXPATH);
 if (CurPath[strlen(CurPath)-1]!='\\') strcat(CurPath,"\\");

 delay(300);
 printf("\n\rWczytywanie deasemblera 8051...");y=wherey();
 LoadDAT(PathDAT);
 gotoxy(32,y);printf(_OK);
 printf("\n\rWczytywanie etykiet...");y=wherey();
 err=LoadDEF(PathDEF);
 if (err<0) WriteError(-err,-err,PathDEF);
 if (err>0) WriteError(err,0,PathDEF);
 if (err==0) { gotoxy(23,y);printf(_OK); }
 printf("\n\rWczytywanie ustawien...");y=wherey();
 LoadCFG(PathCFG);gotoxy(24,y);printf(_OK);
 //inicjacja zmiennych
 ClearBuf(0,0,0,0xFFFF);
 MakeBufferB();
 for (Byte=0;Byte!=MaxMenuNumX;Byte++) MenuYPos[Byte]=1;
 MenuNumX=1;
 MainWindowPos=1;MainWindowxPos=1;
 MainWindowSel=1;MainWindowxSel=1;
 Key=0;SKey=0;OK=0;

 //inicjacja wektorow przerwan
 OldIRQProc=getvect(COMIRQ+0x08);
 OldIRQMask=InitPIC(COMIRQ+0x08,255);
 OldIRQNum=COMIRQ;
 SetIRQProc(COMIRQ);
 Old0x1CProc=getvect(0x1C);setvect(0x1C,Timer);TimerFlag=1;
 Old0x23Proc=getvect(0x23);setvect(0x23,Dumb);
 Old0x1BProc=getvect(0x1B);setvect(0x1B,Dumb);
 InitUART(COMAddr,9600);
}
