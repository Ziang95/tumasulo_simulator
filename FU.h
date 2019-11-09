
#ifndef FU_H
#define FU_H

#include <vector>
using namespace std;

#include "instruction.h"

typedef struct FU_QEntry
{
    bool done;
    opCode code;
    valType rtType;
    int dest;
    memCell *res;
    memCell *oprnd1;
    memCell *oprnd2;
    int offset;
    bool *busy;
}FU_QEntry;

#include "mips.h"

class FU_CDB{
    private:
        CDB queue[Q_LEN];
        CDB bus;
        int front, rear;
        mutex_t Q_lock;
    public:
        int next_vdd;
        FU_CDB();
        pthread_t handle;
        bool enQ(valType t, void *v, int s);
        int get_source();                      //Called at rising edges
        bool get_val(memCell *v);                 //Called at rising edges
        void CDB_automat();
};

void init_FU_CDB();

class FU_Q
{
    private:
        FU_QEntry queue[Q_LEN];
        int front, rear;
        mutex_t Q_lock;
    public:
        FU_Q();
        const FU_QEntry* enQ(opCode c, valType rt, int d, memCell *r, memCell *op1, memCell *op2, int offst, bool *busy);
        FU_QEntry* deQ();
};

class resStation;

#include "reserv_station.h"

class functionUnit
{
    public:
        FU_Q queue;
        FU_QEntry task;
        vector<resStation*> rs;
        functionUnit();
        int next_vdd;
        pthread_t handle;
        virtual void FU_automat(){};
};

class intAdder : public functionUnit
{
    public:
        intAdder();
        ~intAdder();
        void FU_automat();
};

class flpAdder : public functionUnit
{
    private:
        FU_QEntry *pLatency_Q;
        void shift(FU_QEntry &in, FU_QEntry &out);
    public:
        flpAdder();
        ~flpAdder();
        void FU_automat();
};

class flpMtplr : public functionUnit
{
    private:
        FU_QEntry *pLatency_Q;
        void shift(FU_QEntry &in, FU_QEntry &out);
    public:
        flpMtplr();
        ~flpMtplr();
        void FU_automat();
};

class ldsdUnit: public functionUnit
{
    public:
        ldsdUnit();
        ~ldsdUnit();
        void FU_automat();
};

void* intAdder_thread_container(void *arg);
void* flpAdder_thread_container(void *arg);
void* flpMlptr_thread_container(void *arg);
void* ldsdUnit_thread_container(void *arg);

void init_FUs();

#endif
