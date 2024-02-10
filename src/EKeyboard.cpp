#include <LScene.h>
#include "EKeyboard.h"
#include "Global.h"

EKeyboard::EKeyboard(const void *params) : LKeyboard(params) {}

void EKeyboard::keyEvent(UInt32 keyCode, KeyState keyState)
{
    G::scene()->handleKeyEvent(keyCode, keyState);
}

void EKeyboard::keyModifiersEvent(UInt32 depressed, UInt32 latched, UInt32 locked, UInt32 group)
{
    G::scene()->handleKeyModifiersEvent(depressed, latched, locked, group);
}
