#include "CircularQ.h"
#include <iostream>
#include <assert.h>
#ifndef _VEHICLE_Q_
#define _VEHICLE_Q_
class Vehicle
{
    public:
        Vehicle(const std::string &plate, const uint32_t mileage)
        : m_plate(plate)
        , m_mileage(mileage)
        , position(0)
        {
        }

        Vehicle(){}

        std::string m_plate;
        uint32_t m_mileage;
        uint32_t position;
};

class VehicleQ: public xeyes::CircularQ<Vehicle*>
{
    public:

        virtual void allocQ(const uint32_t nTotItems) {
            std::lock_guard<std::mutex> lock(m_mutexRW);

            m_items = nTotItems;
            m_q.clear();
            for(uint32_t i=0;i<m_items;i++) {
                Vehicle* p = new Vehicle("null",0);
                m_q.push_back(p);
            }
            m_v.resize(m_items,0);
            m_headW = 0;
            m_headR = 0;
        }
};
#endif