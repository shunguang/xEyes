#ifndef _FIFO_Q_H_
#define _FIFO_Q_H_

#include <iostream>
#include "UtilDefs.h"
namespace xeyes {
	template <class T>
	class UTIL_EXPORT FifoQ
	{
	private:
		int					the_qsize;
		std::string			the_name;                 //for debug purpose
		std::queue<T>		the_queue;
		mutable boost::mutex	the_mutex;
		boost::condition_variable the_condition_variable;
	public:
		FifoQ(const int qSize_ = 10, const std::string &name_ = "FifoQ") : the_qsize(qSize_), the_name(name_)
		{
		}

		bool isFull() {
			int s = size();
			if (s >= the_qsize) {
				return true;
			}
			else {
				return false;
			}
		}

		int size() {
			int s = 0;
			{
				boost::mutex::scoped_lock lock(the_mutex);
				s = the_queue.size();
			}
			return s;
		}

		bool empty() const
		{
			boost::mutex::scoped_lock lock(the_mutex);
			return the_queue.empty();
		}

		void clear()
		{
			boost::mutex::scoped_lock lock(the_mutex);
			while(!the_queue.empty()){
				the_queue.pop();
			}
		}

		void push( const T &it)
		{
			boost::mutex::scoped_lock lock(the_mutex);
			the_queue.push(it);
			the_condition_variable.notify_one();
		}

		bool try_pop(T& popped_value)
		{
			boost::mutex::scoped_lock lock(the_mutex);
			if (the_queue.empty()){
				return false;
			}

			popped_value = the_queue.front();
			the_queue.pop();
			return true;
		}

		void wait_and_pop(T& popped_value)
		{
			boost::mutex::scoped_lock lock(the_mutex);
			while (the_queue.empty())
			{
				the_condition_variable.wait(lock);
			}

			popped_value = the_queue.front();
			the_queue.pop();
		}
	};
}
#endif
