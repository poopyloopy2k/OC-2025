//var 13
#include <iostream>
#include <vector>
#include <windows.h>
#include <process.h>
struct WorkerData
{
	std::vector<char> charList;
	DWORD sleep_ms;
};
unsigned __stdcall worker_thread_beginthreadex(void* arg)
{
	WorkerData* data = (WorkerData*)arg;
	for (char elem : data->charList)
	{
		if (elem < '0' || elem > '9')
		{
			std::cout <<"[beginthreadex] " <<  elem << " ";
		}
		Sleep(data->sleep_ms);
	}
	delete data;
	return 0;
}
DWORD __stdcall worker_thread_create(LPVOID arg)
{
	WorkerData* data = (WorkerData*)arg;
	for (char elem : data->charList)
	{
		if (elem < '0' || elem > '9')
		{
			std::cout <<"[create elem] " << elem << " ";
		}
		Sleep(data->sleep_ms);
	}
	delete data;
	return 0;
}
int main()
{
	std::cout << "Input array size ";
	int size;
	std::cin >> size;
	std::vector<char> array(size);
	std::cout << "Input elements of Array ";
	for (int i = 0; i < size; ++i)
	{
		std::cin >> array[i];
	}

	DWORD sleep_ms;
	std::cout << "Input sleep of thread ";
	std::cin >> sleep_ms;
	WorkerData* data1 = new WorkerData{ array, sleep_ms };
	WorkerData* data2 = new WorkerData{ array, sleep_ms };

	HANDLE worker = CreateThread(
		NULL,
		0,     //1 mb default
		worker_thread_create,
		data1,
		CREATE_SUSPENDED, // flow state
		NULL
	);

	uintptr_t worker2 = _beginthreadex(
		NULL,
		0,
		worker_thread_beginthreadex,
		data2,
		CREATE_SUSPENDED,
		NULL
	);
	if (worker == NULL || worker2 == 0) {
		std::cerr << "Failed to create threads" << std::endl;
		delete data1;
		delete data2;
		return 1;
	}
	ResumeThread(worker);
	HANDLE	hWorker2 = (HANDLE)worker2;
	ResumeThread(hWorker2);

	WaitForSingleObject(worker, INFINITE);
	WaitForSingleObject(hWorker2, INFINITE);

	CloseHandle(worker);
	CloseHandle(hWorker2);

}