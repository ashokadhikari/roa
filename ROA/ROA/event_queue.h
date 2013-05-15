#ifndef EVENT_QUEUE
#define EVENT_QUEUE

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <queue>

using namespace std;

template <class event_t>
struct event_queue_t
{
private:
	queue<event_t> queue;
	mutable boost::mutex mutex;
	boost::condition condition;

public:
	bool empty() const
	{
		boost::mutex::scoped_lock lock(mutex);
		return queue.empty();
	}

	void push(const event_t &e)
	{
		boost::mutex::scoped_lock lock(mutex);
		queue.push(e);
		lock.unlock();
		condition.notify_one();
	}

	void pop(event_t& e)
	{
		boost::mutex::scoped_lock lock(mutex);
		while(queue.empty()) condition.wait(lock);
		e = queue.front();
		queue.pop();
	}

};

#endif