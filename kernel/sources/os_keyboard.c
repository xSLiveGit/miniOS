#include "os_keyboard.h"
#include "screen.h"
#include "osrt.h"
#include "pic.h"

static const char key_range1_0[]    = "1234567890";
static const char key_rangeQ_P[]    = "qwertyuiop";
static const char key_rangeA_L[]    = "asdfghjkl";
static const char key_rangeZ_M[]    = "zxcvbnm";

bool KbPrvIsKeyPressed(uint8_t Code)
{
    return !(Code & 0x80);
}

bool KbPrvIsDigit(uint8_t Code)
{
    return ((Code >= 0x02) && (Code <= 0x0B));
}
char KbPrvGetDigitCharFromCode(uint8_t Code)
{
    return *(key_range1_0 + (Code - 0x02));
}


bool KbPrvIsQweLine(uint8_t Code)
{
    return ((Code >= 0x10) && (Code <= 0x19));
}
char KbPrvGetQweChar(uint8_t Code)
{
    return *(key_rangeQ_P + (Code - 0x10));
}

bool KbPrvIsAsdLine(uint8_t Code)
{
    return ((Code >= 0x1E) && (Code <= 0x26));
}
char KbPrvGetAsdChar(uint8_t Code)
{
    return *(key_rangeA_L + (Code - 0x1E));
}

bool KbPrvIsZxcLine(uint8_t Code)
{
    return ((Code >= 0x2C) && (Code <= 0x32));
}
char KbPrvGetZxcChar(uint8_t Code)
{
    return *(key_rangeZ_M + (Code - 0x2C));
}

bool KbPrvIsEnter(uint8_t Code)
{
    return (Code == 0x1C);
}
bool KbPrvIsSpace(uint8_t Code)
{
    return (Code == 0x39);
}

bool KbPrvIsLetter(uint8_t Code)
{
    return KbPrvIsQweLine(Code) ||
        KbPrvIsAsdLine(Code) ||
        KbPrvIsZxcLine(Code);
}

char TreatCharWithCapsLock(char Character)
{
    if(gEnviroment.KeyboardInfo.IsCapsLockOn)
    {
        return Character - ' ';
    }

    return Character;
}

char KbPrvGetCharFromLetterCode(uint8_t Code)
{
    char chr = 0;

    if(KbPrvIsQweLine(Code))
    {
        chr = KbPrvGetQweChar(Code);
    }
    else if(KbPrvIsAsdLine(Code))
    {
        chr =  KbPrvGetAsdChar(Code);
    }
    else if(KbPrvIsZxcLine(Code))
    {
        chr = KbPrvGetZxcChar(Code);
    }
    else
    {
        return chr;    
    }

    return TreatCharWithCapsLock(chr);
}

//see https://wiki.osdev.org/PS/2_Keyboard for codes
void IsrKeyboardKeyHandler(uint8_t Code)
{
    char chr = 0; //init with null
	
    if (!KbPrvIsKeyPressed(Code))
	{
		return;
	}

    if(KbPrvIsDigit(Code))
    {
        chr = KbPrvGetDigitCharFromCode(Code);
    }
    else if(KbPrvIsLetter(Code))
    {
        chr = KbPrvGetCharFromLetterCode(Code);
    }
    else if(KbPrvIsEnter(Code))
    {
        chr = '\n';
    }
    else if(KbPrvIsSpace(Code))
    {
        chr = ' ';
    }
    else if(Code == 0x3A)//CapsLock
    {
        gEnviroment.KeyboardInfo.IsCapsLockOn = 1 - gEnviroment.KeyboardInfo.IsCapsLockOn;
        return;
    }
    else if(Code == 0xE)//Back space
    {
        ScrRemoveCharFromCurrentLine(&gEnviroment.ScreenBuffer);
        return;
    }

    if(chr != 0)
    {
        os_printf("%c", chr);
    }
    else    //not handled
    {
        os_printf("IsrAsmKeyboard was called for unhandled code: %x\n", Code);
    }
}

void IsrKeyboard(void)
{
    __cli();

    uint8_t code = __inb(KYBRD_ENC_INPUT_BUF);
    IsrKeyboardKeyHandler((code));
    __outb(PIC_MASTER_CTRL, PIC_EOI);

    __sti();
}

