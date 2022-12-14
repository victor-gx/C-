// lesson4.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <vector>
#include <list>

using namespace std;

//void myprint(int inum)
//{
//    cout << "myprint线程开始执行了,线程编号=" << inum << endl;
//    // 干各种事情
//    cout << "myprint线程结束执行了,线程编号=" << inum << endl;
//    return;
//}

//vector<int> g_v = { 1, 2, 3 };

//void myprint(int inum)
//{
//    cout << "id 为" << std::this_thread::get_id() << "的线程打印g_v值" << g_v[0] << g_v[1] << g_v[2] << endl;
//    return;
//}

class A
{
public:
    // 把收到的消息入到队列的线程
    void inMsgRecvQueue()
    {
        for (int i = 0; i < 100000; i++)
        {
            cout << "inMsgRecvQueue()执行,插入一个元素" << endl;
            msgRecvQueue.push_back(i); // 假设这个数字就是收到的命令,则将其直接放到消息队列里
        }
    }
    // 把数据从消息队列中取出的线程
    void outMsgRecvQueue()
    {
        for (int i = 0; i < 100000; i++)
        {
            if (!msgRecvQueue.empty())
            {
                int command = msgRecvQueue.front(); // 返回第一个元素但不检查元素存在与否
                msgRecvQueue.pop_front();           // 移除第一个元素但不返回
                // 这里可以考虑处理数据
                // ……
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
};

int main()
{
    //一、创建和等待多个线程
    //vector<thread> mythreads;
    //// 创建5个线程。当然，线程的入口函数可以用同一个，这并没什么问题
    //for (int i = 0; i < 5; i++)
    //{
    //    mythreads.push_back(thread(myprint, i)); // 创建并开始执行线程
    //}
    //for (auto iter = mythreads.begin(); iter != mythreads.end(); ++iter)
    //{
    //    iter->join(); // 等待5个线程都返回
    //}
    //cout << "main主函数执行结束!" << endl; // 最后执行这句,然后整个进程退出

    //二、数据共享问题分析
    //vector<thread> mythreads;
    //// 创建5个线程。当然，线程的入口函数可以用同一个，这并没什么问题
    //for (int i = 0; i < 5; i++)
    //{
    //    mythreads.push_back(thread(myprint, i)); // 创建并开始执行线程
    //}
    //for (auto iter = mythreads.begin(); iter != mythreads.end(); ++iter)
    //{
    //    iter->join(); // 等待5个线程都返回
    //}
    //cout << "main主函数执行结束!" << endl; // 最后执行这句,然后整个进程退出

    //三、共享数据的保护实战范例
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
