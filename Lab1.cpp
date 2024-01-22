#include <iostream>
#include <thread> //потоки
#include <mutex>
#include <condition_variable>
#include<chrono>

using namespace std;

condition_variable cond1;
mutex lock1;
int ready = 0;

void producer()
{
	for (int i = 0; i < 7; i++)
	{
		lock1.lock();
		if (ready == 1)
		{
			lock1.unlock();
			i -= 1;
			continue;
		}
		this_thread::sleep_for(chrono::milliseconds(1000)); //Sleep(1000);
		ready = 1;
		cout << "Message provided " << i << endl;
		lock1.unlock();
		cond1.notify_one(); //Оповещаем один ожидающий поток, что данные готовы
	}
}

void consumer()
{
	for (int i = 0; i < 7; ++i)
	{
		unique_lock<mutex> lock(lock1);
		cond1.wait(lock, [] { return ready; });
		ready = 0;
		cout << "Message consumed " << i << endl;
		cond1.notify_one();
	}
}

int main()
{
	thread producerThread(producer), consumerThread(consumer);

	producerThread.join();
	consumerThread.join();

	return 0;
}
