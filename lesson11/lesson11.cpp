// lesson11.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <windows.h>

using namespace std;

//#define _WINDOWSLJQ_ // 宏定义

// 本类用于自动释放Windows下的临界区,防止忘记LeaveCriticalSection的情况发生,类似于C++11中的 std:: lock_guard< std:: mutex>功能
class CWinLock
{
public:
    CWinLock(CRITICAL_SECTION* pCritSect) // 构造函数
    {
        m_pCritical = pCritSect;
        EnterCriticalSection(m_pCritical);
    }
    ~CWinLock() // 析构函数
    {
        LeaveCriticalSection(m_pCritical);
    }

private:
    CRITICAL_SECTION* m_pCritical;
};

class A
{
public:
    A()
    {
#ifdef _WINDOWSLJQ_
        InitializeCriticalSection(&my_winsec); // 初始化临界区
#endif
    }
    virtual ~A()
    {
#ifdef _WINDOWSLJQ_
        DeleteCriticalSection(&my_winsec); // 释放临界区
#endif
    }
    // 把收到的消息放入到队列的线程
//    void inMsgRecvQueue()
//    {
//        for (int i = 0; i < 100000; i++)
//        {
//            cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
//#ifdef _WINDOWSLJQ_
//            // EnterCriticalSection(& my_winsec); //进入临界区
//            // EnterCriticalSection(& my_winsec); //调用两次
//            CWinLock wlock(&my_winsec);
//            CWinLock wlock2(&my_winsec); // 调用多次也没问题
//            msgRecvQueue.push_back(i);
//            // LeaveCriticalSection(& my_winsec); //离开临界区
//            // LeaveCriticalSection(& my_winsec); //也要调用两次
//#else
//            // my_mutex.lock();
//            // my_mutex.lock(); //连续两次调用lock直接报异常
//            //std::lock_guard<std::mutex> sbguard(my_mutex);
//            std::lock_guard<std::recursive_mutex> sbguard(my_mutex);
//            //std::lock_guard<std::mutex> sbguard2(my_mutex); // 这个也一样报异常
//            msgRecvQueue.push_back(i);
//            testfunc1(); //悲剧了，异常.因为多次(超过1次)调用了lock
//            // my_mutex.unlock();
//            // my_mutex.unlock();
//#endif
//        }
//    }
    void inMsgRecvQueue()
    {
        for (int i = 0; i < 100000; i++)
        {
            std::chrono::milliseconds timeout(100);
            //if (my_mutex.try_lock_for(timeout)) // 尝试获取锁，这里只等100ms
            if (my_mutex.try_lock_until(chrono::steady_clock::now() + timeout)) // now:当前时间
            {
                // 在这100ms之内拿到了锁
                cout << "inMsgRecvQueue()执行,插入一个元素" << endl;
                msgRecvQueue.push_back(i);
                // 用完了，还要解锁
                my_mutex.unlock();
}
            else
            {
                // 这次没拿到锁就休息一下等待下次拿吧
                std::chrono::milliseconds sleeptime(100);
                std::this_thread::sleep_for(sleeptime);
            }
    }
}
    bool outMsgLULProc(int& command)
    {
#ifdef _WINDOWSLJQ_
        EnterCriticalSection(&my_winsec);
        if (!msgRecvQueue.empty())
        {
            int command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            LeaveCriticalSection(&my_winsec);
            return true;
        }
        LeaveCriticalSection(&my_winsec);
#else
        my_mutex.lock();
        //std::chrono::milliseconds sleeptime(100000000);
        //std::this_thread::sleep_for(sleeptime);
        if (!msgRecvQueue.empty())
        {
            command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            my_mutex.unlock();
            return true;
        }
        my_mutex.unlock();
#endif
        return false;
    }
    void outMsgRecvQueue()
    {
        int command = 0;
        for (int i = 0; i < 100000; i++)
        {
            bool result = outMsgLULProc(command);
            if (result == true)
            {
                cout << "outMsgRecvQueue()执行了,从容器中取出一个元素" << command << endl; // 这里可以考虑处理数据
                // ……
            }
            else
            {
                cout << "outMsgRecvQueue()执行了,但目前收消息队列中是空元素" << i << endl;
            }
        }
        cout << "end" << endl;
    }

    //void testfunc1()
    //{
    //    //std::lock_guard<std::mutex> sbguard(my_mutex);
    //    std::lock_guard<std::recursive_mutex> sbguard(my_mutex);
    //    // ⋯⋯做一些事
    //    testfunc2(); // 悲剧了，程序异常崩溃了
    //}

    //void testfunc2()
    //{
    //    //std::lock_guard<std::mutex> sbguard(my_mutex);
    //    std::lock_guard<std::recursive_mutex> sbguard(my_mutex);
    //    // ⋯⋯做另外一些事
    //}

private:
    std::list<int> msgRecvQueue; // 容器(收消息队列),专门用于代表玩家给咱们发送过来的命令
    //std::mutex my_mutex;         // 创建独占互斥量
    //std::recursive_mutex my_mutex;      //递归独占互斥量
    std::timed_mutex my_mutex;

#ifdef _WINDOWSLJQ_
    // Windows下叫临界区(类似于互斥量mutex)
    CRITICAL_SECTION my_winsec;
#endif
};

int main()
{
    A myobja;
    std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja);
    std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
    myInMsgObj.join();
    myOutnMsgObj.join();
    cout << "main主函数执行结束!" << endl;

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
