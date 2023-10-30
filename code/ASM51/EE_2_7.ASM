;		*** Program obslugujacy emulator pamieci EPROM ***

			;*** ZNACZENIE BITOW STERUJACYCH ***
LED  	BIT P3.2	; DIODA LED:          0=ON
			;	              1=OFF

GATE 	BIT P3.3	; TRYB PRACY:         0=SYMULACJA
			;		      1=DOSTEP DO PAMIECI

SEL0	BIT P3.4	; DOSTEP DO PAMIECI: SEL0 SEL1
SEL1	BIT P3.5	; 		      0    0   ROM aktywny
			;		      0    1   RAM i ROM nieaktywne
			;		      1    0   stan nie dozwolony
			;		      1    1   RAM aktywny

	LJMP INIT

	ORG 0BH			;*** TIMER 0 INTERRUPT ***
	PUSH PSW
	CLR TR0			;zatrzymaj licznik T0
	MOV TH0,#0		;zeruj jego zawartosc
	MOV TL0,#0
	SETB LED		;zgas LED
	POP PSW
	RETI

	ORG 23H 		;*** SERIAL INTERRUPT ***
	PUSH PSW		;zapamietaj slowo stanu
	PUSH ACC		;zapamietaj A
	CLR LED			;zapal LED
	JB RI,RX		;jesli odebrano znak to skok do RX
	JB TI,TX		;jesli nadano znak to skok do TX
TX:	
	MOV A,17H		;zaladuj A bajtem nadanym
	ADD A,15H		;dodaj CRC L do A
	MOV 15H,A		;aktualizuj sume CRC L
	JNC ENDCRC1
	INC 14H			;jesli C=1 to zwieksz CRC H
ENDCRC1:	
	MOV A,R1		;R1 to liczba bajtow nadanych
	CJNE A,13H,T01		;jesli R1<>wielkosc paczki to skok
	SETB 4			;jesli R1=@13H to koniec paczki danych
	SJMP EOTI
T01:
	INC R1			;zwieksz licznik nadanych bajtow
	SETB 5			;ustaw znacznik nadawania
	SJMP EOTI
RX:
	CLR REN			;zablokuj odbiornik RS-232
	MOV A,SBUF		;dodaj bajt odebrany do sumy CRC L
	ADD A,15H
	MOV 15H,A		;aktualizuj sume CRC L
	JNC ENDCRC2		;jesli nie bylo przeniesienia to koniec
	INC 14H			;jesli C=1 to zwieksz sume CRC H o 1
ENDCRC2:
	JB 1,R01		;jesli znacznik kodu rozkazu=1 to skok
	CLR 2			;jesli  BIT 1=0 to pozostale dwa takze=0
	CLR 3
	MOV 13H,#3		;ustawia wielkosc paczki na 3 bajty
	MOV R1,#1		;ustaw licznik odebranych/nadanych bajtow na 1
	MOV @R0,SBUF		;zapisz w buforze kod instrukcji
	INC R0			;przesun wskaznik na nastepny bajt bufora
	SETB 1			;ustaw bit znacznika kodu instrukcji
	SJMP EORI
R01:
	JB 2,R02
	INC R1			;zwieksz licznik odebranych/nadanych bajtow
	MOV @R0,SBUF		;zapisz 2 bajt instrukcji w buforze
	INC R0			;przesun wskaznik bufora instrukcji
	SETB 2			;ustaw znacznik 2 bajtu instrukcji
	SJMP EORI
R02:
	JB 3,R03
	INC R1
	MOV @R0,SBUF
	INC R0
	SETB 3			;ustaw znacznik 3 bajtu instrukcji
	SJMP EORI
R03:
	INC R1			;zwieksz licznik odebranych/nadanych bajtow
	MOV 16H,SBUF		;w 16H znajduje sie bajt odebrany
	SETB 6			;ustawia znacznik danych do odbioru
	SJMP EORI
EORI:				;koniec procedury odczytu
	CLR RI			;zerowanie znacznika RI
	SETB REN		;wlaczenie odbiornika RS-232
	SJMP EOI
EOTI:
	CLR TI			;zerowanie znacznika TI
