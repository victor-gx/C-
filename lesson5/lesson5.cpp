// lesson5.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>

using namespace std;

class A
{
public:
    //一、互斥量的用法
    //// 把收到的消息入到队列的线程
    //void inMsgRecvQueue()
    //{
    //    for (int i = 0; i < 100000; i++)
    //    {
    //        cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
    //        //my_mutex.lock(); // 要操作共享数据，所以先加锁
    //        msgRecvQueue.push_back(i);
    //        my_mutex.unlock(); // 共享数据操作完毕，解锁
    //    }
    //    return;
    //}

    //bool outMsgLULProc(int& command)
    //{
    //    std::lock_guard<std::mutex> sbguard(my_mutex); // sbguard 是随便起的变量名
    //    // my_mutex.lock( );
    //    if (!msgRecvQueue.empty())
    //    {
    //        command = msgRecvQueue.front(); // 返回第一个元素但不检查元素存在与否
    //        msgRecvQueue.pop_front();
    //        // my_mutex.unlock( );
    //        return true;
    //    }
    //    // my_mutex.unlock( );
    //    return false;
    //}

    //// 把数据从消息队列中取出的线程
    //void outMsgRecvQueue()
    //{
    //    int command = 0;
    //    for (int i = 0; i < 100000; i++)
    //    {
    //        bool result = outMsgLULProc(command);
    //        if (result == true)
    //        {
    //            cout << "outMsgRecvQueue()执行了,从容器中取出一个元素" << command << endl;
    //            // 这里可以考虑处理数据
    //            //  ……
    //        }
    //        else
    //        {
    //            cout << "outMsgRecvQueue()执行了,但目前收消息队列中是空元素" << i << endl;
    //        }
    //    }
    //    cout << "end" << endl;
    //}

    //二、死锁演示
    //void inMsgRecvQueue()
    //{
    //    for (int i = 0; i < 100000; i++)
    //    {
    //        cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
    //        my_mutex.lock(); // 两行 lock( )代码不一定紧挨着,可能它们要保护不同的数据共享块
    //        // ……需要保护的一些共享数据
    //        my_mutex2.lock();
    //        msgRecvQueue.push_back(i);
    //        my_mutex2.unlock();
    //        my_mutex.unlock();
    //    }
    //}
    //void inMsgRecvQueue()
    //{
    //    for (int i = 0; i < 100000; i++)
    //    {
    //        cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
    //        //std::lock_guard<std::mutex> sbguard1(my_mutex);
    //        //std::lock_guard<std::mutex> sbguard2(my_mutex2);
    //        std::lock(my_mutex, my_mutex2); // 相当于每个互斥量都调用了lock
    //        msgRecvQueue.push_back(i);
    //        my_mutex2.unlock(); // 前面锁住2个，后面就得解锁2个
    //        my_mutex.unlock();
    //    }
    //}
    //std::lock_guard 的 std::adopt_lock 参数
    void inMsgRecvQueue()
    {
        for (int i = 0; i < 100000; i++)
        {
            cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
            std::lock(my_mutex, my_mutex2);
            std::lock_guard<std::mutex> sbguard1(my_mutex, std::adopt_lock);
            std::lock_guard<std::mutex> sbguard2(my_mutex2, std::adopt_lock);
            msgRecvQueue.push_back(i);
        }
    }

    //bool outMsgLULProc(int& command)
    //{
    //    my_mutex2.lock();
    //    my_mutex.lock();
    //    if (!msgRecvQueue.empty())
    //    {
    //        command = msgRecvQueue.front();
    //        msgRecvQueue.pop_front();
    //        my_mutex.unlock();
    //        my_mutex2.unlock();
    //        return true;
    //    }
    //    my_mutex.unlock();
    //    my_mutex2.unlock();
    //    return false;
    //}
    bool outMsgLULProc(int& command)
    {
        //std::lock_guard<std::mutex> sbguard1(my_mutex);
        //std::lock_guard<std::mutex> sbguard2(my_mutex2);
        std::lock(my_mutex2, my_mutex); // 两个顺序谁在前谁在后无所谓
        if (!msgRecvQueue.empty())
        {
            command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            my_mutex.unlock();
            my_mutex2.unlock();
            return true;
        }
        my_mutex.unlock();
        my_mutex2.unlock();
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
                cout << "outMsgRecvQueue()执行了,从容器中取出一个元素" << command << endl;
                // 这里可以考虑处理数据
                //  ……
            }
            else
            {
                cout << "outMsgRecvQueue()执行了,但目前收消息队列中是空元素" << i << endl;
            }
        }
        cout << "end" << endl;
    }

private:
    std::list<int> msgRecvQueue; // 容器(收消息队列),专门用于代表玩家给咱们发送过来的命令
    std::mutex my_mutex; //创建互斥量
    std::mutex my_mutex2; //创建互斥量
};

int main()
{
    A myobja;
    std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja); // 注意这里第二个参数必须是引用(用//std：：ref也可以)，才能保证线程里用的是同一个对象(上一节详细分析过了)
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
