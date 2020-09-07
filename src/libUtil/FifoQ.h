/*
*------------------------------------------------------------------------
*FifoQ.h
*
* This code was developed by Shunguang Wu in his spare time. No government
* or any client funds were used.
*
*
* THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
*
* IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTOR BE LIABLE FOR
* ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
* OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
* WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
* LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
* OF THIS SOFTWARE.
*
* Permission to use, copy, modify, distribute, and sell this software and
* its documentation for any purpose is prohibited unless it is granted under
* the author's written notice.
*
* Copyright(c) 2020 by Shunguang Wu, All Right Reserved
*-------------------------------------------------------------------------
*/
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
