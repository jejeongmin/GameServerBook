#pragma once 

// https://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads 의 소스를 기반하여 만들어졌습니다.

#include <mutex>
#include <condition_variable>

/*
	windows 환경에 국한된다면 system api 로 semaphore 를 지원하니까 직접 만들 필요가 없지만,
	크로스 플랫폼 환경이면 아래 구현이 유용할 듯 싶다.

	condition_variable. event 대신 쓰기 좋은 자료형이네.
	https://m.blog.naver.com/jidon333/220182028669
*/
class Semaphore {
public:
	Semaphore(int count_ = 0)
		: count(count_) {}

	inline void Notify()
	{
		std::unique_lock<std::mutex> lock(mtx);
		count++;
		cv.notify_one();
	}

	inline void Wait()
	{
		std::unique_lock<std::mutex> lock(mtx);

		while (count == 0) {
			cv.wait(lock);
		}
		count--;
	}

private:
	std::mutex mtx;
	std::condition_variable cv;
	int count;
};