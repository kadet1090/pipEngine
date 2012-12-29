#ifndef LTC_H
#define LTC_H

#include <iostream>
#include <vector>
#include <functional>
#include <time.h>

#define THREAD_FUNC bool

#define THREAD_START \
	for (unsigned int i = 0; i < this->children.size(); i++) \
	{ \
		this->children[i]->run(); \
		if(!this->children[i]->isAlive) \
		{ \
			std::cout << "Die bastard!" << std::endl;\
			delete this->children[i]; \
			this->children.erase(this->children.begin() + i); \
		} \
	} \
	if(gotoPtr) goto *gotoPtr;

#define SCHELUDE(no) \
		gotoPtr = &&label##no##x; \
		return true; \
		label##no##x:

#define WAIT(no, sec) \
	startTime = time(0); \
	while (true) \
	{ \
		if (time(0) < (startTime + (sec))) \
		{ \
			SCHELUDE(no); \
		} \
		else \
		{ \
			startTime = time(0); \
			break; \
		} \
	}

#define WAITFUNC(no, obj, sec)\
		{\
		startTime = time(0);\
		while (true)\
		{\
			if (time(0) <= (startTime + (sec)))\
			{\
				obj->WaitFunc();\
				SCHELUDE(no);\
			}\
			else\
			{\
				startTime = time(0);\
				break;\
			}\
		}\
		}

#define THREAD_END \
	this->isAlive = false; \
	for (unsigned int i = 0; i < this->children.size(); i++) \
	{ \
		delete this->children[i]; \
	} \
	return false;

class LightThread;

typedef std::vector<LightThread* > ThreadsPool;


class LightThread
{
public:
	LightThread()
	{
		startTime = 0;
		gotoPtr = 0;
		isAlive = true;
	}
	virtual ~LightThread() {}
	virtual THREAD_FUNC run() {THREAD_END;}
	int startTime;
	void *gotoPtr;
	bool isAlive;
	ThreadsPool children;
};

void add(LightThread* thread, ThreadsPool &threadsPool)
{
	threadsPool.push_back(thread);
}

void run(ThreadsPool &threadsPool)
{
	for (unsigned int i = 0; i < threadsPool.size(); i++)
	{
		threadsPool[i]->run();

		if(!threadsPool[i]->isAlive)
			threadsPool.erase(threadsPool.begin() + i);
	}
}

#endif
