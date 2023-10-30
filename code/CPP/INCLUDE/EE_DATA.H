//wymagane pliki
unsigned char PathDAT[MAXPATH]="EEPROM.DAT";
unsigned char PathDEF[MAXPATH]="EEPROM.DEF";
unsigned char PathCFG[MAXPATH]="EEPROM.CFG";
unsigned char PathHLP[MAXPATH]="EEPROM.HLP";

//zmienne dotyczace plikow
unsigned char CurFile=0;		     //typ otwartego pliku
unsigned char CurPath[MAXPATH]="C:\\";	     //biezacy katalog
unsigned char CurMask[13]="*.*";	     //maska plikow
unsigned char PathStr[MAXPATH];
unsigned char DriveStr[MAXDRIVE];
unsigned char DirStr[MAXDIR];
unsigned char FileStr[MAXFILE];
unsigned char ExtStr[MAXEXT];
unsigned char FileName[13];
unsigned char FileList[512][13];	     //lista plikow
unsigned char DirList[512][13];              //lista katalogow
unsigned int FileMax,DirMax;

//zmienne ustawiane w CFG
unsigned char COMNum=2;		   	     //numer uzywanego portu COM
unsigned int COMAddr=0x02F8;		     //adres uzywanego portu COM
unsigned char COMIRQ=3;			     //numer przerwania IRQ
unsigned int Speed_1=9600;		     //max predkosc transmisji
unsigned int Speed_2=2400;		     //min predkosc transmisji
unsigned char MaxSize=32;		     //max rozmiar pakietu
unsigned int MaxbufferA=65535;               //max rozmiar bufora
unsigned char MaxCRCchecks=5;	             //max. ilosc powtorzen przy bledzie CRC
unsigned int MaxTimeOut=110; 	             //max. czas oczekiwania
unsigned char ScrollTime=20;		     //opoznienie rozwijania menu

//zmienne dotyczace bufora i widoku
unsigned char far *bufferA;         	     //bufor danych 1
unsigned char far *bufferB;         	     //bufor danych 2
unsigned int MaxbufferB;                     //max ilosc instrukcji
unsigned int UpAddr1=0,UpAddr2=0,
	     UpAddr3=0,UpAddr4=0;            //adresy stron wyswietlania
unsigned char CodeLen[0x100];                //dlugosci kodow rozkazow
unsigned char MnemStr[0x100][20];	     //mnemoniki
unsigned int Address=0;			     //adres edytowanej komorki
unsigned int ASMLine=0;			     //numer instrukcji
unsigned long CodeLine=0;		     //numer linii kodu
unsigned char Code;			     //kod rozkazu
unsigned char OutStream=0;		     //strumien deasemblera
unsigned long AboutTime=1;		     //okres demo
unsigned char ViewType=1;		     //zadany widok
unsigned char CurView=0;		     //aktualny widok
unsigned char Refresh=1;		     //znacznik odswiezenia widoku
unsigned char MenuNumX,MenuNumY;	     //aktualna pozycja menu X i Y
unsigned char MenuYPos[6];		     //tablica pozycji meny Y
unsigned char MainWindowPos,MainWindowxPos;  //pozycja i zaznaczenie main
unsigned char MainWindowSel,MainWindowxSel;
unsigned char Obraz[4096];		     //pamiec obrazu
unsigned char PL_1[9][16];		     //polskie znaki
unsigned char PL_2[9][16];		     //polskie znaki
const unsigned char ZBit[2][16]={
{0x00,0x00,0x00,0x3C,0x42,0x81,0x81,0x81,0x81,0x81,0x81,0x42,0x3C,0x00,0x00,0x00},
{0x00,0x00,0x00,0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C,0x00,0x00,0x00}};//znaki bitow
unsigned char Ramki[3][11];		     //obramowania
unsigned char NewKolory[15];		     //Kolory do zapisu
unsigned char Kolory[15];		     //Kolory:
//0 - znacznik zmiany kolorow
//1 - menu glowne
//2 - menu rozwijane
//3 - zaznaczenie menu
//4 - pasek statusu nieaktywny
//5 - tekst w oknie glownym
//6 - ramka w oknie glownym
//7 - zaznaczenie w oknie glownym i dialogowym
//8 - adres w oknie glownym
//9 - etykieta w oknie glownym
//10- tekst w oknie dialogowym
//11- ramka w oknie dialogowym
//12- przycisk nieaktywny
//13- przycisk aktywny
//14- pasek statusu aktywny

