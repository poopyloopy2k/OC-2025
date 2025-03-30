#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void runChildProcess(const wstring& commandLine) {
    STARTUPINFO si;              
    PROCESS_INFORMATION pi;      

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;  
    si.wShowWindow = SW_SHOW;
    si.lpTitle = (LPWSTR)L"Child Process Window"; 

    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(
        NULL,                       // Имя исполняемого файла
        (LPWSTR)commandLine.c_str(), // Командная строка
        NULL,                       // Атрибуты защиты процесса
        NULL,                       // Атрибуты защиты потока
        FALSE,                      // Наследование дескрипторов
        0,                          // Флаги создания процесса
        NULL,                       // Указатель на блок окружения
        NULL,                       // Текущий каталог
        &si,                        // Указатель на структуру STARTUPINFO
        &pi                         // Указатель на структуру PROCESS_INFORMATION
    )) {
        wcerr << L"Failed to create child process. Error: " << GetLastError() << endl;
        return;
    }


    WaitForSingleObject(pi.hProcess, INFINITE);
    
   
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    int size;
    vector<__int16> array;

    wcout << L"Enter the size of the array: ";
    cin >> size;

    wcout << L"Enter " << size << L" elements (__int16): ";
    for (int i = 0; i < size; ++i) {
        __int16 num;
        cin >> num;
        array.push_back(num);
    }

 
    wstring commandLine = L"Child.exe " + to_wstring(size);
    for (const auto& num : array) {
        commandLine += L" " + to_wstring(num);
    }

    
    runChildProcess(commandLine);
   

    return 0;
}