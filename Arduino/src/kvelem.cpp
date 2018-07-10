#include "kvelem.h"

TKvelem::TKvelem(string newKey, TCallbackHandlerFunction newVal)
    : Key(newKey), Val(newVal), Length(newKey.length()) {}

void TKvelem::Print() {}
void TKvelem::SetKey(const string &newKey) { Key = newKey; }
void TKvelem::SetVal(TCallbackHandlerFunction newVal) { Val = newVal; }
const string &TKvelem::GetKey() { return Key; }
TCallbackHandlerFunction TKvelem::GetVal() { return Val; }

TKvelem::~TKvelem() {}