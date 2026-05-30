#include "CEventBus.h"
#include "CCollisionManager.h"
#include "CCollisionEventManager.h"
#include "CTriggerEventManager.h"

IMPLEMENT_SINGLETON(CEventBus)

CEventBus::CEventBus()
{

}

CEventBus::~CEventBus()
{
    Free();
}

void CEventBus::Delete_Event()
{
    for (size_t ph = 0; ph < PHASE_TYPE_COUNT; ++ph)
    {
        auto& vec = m_vChannelTopicList[ph];

        for (auto& channelMap : vec) // CHANNEL_TOPIC
        {
            for (auto& [channel, topicArray] : channelMap) // TOPIC_EVENT
            {
                for (auto& q : topicArray) // queue<EVENT>
                {
                    std::queue<EVENT> empty;
                    std::swap(q, empty); // queue 비우기
                }
            }
            channelMap.clear();
        }

        vec.clear();
    }
}

void CEventBus::Free()
{
    for (size_t ph = 0; ph < PHASE_TYPE_COUNT; ++ph)
    {
        auto& vec = m_vChannelTopicList[ph];

        for (auto& channelMap : vec) // CHANNEL_TOPIC
        {
            for (auto& [channel, topicArray] : channelMap) // TOPIC_EVENT
            {
                for (auto& q : topicArray) // queue<EVENT>
                {
                    std::queue<EVENT> empty;
                    std::swap(q, empty); // queue 비우기
                }
            }
            channelMap.clear();
        }

        vec.clear();
    }
}