EOI:
	SETB TR0		;start licznika T0
	POP ACC			;odtworz A
	POP PSW			;odtworz slowo stanu
	RETI			;*** END ***

	ORG {$+02H} 		;*** PROGRAM GLOWNY ***

INIT:
	MOV SP,#17H		;ustaw stos
	ACALL INITREG		;inicjacja zmiennych
	MOV A,#05H
	ACALL SETMODE		;ustawienie predkosci transmisji na tryb 5
	ACALL MEMTEST		;sprawdz pamieci
	ACALL READY		;wypisz logo
	MOV A,87H		;ustaw tryb IDL
	ORL A,#00000001B
	MOV 87H,A

MAIN:
	JB 4,SENDCRC		;jesli koniec paczki to nadaj sume CRC
	JB 7,REG		;jesli dostep do RAM procesora to skok
	JB 6,LOADMEM		;zapisz bajt w pamieci RAM
	JB 5,SENDMEM		;nadaj bajt odczytany z pamieci RAM
	JB 3,NOIDLE
	MOV A,87H		;ustaw tryb IDL
	ORL A,#00000001B
	MOV 87H,A
NOIDLE:
	JNB 3,MAIN		;jesli odebrano 3 bajtowy kod instrukcji
	JB 0,MAIN		;i jesli kod instrukcji jest OK to skok
	ACALL DECODE		;dekoduj kod instrukcji
CHECK:
	MOV A,R1		;R1 to liczba bajtow nadanych
	CJNE A,13H,MAIN 	;jesli R1<>wielkosc paczki to skok
	SETB 4			;jesli R1=@13H to koniec paczki danych
	SJMP MAIN
REG:
	JB 6,LOADREG		;zapisz bajt w pamieci RAM procesora
	JB 5,SENDREG		;nadaj bajt odczytany z pamieci RAM procesora
	SJMP MAIN

				;*** PROCEDURY ***
	
LOADMEM:			;*** PROCEDURA LADUJACA 1 BAJT DO PAMIECI ***
	CLR REN			;blokuj odbiornik RS-232
	MOV A,16H		;przepisz bajt odebrany do A
	MOV DPH,R4
	MOV DPL,R5
	MOVX @DPTR,A		;zapisz bajt
	INC DPTR		;zwieksz adres o 1
	MOV R4,DPH
	MOV R5,DPL
	CLR 6			;zeruj znacznik odbioru danych
	SETB REN		;uaktywnij odbiornik RS-232
	AJMP CHECK		;*** END LOADRAM ***

SENDMEM:			;*** PROCEDURA ODCZYTUJACA 1 BAJT Z PAMIECI ***
	CLR REN			;blokuj odbiornik RS-232
	MOV DPH,R6		
	MOV DPL,R7
	MOVX A,@DPTR		;bajt odczytany w A
	INC DPTR		;zwieksz adres o 1
	MOV R6,DPH
	MOV R7,DPL
	MOV 17H,A		;zaladuj bajt odebrany z A
	MOV SBUF,17H		;wyslij bajt przez RS-232
	CLR 5			;zeruj znacznik nadawania danych
	SETB REN		;uaktywnij odbiornik RS-232
	AJMP CHECK		;*** END SENDRAM***

SENDCRC:			;*** PROCEDURA WYSYLAJACA SUME KONTROLNA ***
	CLR ES			;zablokuj przerwania z RS-232
	JB TI,$			;czekaj na wyslanie calego znaku
	MOV SBUF,14H		;CRC H
	JNB TI,$		;czekaj na wyslanie calego znaku
	CLR TI			;zeruj znacznik TI
	MOV SBUF,15H		;CRC L
	JNB TI,$
	CLR TI
	MOV 13H,#3		;ustaw domyslny rozmiar pakietu
	MOV 14H,#0		;zerowanie
	MOV 15H,#0		;CRC
	MOV 20H,#0		;rozpoczecie nowego cyklu
	MOV R0,#10H		;ustaw R0 na poczatek bufora instrukcji
	MOV R1,#0		;zeruj licznik odebranych/nadanych bajtow
	SETB ES			;wlacz przerwania z RS-232
	AJMP MAIN		;*** END SENDCRC ***