//zmienne dotyczace configuracji
unsigned char CFGLineStr[40];
unsigned char CFGLineNum=0;
unsigned char CFGParamOK=0;
const unsigned char MaxCFGParam=11;	     //max liczba znanych parametrow
const unsigned char CFGParam[11][15]=	     //lista parametrow
 {"COMNUM",
  "COMADDR",
  "COMIRQ",
  "SPEED_1",
  "SPEED_2",
  "SCROLLTIME",
  "PACKETSIZE",
  "MAXTIMEOUT",
  "REPEAT",
  "VIEW",
  "MEMSIZE"};
const unsigned char Sep='=';		     //znak rozdzielajacy
const unsigned char Rem=';';		     //znak poczatku komentarza
unsigned char far SFRBit[256][11];	     //predefiniowane nazwy bitow
unsigned char far SFRReg[256][11];	     //predefiniowane nazwy rejestrow SFR
unsigned char far LabelStr[256][16];	     //predefiniowane etykiety adresowe
unsigned int far LabelAdr[256];		     //predefiniowane adresy etykiet

//zmienne dotyczace RS-232
void interrupt (*Old0x1CProc)(...);	     //stara procedura int 0x1C
void interrupt (*OldIRQProc)(...);  	     //stara procedura IRQ
void interrupt (*Old0x23Proc)(...);  	     //stara procedura Ctrl+C
void interrupt (*Old0x1BProc)(...);  	     //stara procedura Ctrl+Break
unsigned char OldIRQNum=3;		     //stary numer IRQ
unsigned char OldIRQMask;		     //stara maska przerwania IRQ
unsigned char UARTType;			     //typ ukladu transmisji szeregowej
unsigned char _DLL,_DLH,_IER,_IID,_LCR,_MCR; //stare ustawienia UART
unsigned char COMstatus;		     //rejestr stanu linii
unsigned char MaxCOMNum;		     //max numer portu COM
volatile unsigned int TimeOut;		     //czas oczekiwania na port szeregowy
unsigned char TimerFlag=0;		     //czy jest procedura timer
unsigned char IRQFlag=0;		     //czy jest procedura IRQ
unsigned char CRCchecks=0;		     //ilosc powtorzen przy bledzie CRC
volatile unsigned char crcL,crcH;	     //suma CRC odebrana z portu
volatile unsigned int CheckSum;		     //suma CRC policzona
volatile unsigned char _BytePtr=0;	     //numer przesylanego bajtu
volatile unsigned char DataType=0;	     //1-dane 2-crc
unsigned char PacketSize=0;		     //rozmiar pakietu
unsigned char Wait=1;			     //czas reakcji emulatora
unsigned char COMerror;			     //numer bledu

//inne zmienne pomocnicze
const unsigned char hextab[16]= {"0123456789ABCDEF"};

const unsigned char TAB=9;
const unsigned char STAB=15;
const unsigned char ENTER=13;
const unsigned char ESC=27;
const unsigned char UP=72;
const unsigned char DOWN=80;
const unsigned char LEFT=75;
const unsigned char RIGHT=77;
const unsigned char PGUP=73;
const unsigned char PGDOWN=81;
const unsigned char SPACE=32;
const unsigned char CR=13;
const unsigned char LF=10;
const unsigned char _OK[3]="OK";
const unsigned char _Anuluj[7]="Anuluj";
const unsigned char _Uwaga[8]="Uwaga !";
const unsigned char _AdresP[19]="Adres poczätkowy :";
const unsigned char _AdresK[19]="Adres koécowy    :";
const unsigned char _AdresD[19]="Adres docelowy   :";
const unsigned char _IloscB[19]="Iloêã bajtèw     :";

unsigned char far TempBuf[128];		     //bufor pomocniczy
unsigned char StrA[40],StrB[40];
unsigned char Byte;
unsigned int Word;
unsigned long Long;
unsigned int RSeg,ROfs;
unsigned char Key,SKey,OK;

//lista komunikatow
unsigned char far ErrorStr[11][40]=
{
 "Nowe ustawienia zacznä dziaçaã przy",
 "naståpnym uruchomieniu programu.",
 "Brak pliku pomocy.",
 "Bçäd podczas zapisu na dysku.",
 "Bçäd odczytu z dysku.",
 "Bçäd w nazwie pliku.",
 "Bçäd w pliku definicji.",
 "Dysk moíe byã peçny.",
 "Bçäd komunikacji z emulatorem.",
 "Sprawdë poçäczenie z komputerem.",
 "Brak pamieci"};

