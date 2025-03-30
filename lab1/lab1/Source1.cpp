//var 13
#include <iostream>
#include <vector>
#include <windows.h>
struct WorkerData
{
	std::vector<char> charList;
	DWORD sleep_ms;
};
DWORD __stdcall worker_thread_func(LPVOID arg)
{
	WorkerData* data = (WorkerData*)arg;
	for (char elem : data->charList)
	{
		if (elem < '0' || elem > '9')
		{
			std::cout << elem << " ";
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
	WorkerData* data = new WorkerData{ array, sleep_ms };

	HANDLE worker = CreateThread(
		NULL,
		0,     //1 mb default
		worker_thread_func,
		data,
		CREATE_SUSPENDED, // flow state
		NULL
	);
	ResumeThread(worker);

	WaitForSingleObject(worker, INFINITE);

	CloseHandle(worker);

}