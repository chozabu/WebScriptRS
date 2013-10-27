#ifndef RSJSONITEMS_H
#define RSJSONITEMS_H

#include "serialiser/rsserial.h"
#include <string>



const uint8_t QOS_PRIORITY_RS_JSON = 3;
extern const uint16_t RS_SERVICE_TYPE_JSON_PLUGIN;
extern const uint32_t CONFIG_TYPE_JSON_PLUGIN;


class RsJsonItem: public RsItem
{
public:
    RsJsonItem( )
        : RsItem( RS_PKT_VERSION_SERVICE,RS_SERVICE_TYPE_JSON_PLUGIN, JSON_ITEM )
    {
        setPriorityLevel(QOS_PRIORITY_RS_JSON);
        m_msg = "Hello World";
    }
    RsJsonItem(void *data, uint32_t pktsize);

    enum RS_PKT_SUBTYPE {
        JSON_ITEM = 0x01

        // add any number of item types here
    };

    virtual ~RsJsonItem() {};
    virtual void clear() {};
    virtual std::ostream& print(std::ostream &out, uint16_t indent = 0);

    virtual bool serialise(void *data,uint32_t& size);
    virtual uint32_t serial_size() const;

    const std::string & getMessage(){ return m_msg; }
    void setMessage(std::string msg){ m_msg = msg; }
private:
    std::string m_msg;
};





class RsJsonSerialiser: public RsSerialType
{
public:
    RsJsonSerialiser()
        :RsSerialType(RS_PKT_VERSION_SERVICE, RS_SERVICE_TYPE_JSON_PLUGIN)
    {
    }
    virtual ~RsJsonSerialiser() {}

    virtual uint32_t 	size (RsItem *item)
    {
        return dynamic_cast<RsJsonItem *>(item)->serial_size() ;
    }

    virtual	bool serialise  (RsItem *item, void *data, uint32_t *size)
    {
        return dynamic_cast<RsJsonItem *>(item)->serialise(data,*size) ;
    }
    virtual	RsItem *deserialise(void *data, uint32_t *size);
};

#endif // RSJSONITEMS_H