LOADREG:			;*** PROCEDURA ZAPISUJACA BAJT W PAMIECI PROCESORA ***
	CLR REN			;blokuj odbiornik RS-232
	MOV A,R5		;sprawdz czy adres L=0 (R0)
	JNZ LREG0		;jesli adres L<>R0 to skok
	MOV R0,16H		;zapisz bajt odebrany w R0
	SJMP LREG1
LREG0:
	MOV A,R0
	PUSH ACC		;zapamietaj R0
	MOV A,R5		;laduj A adresem L
	MOV R0,A		;zapisz adres L w R0
	MOV @R0,16H		;zapisz bajt w pamieci procesora
LREG1:
	CLR 6			;zeruj znacznik odbioru danych
	CLR 7			;zeruj znacznik dostepu do pamieci procesora
	POP ACC
	MOV R0,A		;przywroc R0
	SETB REN		;uaktywnij odbiornik RS-232
	AJMP CHECK		;*** END LOADREG ***

SENDREG:
	CLR REN			;blokuj odbiornik RS-232
	MOV A,R0
	PUSH ACC		;zapamietaj R0
	MOV A,R7
	JNZ SREG0
	MOV A,R0
	SJMP SREG1
SREG0:
	MOV R0,A		;zapisz adres L w R0
	MOV A,@R0		;odczytaj bajt z pamieci procesora
SREG1:
	MOV 17H,A		;zapisz go w buforze nadawania
	MOV SBUF,17H		;wyslij ten bajt przez RS-232
	CLR 5			;zeruj znacznik nadawania danych
	CLR 7			;zeruj znacznik dostepu do pamieci procesora
	POP ACC
	MOV R0,A		;przywroc R0
	SETB REN		;uaktywnij odbiornik RS-232
	AJMP CHECK		;*** END SENDREG***

DECODE:				;*** PROCEDURA DEKODUJACA ROZKAZ ***
	MOV A,10H		;kod rozkazu w A

_00:				;* (00H) NOP *
	JNZ _01
	AJMP END_DECODE

_01:				;* (01H-40H) ZAPIS 1-64 BAJTOW POD ADRES *
	CJNE A,#40H,_NIE20
	SJMP _01_40
_NIE20:
	JNC _41			;jesli A>40H to skok
_01_40:
	MOV A,10H
	ADD A,#3		;3 + kod instrukcji = nowy rozmiar paczki
	MOV 13H,A		;zaladuj nowa dana
	MOV R3,10H		;zaladuj licznik bajtow L
	MOV R4,11H		;zaladuj adres docelowy (H I L)
	MOV R5,12H		;adresem odebranym z RS-232
	SETB GATE		;wymusza tryb dostepu
	SETB SEL0		;do pamieci
	SETB SEL1		;RAM
	AJMP END_DECODE

_41:				;* (41H-80H) ODCZYT 1-64 BAJTOW Z ADRESU *
	CJNE A,#80H,_NIE80
	SJMP _41_80
_NIE80:
	JNC _A0			;jesli A>80h to skok
_41_80:
	MOV A,10H		;zaladuj A kodem rozkazu
	CLR C			;zeruj C
	SUBB A,#40H		;odejmij od kodu rozkazu 40H (ilosc bajtow do odczytu)
	MOV R3,A		;zaladuj licznik bajtow
	ADD A,#3		;zwieksz o 3
	MOV 13H,A		;i zaladuj jako wielkosc paczki
	MOV R6,11H		;zaladuj adres zrodlowy (H I L)
	MOV R7,12H		;adresem odebranym z RS-232
	SETB 5			;ustaw znacznik nadawania
	AJMP END_DECODE

_A0:				;* (A0H) USTAW TRYB "SYMULACJA" *
	CJNE A,#0A0H,_B0
	CLR GATE
	CLR SEL0
	SETB SEL1
	AJMP END_DECODE

_B0:				;* (B0H) USTAW TRYB "ZAPIS/ODCZYT" RAM *
	CJNE A,#0B0H,_BB
	SETB GATE
	SETB SEL0
	SETB SEL1
	AJMP END_DECODE

_BB:				;* (BBH) USTAW TRYB "ODCZYT" ROM *
	CJNE A,#0BBH,_C0
	SETB GATE
	CLR SEL0
	CLR SEL1
	AJMP END_DECODE

