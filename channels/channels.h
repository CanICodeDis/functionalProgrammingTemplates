#ifndef _CHANNELS_H_
#define _CHANNELS_H_
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

namespace ipc {
template <typename ValueType>
class channel {
private:
    std::queue<ValueType> fifo;
    std::mutex key;
    std::condition_variable non_empty;

public:
    void send(const ValueType& T_tosend)
    {
	std::lock_guard<std::mutex> guard(key);
	fifo.push(T_tosend);
	non_empty.notifiy_all();
    }

    bool tryReceive(ValueType& T_toreceive)
    {
	bool received = false;
	if (key.try_lock()) {
	    std::lock_guard<std_mutex> guard(key, std::adopt_lock);
	    if (!fifo.empty()) {
		T_toreceive = fifo.front();
		fifo.pop();
		received = true;
	    }
	}
	return received;
    }

    void receive(ValueType& T_toreceive)
    {
	std::lock_guard<std_mutex> guard(key);
	non_empty.wait(key, [this] { return !fifo.empty(); })
	T_toreceive  = fifo.front();
	fifo.pop();
    }
}; ///end class channel

} //end namespace ipc
#endif
