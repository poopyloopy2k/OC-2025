#include <windows.h>
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

// Глобальные переменные
vector<double> arr; // Массив вещественных чисел
double A; // Пороговое значение
int K1, K2; // Индексы для суммирования
int arraySize; // Размер массива
double sumResult = 0.0; // Результат суммирования
HANDLE hEventWorkToMain; // Событие с ручным сбросом для синхронизации work -> main
HANDLE hEventWorkToSum; // Событие с ручным сбросом для синхронизации work -> SumElement
CRITICAL_SECTION cs; // Критическая секция

// Функции потоков
DWORD WINAPI WorkThread(LPVOID lpParam);
DWORD WINAPI SumElementThread(LPVOID lpParam);

int main() {
    SetConsoleOutputCP(1251); 
    srand(static_cast<unsigned>(time(0)));

    // Инициализация объектов синхронизации
    hEventWorkToMain = CreateEvent(NULL, TRUE, FALSE, NULL);
    hEventWorkToSum = CreateEvent(NULL, TRUE, FALSE, NULL);
    InitializeCriticalSection(&cs);

    // Запрос размера массива и его заполнение
    cout << "Введите размер массива: ";
    cin >> arraySize;
    arr.resize(arraySize);

    cout << "Хотите ввести массив вручную (1) или сгенерировать случайные значения (0)? ";
    int choice;
    cin >> choice;
    if (choice == 1) {
        cout << "Введите " << arraySize << " элементов:\n";
        for (int i = 0; i < arraySize; i++) {
            cin >> arr[i];
        }
    }
    else {
        for (int i = 0; i < arraySize; i++) {
            arr[i] = (rand() % 1000) / 10.0;
        }
    }

    // Вывод исходного массива
    cout << "Исходный массив:\n";
    for (int i = 0; i < arraySize; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    
    cout << "Введите A: ";
    cin >> A;

    do {
        cout << "Введите K1 (0 < K1 < K2 < " << arraySize << "): ";
        cin >> K1;
        cout << "Введите K2: ";
        cin >> K2;
        if (K1 >= K2 || K2 >= arraySize || K1 <= 0) {
            cout << "Некорректные значения K1 или K2! Попробуйте снова.\n";
        }
    } while (K1 >= K2 || K2 >= arraySize || K1 <= 0);

    
    HANDLE hWorkThread = CreateThread(NULL, 0, WorkThread, NULL, 0, NULL);
    HANDLE hSumElementThread = CreateThread(NULL, 0, SumElementThread, NULL, 0, NULL);

    // Ожидание сигнала от work для вывода первых K2 элементов
    WaitForSingleObject(hEventWorkToMain, INFINITE);
    EnterCriticalSection(&cs);
    cout << "вход в кр. секцию потоком main" << endl;
    cout << "Основной поток: Вывод первых " << K2 << " элементов:\n";
    for (int i = 0; i < K2; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    LeaveCriticalSection(&cs);

    // Ожидание результата от SumElement
    EnterCriticalSection(&cs);
    cout << "вход в кр. секцию потоком main" << endl;
    cout << "Сумма элементов от K1 до K2: " << sumResult << endl;
    LeaveCriticalSection(&cs);

    // Ожидание завершения work и вывод оставшихся элементов
    WaitForSingleObject(hWorkThread, INFINITE);
    EnterCriticalSection(&cs);
    cout << "вход в кр. секцию потоком main" << endl;
    cout << "Основной поток: Вывод оставшихся элементов (с K2):\n";
    for (int i = K2; i < arraySize; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    LeaveCriticalSection(&cs);

    // Очистка ресурсов
    WaitForSingleObject(hSumElementThread, INFINITE);
    CloseHandle(hWorkThread);
    CloseHandle(hSumElementThread);
    CloseHandle(hEventWorkToMain);
    CloseHandle(hEventWorkToSum);
    DeleteCriticalSection(&cs);

    return 0;
}

DWORD WINAPI WorkThread(LPVOID lpParam) {
    int sleepInterval;
    cout << "Поток work: Введите интервал отдыха (мс): ";
    cin >> sleepInterval;

    vector<double> temp(arraySize, 0.0);
    int leftPos = 0;

    for (int i = 0; i < arraySize; i++) {
        if (arr[i] > A) {
            temp[leftPos++] = arr[i];
        }
        Sleep(sleepInterval);
    }

    EnterCriticalSection(&cs);
    cout << "вход в кр. секцию потоком work" << endl;
    arr = temp;
    LeaveCriticalSection(&cs);

    // Устанавливаем события после завершения обработки
    SetEvent(hEventWorkToMain); // Сигнал для main
    SetEvent(hEventWorkToSum); // Сигнал для SumElement
    cout << "выход из кр. секции потоком work" << endl;

    return 0;
}

DWORD WINAPI SumElementThread(LPVOID lpParam) {
    WaitForSingleObject(hEventWorkToSum, INFINITE);

    EnterCriticalSection(&cs);
    cout << "вход в кр. секцию потоком SumElement" << endl;
    sumResult = 0.0;
    for (int i = K1; i < K2; i++) {
        sumResult += arr[i];
    }
    cout << "выход из кр. секции потоком SumElement" << endl;
    LeaveCriticalSection(&cs);

    return 0;
}