_C0:				;* (C0H) USTAW PREDKOSC RS-232 *
	CJNE A,#0C0H,_CC
	MOV A,#5
	ACALL DELAY		;opoznienie przed zmiana predkosci pracy RS-232
	MOV A,12H		;3 bajt kodu instrukcji zawiera tryb pracy
				;2 bajt jest nie istotny
	ACALL SETMODE		;zmien predkosc RS-232
	AJMP END_DECODE

_CC:				;* (CCH) USTAW PREDKOSC RS-232 RECZNIE *
	CJNE A,#0CCH,_D0
	MOV A,#5
	ACALL DELAY		;opoznienie przed zmiana predkosci pracy RS-232
	CLR EA			;zablokowanie przerwan z RS-232
	CLR REN			;wylaczenie odbiornika RS-232
	CLR TR1			;wylaczenie licznika T1
	MOV TH1,11H		;zapisz nowa wartosc do licznika T1
	SETB TR1		;start licznika T1
	MOV A,12H		;sprawdz czy wyzerowac SMOD
	JZ MODE0		;jesli tak to skok
MODE1:
	ORL 87H,#10000000B	;ustaw SMOD=1
	SJMP END_MODE
MODE0:
	ANL 87H,#01111111B	;zeruj SMOD=0
END_MODE:
	MOV A,#10
	ACALL DELAY		;opoznienie przed zmiana predkosci pracy RS-232
	SETB EA			;wlaczenie przerwan
	SETB ES			;wlaczenia przerwan RS-232
	SETB REN		;wlaczenie odbiornika RS-232
	AJMP END_DECODE

_D0:				;* (D0H) ZAPISZ 1 BAJT DO PAMIECI PROCESORA *
	CJNE A,#0D0H,_DD
	MOV 13H,#04H		;ustawia wielkosc paczki na 4 bajty
	MOV R5,12H		;zaladuj docelowy adres L
	SETB 7			;ustaw znacznik dostepu do pamieci procesora
	AJMP END_DECODE

_DD:				;* (DDH) ODCZYTAJ 1 BAJT Z PAMIECI PROCESORA *
	CJNE A,#0DDH,_E0
	MOV 13H,#04H		;ustawia wielkosc paczki na 4 bajty
	MOV R7,12H		;zaladuj zrodlowy adres L
	SETB 5			;ustaw znacznik nadawania
	SETB 7			;ustaw znacznik dostepu do pamieci procesora
	AJMP END_DECODE

_E0:				;* (EOH) USTAW TRYB POWER DOWN *
	CJNE A,#0E0H,_F0
	MOV A,87H
	ORL A,#00000010B
	MOV 87H,A
	AJMP END_DECODE

_F0:				;* (F0H) PODAJ WERSJE PROGRAMU *
	CJNE A,#0F0H,_F1
	MOV 14H,#0		;ustaw CRC na 0
	MOV 15H,#0
	ACALL READY
	AJMP END_DECODE

_F1:				;* (F1H) WYSWIETL LOGO EMULATORA *
	CJNE A,#0F1H,_FF
	CLR A
	MOV DPTR,#VER
	MOVC A,@A+DPTR
	MOV 14H,A		;crc L=VER
	MOV A,#1
	MOVC A,@A+DPTR
	MOV 15H,A		;crc H=VER
	MOV DPTR,#STR1		;zaladuj adres ciagu znakow
	ACALL SENDSTR		;wyslij go
	AJMP END_DECODE

_FF:				;* (FFH) CREDITS *
	CJNE A,#0FFH,END_DECODE
	MOV 14H,#3AH		;ustaw CRC (":)")
	MOV 15H,#29H
	MOV DPTR,#STR2		;zaladuj adres ciagu znakow
	ACALL SENDSTR		;wyslij go
	AJMP END_DECODE

END_DECODE:			;koniec procedury pomyslny
	SETB 0			;ustaw znacznik zdekodowanego rozkazu
	RET			;*** END DECODE ***

