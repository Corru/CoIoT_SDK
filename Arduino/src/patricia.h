#ifndef H_PATRICIA
#define H_PATRICIA

#include "kvelem.h"
/*#include <cctype>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>*/
using namespace std;
class TPatricia {
  public:
    unsigned short Number;
    TPatricia *Left = NULL;
    TPatricia *Right = NULL;
    TKvelem *Elem = NULL;

    TPatricia();

    short Insert(const string &word, TCallbackHandlerFunction val);
    short Remove(const string &word);
    TPatricia *Search(const string &word);
    TPatricia *SearchParent(TPatricia *root);
    TPatricia *SearchBackwardPointer(TPatricia *root);

    bool Empty();
    short Destroy();

    inline static unsigned short GetBit(const string &word, unsigned short bit);

    virtual ~TPatricia();
};

#endif
