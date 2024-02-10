#ifndef EKEYBOARD_H
#define EKEYBOARD_H

#include <LKeyboard.h>

using namespace Louvre;

class EKeyboard : public LKeyboard
{
public:
    EKeyboard(const void *params);

    void keyEvent(UInt32 keyCode, KeyState keyState) override;
    void keyModifiersEvent(UInt32 depressed, UInt32 latched, UInt32 locked, UInt32 group) override;
};

#endif // EKEYBOARD_H
