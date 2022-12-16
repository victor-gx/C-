// lesson6.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <list>
#include <thread>
#include <mutex>

using namespace std;

class A
{
public:
    std::unique_lock<std::mutex> rtn_unique_lock()
    {
        std::unique_lock<std::mutex> tmpguard(my_mutex);
        return tmpguard; // 从函数返回一个局部 unique_lock 对象是可以的,返回这种局部对象tmpguard 会导致系统生成临时 unique_lock 对象,并调用 unique_lock 的移动构造函数
    }

    // 把收到的消息(玩家命令)放入到一个队列的线程，
    // //std::adopt_lock和std::try_to_lock
    //void inMsgRecvQueue() // unlock()
    //{
    //    for (int i = 0; i < 100000; ++i)
    //    {
    //        cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
    //        //std::lock_guard<std::mutex> sbguard1(my_mutex);
    //        //std::unique_lock<std::mutex> sbguard1(my_mutex);
    //        //my_mutex.lock();
    //        //std::unique_lock<std::mutex> sbguard1(my_mutex, std::adopt_lock);
    //        //msgRecvQueue.push_back(i); // 假设这个数字就是收到的命令,则把它直接放到消息队列里
    //        // ……
    //        // 其他处理代码

    //        std::unique_lock<std::mutex> sbguard1(my_mutex, std::try_to_lock);
    //        if (sbguard1.owns_lock()) // 条件成立表示拿到了锁头
    //        {
    //            // 拿到了锁头，离开sbguard1作用域锁头会自动释放
    //            msgRecvQueue.push_back(i);
    //            // ……
    //            // 其他处理代码
    //        }
    //        else
    //        {
    //            // 没拿到锁
    //            cout << "inMsgRecvQueue()执行,但没拿到锁,只能干点别的事" << i << endl;
    //        }
    //    }
    //    return;
    //}
    // //std::defer_lock
    //void inMsgRecvQueue()
    //{
    //    for (int i = 0; i < 100000; ++i)
    //    {
    //        cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
    //        std::unique_lock<std::mutex> sbguard1(my_mutex, std::defer_lock);
    //        sbguard1.lock(); // 反正 unique_lock 能自动解锁,不用自己解,所以这里只管加锁
    //        msgRecvQueue.push_back(i);
    //    }
    //    return;
    //}
    // //lock和unlock
    //void inMsgRecvQueue()
    //{
    //    for (int i = 0; i < 100000; ++i)
    //    {
    //        cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
    //        std::unique_lock<std::mutex> sbguard1(my_mutex, std::defer_lock);
    //        if (sbguard1.try_lock() == true) // 返回 true 表示拿到了锁,自己不用管 unlock 问题
    //        {
    //            msgRecvQueue.push_back(i);
    //        }
    //        else
    //        {
    //            cout << "抱歉,没拿到锁,做点别的事情吧!" << endl;
    //        }
    //    }
    //    return;
    //}
    ////try_lock
    //void inMsgRecvQueue()
    //{
    //    for (int i = 0; i < 100000; ++i)
    //    {
    //        cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
    //        std::unique_lock<std::mutex> sbguard1(my_mutex, std::defer_lock);
    //        if (sbguard1.try_lock() == true) // 返回 true 表示拿到了锁,自己不用管 unlock 问题
    //        {
    //            msgRecvQueue.push_back(i);
    //        }
    //        else
    //        {
    //            cout << "抱歉,没拿到锁,做点别的事情吧!" << endl;
    //        }
    //    }
    //    return;
    //}
    ////release
    //void inMsgRecvQueue()
    //{
    //    for (int i = 0; i < 100000; ++i)
    //    {
    //        cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
    //        std::unique_lock<std::mutex> sbguard1(my_mutex); // mutex 锁定
    //        std::mutex* p_mtx = sbguard1.release();          // 现在关联关系解除,程序员有责任自己解锁了,其实这个就是 my_mutex,现在sbguardl已经不和 my_mutex 关联了(可以设置断点并观察)
    //        msgRecvQueue.push_back(i);
    //        p_mtx->unlock(); // 因为前面已经加锁，所以这里要自己解锁了
    //    }
    //    return;
    //}
    ////unique_lock 所有权的传递
    //void inMsgRecvQueue()
    //{
    //    for (int i = 0; i < 100000; ++i)
    //    {
    //        cout << "inMsgRecvQueue()执行,插入一个元素" << i << endl;
    //        std::unique_lock<std::mutex> sbguard1(my_mutex);
    //        // std:: unique_lock< std:: mutex>sbguard10(sbguard1);//复制所有权,不可以
    //        std::unique_lock<std::mutex> sbguard10(std::move(sbguard1)); // 移动语义,现在 my_mutex和 sbguard10可以绑定到一起了.设置断点调试,移动后sbguard1指向空,sbguard10指向了该 my_mutex
    //        msgRecvQueue.push_back(i);
    //    }
    //    return;
    //}
    void inMsgRecvQueue()
    {
        for (int i = 0; i < 100000; ++i)
        {
            std::unique_lock<std::mutex> sbguard1 = rtn_unique_lock();
            msgRecvQueue.push_back(i);
        }
        return;
    }

    bool outMsgLULProc(int& command)
    {
        //std::lock_guard<std::mutex> sbguard1(my_mutex);
        std::unique_lock<std::mutex> sbguard1(my_mutex);
        //std::chrono::milliseconds dura(20000); //定义一个时间相关对象,初值2万,单位毫秒
        //std::this_thread::sleep_for(dura); //卡在这里2万毫秒(20s)
        if (!msgRecvQueue.empty())
        {
            // 消息不为空
            command = msgRecvQueue.front(); // 返回第一个元素，但不检查元素是否存在
            msgRecvQueue.pop_front();       // 移除第一个元素，但不返回
            return true;
        }
        return false;
    }

    // 把数据从消息队列中取出的线程
    void outMsgRecvQueue()
    {
        int command = 0;
        for (int i = 0; i < 100000; ++i)
        {
            bool result = outMsgLULProc(command);
            if (result == true)
            {
                cout << "outMsgRecvQueue()执行了,从容器中取出一个元素" << command << endl; // 可以考虑进行命令(数据)处理
                // ⋯⋯
            }
            else
            {
                // 消息对列为空
                cout << "outMsgRecvQueue()执行了,但目前收消息队列中是空元素" << i << endl;
            }
        }
        cout << "end" << endl;
    }

    

private:
    std::list<int> msgRecvQueue; // 容器(消息队列),专门用于代表玩家给咱们发送过来的命令
    std::mutex my_mutex;         // 创建了一个互斥量(一把锁头)
};
using namespace std;
 
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