SETMODE:			;*** PROCEDURA USTAWIAJACA PREDKOSC RS-232 ***
	CLR EA			;zablokowanie przerwan
	CLR REN			;wylaczenie odbiornika RS-232
	CLR TR1			;wylaczenie licznika T1
	MOV TMOD,#00100001B	;ustawienie licznika T1: tryb 2 i T0: tryb 1
	MOV SCON,#01100000B	;ustawienie transmisjii szeregowej
		 		;na tryb 1 (8 bitow)
 				;sprzetowe odrzucanie znakow bez bitu stopu
	MOV DPTR,#BPS		;zaladowanie adresu tablicy predkosci
	MOV B,#2		;w A numer trybu pracy (predkosc)
	MUL AB			;A*2=adres komorki (2 bajtowej)
	PUSH ACC		;zapamietaj ten adres
	MOVC A,@A+DPTR		;odczytanie wartosci z tablicy predkosci
	MOV TH1,A		;ustalenie predkosci transmisjii
	POP ACC			;odtworz adres komorki w tablicy
	INC A			;i zwieksz go o jeden (nastepny bajt)
	MOVC A,@A+DPTR		;zaladuj do A
	JZ SMOD0		;jesli A=0 to skocz
SMOD1:
	ORL 87H,#10000000B	;ustaw SMOD=1
	SJMP END_SET
SMOD0:
	ANL 87H,#01111111B	;zeruj SMOD=0
END_SET:
	SETB TR1		;start licznika T1
	SETB ES			;odblokowanie przerwan z RS-232
	SETB EA			;wlaczenie przerwan
	SETB REN		;wlaczenie odbiornika RS-232
	RET			;*** END SETMODE ***


READY:				;*** PROCEDURA WYPISUJACA WERSJE PROGRAMU ***
	CLR ES			;wylacz przerwania z RS-232
	MOV DPTR,#VER		;zaladuj adres ciagu znakow
	CLR A			;przesuniecie 0
	MOVC A,@A+DPTR
	MOV SBUF,A		;wyslij znak
	JNB TI,$		;czekaj na RS-232
	CLR TI			;zeruj znacznik przerwania
	INC DPTR		;zwieksz adres
	CLR A			;przesuniecie 0
	MOVC A,@A+DPTR
	MOV SBUF,A		;wyslij znak
	JNB TI,$		;czekaj na RS-232
	CLR TI			;zeruj znacznik przerwania
	SETB ES			;wlacz przerwania z RS-232
	RET			;*** END READY ***

SENDSTR:			;*** PROCEDURA WYSYLAJACA CIAG ZNAKOW ***
				;DPTR=adres poczatku ciagu
				;CRC=dowolne (nie jest zliczane)
	CLR ES			;wylacz przerwania z RS-232
REP:
	CLR LED			;wlacz LED
	CLR A
	MOVC A,@A+DPTR		;odczytaj bajt
	INC DPTR		;zwieksz wskaznik o 1
	MOV SBUF,A		;wyslij bajt
	JNB TI,$		;czekaj na koniec trnasmisji
	CLR TI
	JNZ REP			;powtarzaj dopoki bajt nie bedzie zerem
	SETB ES			;wlacz przerwania z RS-232
	RET			;*** END SENDSTR ***

DELAY:				;*** PROCEDURA OPOZNIAJACA ***
	 			;powtorz A razy przepelnienie licznika T0
				;DELAY=(A*12*65535)+2 cykli
	SETB TR0		;uruchom licznik T0
	JB TR0,$		;skacz dopoki T0 nie zatrzyma sie
	DEC A			;zmniejsz A o 1
	JNZ DELAY		;skacz jesli A<>0
	RET			;*** END DELAY ***

MEMTEST:			;*** PROCEDURA SPRAWDZAJACA PAMIECI ***
	PUSH PSW		;zapamietaj slowo stanu
	SETB GATE		;ustaw tryb "ZAPIS/ODCZYT" RAM
	SETB SEL0
	SETB SEL1
	CLR RS1			;wybierz 2 bank rejestrow
	SETB RS0
	MOV R0,#0AAh		;wartosc zapisywana
	MOV R4,#00h		;Adres H poczatku
	MOV R5,#00h		;Adres L poczatku
	MOV DPH,R4
	MOV DPL,R5
