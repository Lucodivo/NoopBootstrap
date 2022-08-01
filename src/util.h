#pragma once

void setFlags(b32* outFlags, b32 newFlags) { *outFlags |= newFlags; }
void removeFlags(b32* outFlags, b32 removeFlags) { *outFlags &= ~removeFlags; }