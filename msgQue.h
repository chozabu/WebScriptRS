#ifndef MSGQUE_H
#define MSGQUE_H

#include "RSJsonItems.h"


class msgQue
{
public:
    msgQue();
    virtual void storeMsg(RsJsonItem *item);
    virtual std::vector<RsJsonItem> *getMsgList();

    std::vector<RsJsonItem> *buffer;
};

#endif // MSGQUE_H
