// parallel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <queue>

const int size = 11;

struct point {
    int front;
    int back;
};

std::queue<point*> backLog;
bool threadOneRun = true;
bool threadTwoRun = true;

//this runs on thread 2 and handles exchanges
void exchangeHandler(int arr[]) {
    while (threadOneRun || !backLog.empty()) {
        if (!backLog.empty()) {
            point* tempPoint = backLog.front();
            backLog.pop();

            //swap items
            int temp = arr[tempPoint->back];
            arr[tempPoint->back] = arr[tempPoint->front];
            arr[tempPoint->front] = temp;
        }
    }
    threadTwoRun = false;
}

//this runs on thread 1 and does twos pointers
void sort(int arr[]) {
    int p1 = 0;
    int p2 = size - 1;
    while (true) {
        if (p1 >= p2) {
            threadOneRun = false;
            return;
        }
        if (arr[p1] > arr[p2]) {
            //add this exchange to the backlog
            point* p = new point();
            p->front = p1;
            p->back = p2;
            backLog.push(p);

            p1++;
            p2--;
        }
        if (arr[p1] < arr[p2]) {
            p1++;
            p2--;
        }
        if (p1 == p2) {
            p1++;
        }
    }
}

int main()
{
    int arr[size] = { 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0 };

    std::thread t1(sort, arr);
    std::thread t2(exchangeHandler, arr);

    t1.join(); //note at this point we could change t1 to exchangeHandler if backlog ! empty
    t2.join();
    for (int i = 0; i < size; i++) {
        std::cout << " " << arr[i];
    }
    
}

