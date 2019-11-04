#ifndef REGISTOR_H
#define REGISTOR_H

using namespace std;

#include "mips.h"

class registor
{
    private:
        int *intReg;
        float *flpReg;
    public:
        registor(int iRegSize, int fRegSize);
        ~registor();
        bool get(string name, void *ret);
        bool set(string name, void *val);
        void clear();
};

#endif