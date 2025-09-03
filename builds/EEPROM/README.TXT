
		======	EEPROM (Emulator Pamieci EPROM)  ======





	Spis tresci
	-----------
1. Zmiany w programie EEPROM.EXE
2. Wymagania sprzetowe programu
3. Parametry techniczne emulatora pamieci EPROM








--------------------------------------------------------------------------------


1. Zmiany w programie EEPROM.EXE
--------------------------------

   a)	  Program umozliwia zapis bufora na dysku w postaci pliku binarnego,
	  pliku w formacie Intel HEX II lub pliku tekstowego zawierajacego
	  program po deasemblacji. Zapis w formacie Intel HEX II dokonywany
	  jest zawsze, gdy podamy w nazwie pliku do zapisu rozszerzenie
	  ".HEX". Zapis postaci tekstowej deasemblera nastepuje po wpisaniu
	  rozszerzenia ".ASM". Postac ta nie moze zostac ponownie odczytana
	  przez program. Jest ona przydatna tylko do analizy lub zmiany kodu
	  programu za pomoca kompilatora '51. Przy podaniu innego 
	  rozszerzenia lub jego braku dane bufora zostaja zapisane w formie
	  binarnej. Za kazdym razem zapisywana jest cala widoczna czesc 
	  bufora, tj. wielkosc bufora ustawiona w menu "USTAWIENIA/EPROM".
	
   b)	  Wszystkie etykiety sa wyswietlane z uwzglednieniem duzych i malych
	  liter. Mozliwe jest zdefiniowanie kilku etykiet o tej samej nazwie
	  lecz roznych adresach. Nalezy zwrocic uwage na fakt, ze gdy zostanie
	  zadeklarowanych kilka etykiet adresowych o tej samej nazwie,
	  to podczas uzywania polecenia "IDZ DO" w menu "EDYCJA" skoki beda
	  wykonywane do pierwszej (liczac od adresu 0000h) napotkanej
	  etykiety adresowej.
	
   c)	  W czasie pracy programu EEPROM mozliwe jest sprawdzenie czy emulator
	  jest poprawnie podlaczony do komputera. Nalezy wowczas wylaczyc i po
	  pewnym czasie ponownie wlaczyc emulator, gdy polaczenie jest 
	  poprawne to po wlaczeniu emulatora bedzie slychac krotki sygnal
	  dzwiekowy z glosnika komputera. Jesli z jakichs powodow sygnal nie
	  bedzie slyszalny podczas inicjowania emulatora to nalezy sprawdzic
	  poprawnosc konfiguracji portu COM oraz kabla polaczeniowego. Jesli
	  wszystkie ustawienia sa poprawne i nadal nie mozna uzyskac polaczenia
	  z emulatorem to nalezy ponownie uruchomic komputer naciskajac 
	  przycisk RESET na obudowie.


2. Wymagania sprzetowe programu
-------------------------------

	- DOS 6.0 lub wyzszy lub Windows 95/98
	  (w przypadku Windows 95/98 polskie litery beda widoczne
           tylko w trybie pelnoekranowym)
        - Procesor:        286/12 MHz, zalecane 386/40 MHz lub szybszy
	- Pamiec:          640 kB RAM, ok. 360 kB wolnych
	- Karta graficzna: VGA

	UWAGA !
	W przypadku pracy pod kontrola Windows 95/98 nie nalezy stosowac
	polskiej strony kodowej 852, poniewaz w programie zle beda wyswietlane
	obramowania.


3. Parametry techniczne emulatora pamieci EPROM
-----------------------------------------------

	- zakres napiecia zasilania     :  6-15 V
	- maksymalna predkosc transmisji:  57600 bps
	- czas dostepu pamieci emulatora:  < 150 ns
	- emulowane i odczytywane uklady pamieci:
		    2764 =>  8 kB EPROM
		   27128 => 16 kB EPROM
		   27256 => 32 kB EPROM
		   27512 => 64 kB EPROM 
	  rowniez wersje CMOS powyzszych ukladow



