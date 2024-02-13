#include <LScene.h>
#include <LCursor.h>
#include <LOutput.h>
#include "EKeyboard.h"
#include "Global.h"

EKeyboard::EKeyboard(const void *params) : LKeyboard(params) {}

void EKeyboard::keyEvent(UInt32 keyCode, KeyState keyState)
{
    if (keyState == Released)
    {
        const bool L_META       { isKeyCodePressed(KEY_LEFTMETA) };
        const bool L_SHIFT      { isKeyCodePressed(KEY_LEFTSHIFT) };
        LOutput *cursorOutput   { cursor()->output() };

        if (L_META && L_SHIFT && cursorOutput)
        {
            if (keyCode == KEY_UP && cursorOutput->fractionalScale() < 3.f)
                cursorOutput->setScale(cursorOutput->fractionalScale() + 0.25f);
            else if (keyCode == KEY_DOWN && cursorOutput->fractionalScale() > 0.25f)
                cursorOutput->setScale(cursorOutput->fractionalScale() - 0.25f);
        }
    }

    G::scene()->handleKeyEvent(keyCode, keyState);
}

void EKeyboard::keyModifiersEvent(UInt32 depressed, UInt32 latched, UInt32 locked, UInt32 group)
{
    G::scene()->handleKeyModifiersEvent(depressed, latched, locked, group);
}