//ustawienia dla systemu menu
const unsigned char MaxMenuNumX=6;
const unsigned char MaxMenuNumY=8;
const unsigned char MaxMenuLen=35;
const unsigned char Begins[10]= {4,12,22,36,59,73,1,1,1,1};

typedef unsigned char far MenuStr[8][35];
MenuStr MainMenu=
 {"Plik",
  "Edycja",
  "Widok",
  "Symulacja",
  "Ustawienia",
  "Pomoc","",""};

MenuStr FileMenu=
 {"Otwèrz...               F3",
  "Zachowaj                F2",
  "Zachowaj jako...  Shift+F2",
  "",
  "Zakoécz              Alt+X","","",""};

MenuStr EditMenu=
 {"Kopiuj...         Ctrl+K",
  "Przesué...        Ctrl+R",
  "Wypeçnij...       Ctrl+F",
  "",
  "Wyczyêã bufor     Ctrl+D",
  "Wyczyêã etykiety",
  "",
  "Idë do...         Ctrl+G"};

MenuStr ViewMenu=
 {"DeAsembler        F5",
  "Edytor HEX 16     F6",
  "Edytor HEX 8      F7",
  "Mapa bitowa       F8","","","",""};

MenuStr SymMenu=
 {"Zapisz pamiåã...",
  "Odczytaj pamiåã RAM...",
  "Odczytaj pamiåã ROM...",
  "",
  "Uruchom symulacjå        Ctrl+F10",
  "Przerwij symulacjå       Ctrl+F11",
  "Resetuj                  Ctrl+F12",""};

MenuStr SetupMenu=
 {"COM...",
  "Transmisja...",
  "EPROM...",
  "",
  "Kolory...",
  "",
  "Zachowaj",""};

MenuStr HelpMenu=
 {"Instrukcja obsçugi        F1",
  "Pamiåci EPROM       Shift+F1",
  "",
  "O programie...","","","",""};

typedef unsigned char far StatusStr[78];
StatusStr Status;
StatusStr StatusMain[6]=
 {"Menu PLIK",
  "Menu EDYCJA",
  "Menu WIDOK",
  "Menu SYMULACJA",
  "Menu USTAWIENIA",
  "Menu POMOC"};

StatusStr StatusFile[5]=
 {"Otwiera plik",
  "Zachowuje zawartoêã pamiåci na dysku",
  "Zachowuje zawartoêã pamiåci w podanym pliku",
  "",
  "Wyjêcie z programu"};

StatusStr StatusEdit[8]=
 {"Kopiuje blok pamiåci",
  "Przesuwa blok pamiåci",
  "Wypeçnia blok pamiåci zadanä wartoêciä ",
  "",
  "Czyêci zawartoêã caçego bufora",
  "Kasuje zdefiniowane etykiety",
  "",
  "Przesuwa kursor pod wskazane miejsce"};

StatusStr StatusView[4]=
 {"Wçäcza widok DeAsemblera 8051",
  "Wçäcza widok edytora HEX (16 bajtèw w linii)",
  "Wçäcza widok edytora HEX (8 bajtèw w linii)",
  "Wçäcza widok mapy bitowej"};

StatusStr StatusSym[7]=
 {"Zapisuje dane do pamiåci emulatora",
  "Odczytuje dane z pamiåci RAM emulatora",
  "Odczytuje dane z pamiåci ROM emulatora",
  "",
  "Uruchamia symulacjå",
  "Przerywa symulacjå",
  "Wystawia sygnaç RST na wtyku emulacyjnym"};

StatusStr StatusSetup[7]=
 {"Ustawienia numeru, adresu i przerwaé dla portu COM",
  "Ustawienie parametrèw dotyczäcych transmisji",
  "Ustawienie wielkoêci pamiåci EPROM",
  "",
  "Ustawienia kolorèw programu",
  "",
  "Zachowuje bieíäce ustawienia jako domyêlne"};

StatusStr StatusHelp[4]=
 {"Podråcznik uíytkownika Emulatora EPROM",
  "Informacje o typach i rodzajach pamiåci EPROM",
  "",
  "Informacje o programie"};