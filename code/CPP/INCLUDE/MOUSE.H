void InitMouse(void)
{
 asm mov ax,0;
 asm int 0x33;
}

void ShowMouse(void)
{
 asm mov ax,2;
 asm int 0x33;
 delay(100);
 asm mov ax,1;
 asm int 0x33;
}