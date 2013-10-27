#include "msgQue.h"

msgQue::msgQue()
{

    buffer = new std::vector<RsJsonItem>;
}


void msgQue::storeMsg(RsJsonItem * item){

    //should be doing a mutex lock here?
    buffer->push_back(*item);
}


//std::list<std::string>
std::vector<RsJsonItem> *msgQue::getMsgList(){
    //should also be doing a mutex lock here?
    std::vector<RsJsonItem> *oldbuffer = buffer;
    buffer = new std::vector<RsJsonItem>;
    return oldbuffer;

}
