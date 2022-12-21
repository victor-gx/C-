// lesson8.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <list>
#include <mutex>

using namespace std;

class A
{
public:
    // 把收到的消息(玩家命令)放入到一个队列的线程
    //void inMsgRecvQueue()
    //{
    //    for (int i = 0; i < 100000; ++i)
    //    {
    //        cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
    //        std::unique_lock<std::mutex> sbguard1(my_mutex);
    //        msgRecvQueue.push_back(i); // 假设这个数字就是收到的命令,则将其直接放到消息//队列里
    //        // ……
    //        // 其他处理代码
    //    }
    //    return;
    //}
    void inMsgRecvQueue()
    {
        for (int i = 0; i < 100000; ++i)
        {
            cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
            std::unique_lock<std::mutex> sbguard1(my_mutex);
            msgRecvQueue.push_back(i);
            //my_cond.notify_one(); // 尝试把卡(堵塞)在 wait( )的线程唤醒,但仅唤醒了还不够,这里必须把互斥量解锁，另外一个线程的wait()才会继续正常工作
            my_cond.notify_all();
        }
        return;
    }

    bool outMsgLULProc(int& command)
    {
        if (!msgRecvQueue.empty()) // 不为空
        {
            std::unique_lock<std::mutex> sbguard1(my_mutex);
            if (!msgRecvQueue.empty())
            {
                // 消息不为空
                command = msgRecvQueue.front(); // 返回第一个元素，但不检查元素是否存在
                msgRecvQueue.pop_front();       // 移除第一个元素，但不返回
                return true;
            }
        }
        return false;
    }

    // 把数据从消息队列中取出的线程
    //void outMsgRecvQueue()
    //{
    //    int command = 0;
    //    for (int i = 0; i < 100000; ++i)
    //    {
    //        bool result = outMsgLULProc(command);
    //        if (result == true)
    //        {
    //            cout << "outMsgRecvQueue()执行了,从容器中取出一个元素" << command << endl;
    //            // 可以考虑进行命令(数据)处理
    //            // ……
    //        }
    //        else
    //        {
    //            // 消息对列为空
    //            cout << "outMsgRecvQueue()执行了,但目前收消息队列中是空元素" << i << endl;
    //        }
    //    }
    //    cout << "end" << endl;
    //}
    //void outMsgRecvQueue()
    //{
    //    int command = 0;
    //    while (true)
    //    {
    //        std::unique_lock<std::mutex> sbguard1(my_mutex); 
    //        // 临界进去wait()用于等一个东西
    //        // 如果wait()第二个参数的1ambda表达式返回的是true, wait就直接返回
    //        // 如果wait()第二个参数的1ambda表达式返回的是false,那么wait()将解锁互斥量并堵
    //        // 塞到这行。那堵到什么时候为止呢?堵到其他某个线程调用 notify_one( )通知为止
    //        // 如果wait()不用第二个参数，那跟第二个参数为1ambda表达式并且返回false效果一样
    //        //(解锁互斥量并堵塞到这行，堵到其他某个线程调用 notify_one( )通知为止)
    //        my_cond.wait(sbguard1, [this] {
    //            if (!msgRecvQueue.empty())
    //                return true;
    //            return false; });
    //        // 一会再写其他的⋯⋯
    //    } // end while
    //}
    void outMsgRecvQueue()
    {
        int command = 0;
        while (true)
        {
            std::unique_lock<std::mutex> sbguard1(my_mutex);
            my_cond.wait(sbguard1, [this] {
                if (!msgRecvQueue.empty())
                    return true;
                return false; });
            // 现在互斥量是锁着的，流程走下来意味着msgRecvQueue队列里必然有数据
            command = msgRecvQueue.front(); // 返回第一个元素，但不检查元素是否存在
            msgRecvQueue.pop_front();       // 移除第一个元素，但不返回
            cout << "outMsgRecvQueue()执行,取出一个元素" << command << " threadid = " << std::this_thread::get_id() << endl;
            sbguard1.unlock();              // 因为unique_lock的灵活性,可以随时unlock解锁,以免锁住太长时间
        } // end while
    }

private:
    std::list<int> msgRecvQueue; // 容器(消息队列),专门用于代表玩家给咱们发送过来的命令
    std::mutex my_mutex;         // 创建了一个互斥量(一把锁头)
    std::condition_variable my_cond; // 生成一个条件对象
};

int main()
{
    A myobja;
    std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja); // 第二个参数是引用,才能保证线程里用的是同一个对象
    std::thread myOutnMsgObj2(&A::outMsgRecvQueue, &myobja);
    std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
    myInMsgObj.join();
    myOutnMsgObj2.join();
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