FILLMEM:			;wypelnij cala pamiec RAM
	MOV A,R0
	MOVX @DPTR,A		;zapisz wartosc
	INC DPTR		;zwieksz adres
	MOV A,DPL
	JNZ FILLMEM		;jesli A<>0 to skok
	MOV A,DPH
	JNZ FILLMEM		;jesli A<>0 to skok
	MOV DPH,R4		;zaladuj adres poczatkowy
	MOV DPL,R5
CHECKMEM:
	MOVX A,@DPTR		;odczytaj wartosc
	CJNE A,08h,FAILMEM	;cjne a,r0,failmem
	MOV A,#0
        MOVX @DPTR,A		;jesli komorka OK to ja zeruje
	INC DPTR		;zwieksz adres
	MOV A,DPL
	JNZ CHECKMEM		;skok jesli A<>0
	MOV A,DPH
	JNZ CHECKMEM		;skok jesli A<>0
	SJMP OKMEM		;pamiec sprawna
FAILMEM:			;jesli pamiec niesprawna to miga LED
	MOV P1,DPH		;na porcie P1 jest Adres H
	CLR LED			;zapal LED
NOEXIT:
	MOV R0,#80h
LOOP1:
	MOV R1,#0FFh
LOOP2:
	DEC R1
	MOV A,R1
	JNZ LOOP2
	DEC R0
	MOV A,R0
	JNZ LOOP1
	CPL LED			;miga LED
	SJMP NOEXIT		;petla bez wyjscia
OKMEM:
	POP PSW
	RET			;*** END MEMTEST ***

INITREG:			;*** PROCEDURA INICJUJACA WSZYSTKIE ZMIENNE ***
	MOV R0,#10H 		;ustaw poczatek bufora kodu instrukcji
	MOV R1,#0		;zeruj licznik odebranych/nadanych bajtow
	MOV R2,#0 		;zeruj licznik bajtow
	MOV R3,#0		;
	MOV 13H,#03H		;ustaw wielkosc paczki na 3
	MOV 14H,#0		;zeruj sume CRC H
	MOV 15H,#0		;i CRC L
	MOV 20H,#0		;zeruj znaczniki
	SETB PS			;ustaw wyzszy priorytet dla RS-232
	SETB EA			;wlacz system przerwan
	SETB ET0		;wlacz przerwania dla T0
	RET			;*** END INITREG ***

	ORG {$+02H}		;*** STALE ***

				;*** TABLICA PREDKOSCI RS-232 ***
BPS:	DB 0A0H,0H		;Tryb 0 :   300  BPS, SMOD=0
	DB 0D0H,0H		;Tryb 1 :   600  BPS, SMOD=0
	DB 0E8H,0H		;Tryb 2 :  1200  BPS, SMOD=0
	DB 0F4H,0H		;Tryb 3 :  2400  BPS, SMOD=0
	DB 0FAH,0H		;Tryb 4 :  4800  BPS, SMOD=0
	DB 0FDH,0H		;Tryb 5 :  9600  BPS, SMOD=0
	DB 0FDH,1H		;Tryb 6 : 19200  BPS, SMOD=1
	DB 0FFH,1H		;Tryb 7 : 57600  BPS, SMOD=1

	DB 13,10
				;*** LOGO EMULATORA ***
STR1:	DB '< Emulator EPROM ver: 2.7 > ',0H

 				;*** CREDITS ***
STR2:	DB 13,10
	DB '              < Emulator EPROM ver: 2.7 >'
	DB 13,10,10
	DB '              Wszystkie prawa zastrzezone.',13,10
	DB 'Kopiowanie programu w calosci lub w czesciach zabronione.'
	DB 13,10,10 
	DB 'Program na ',0E6H,'C 8051 napisal : Tomasz Nowakowski'
	DB 13,10,10
	DB 'Specjalne dzieki dla:',13,10
	DB 'nauczycieli  - za wsparcie, rady i dostep do pracowni',13,10
	DB 'Klosa        - za symulator EPROM',13,10
	DB 'Mentosa      - za programowanie kostek EPROM, 89C51 i literature',13,10
	DB 'Tasaka       - za drugi symulator i literature',13,10
	DB 'Doti         - za cierpliwosc',13,10
	DB 0H

				;*** VERSION ***
VER:	DB 02H,07H

	END			;*** KONIEC LISTINGU PROGRAMU ***