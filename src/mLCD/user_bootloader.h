//Reset Vector Address
#build(reset=0x200)
//Intterupts Vector Address
#build(interrupt=0x208)

//Bootloader Area
#org 0x0000,0x01ff
void bootloader() 
{
#asm
  nop //No Operation
#endasm
} 

