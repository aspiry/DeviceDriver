#include "device_driver.h"
#include <stdexcept>
DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}
int DeviceDriver::read(long address) 
{
    // TODO: implement this method properly
    int ret = 0; 
    int before_read  = (int)(m_hardware->read(address));
    for (int i = 0; i < 4; i++)
    {
        ret = (int)(m_hardware->read(address));
        if(ret != before_read)
            throw std::runtime_error("Different Val."); 
    }
    return ret;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    if (read(address) == 0xFF)
    {
        //write
        m_hardware->write(address, (unsigned char)data);
    }
    else
        throw std::runtime_error("already writed");
}