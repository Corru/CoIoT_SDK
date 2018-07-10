#ifndef H_KVELEM
#define H_KVELEM

//#define string String
#include <string>
#include <Arduino.h>

typedef std::function<void(byte*,unsigned int)> TCallbackHandlerFunction;

using namespace std;
class TKvelem {
  private:
  public:
    string Key;
    TCallbackHandlerFunction Val;
    short Length;
    TKvelem(string newKey, TCallbackHandlerFunction newVal);
    void SetKey(const string &newKey);
    void SetVal(TCallbackHandlerFunction newVal);
    const string &GetKey();
    TCallbackHandlerFunction GetVal();
    void Print();
    ~TKvelem();
};

#endif
