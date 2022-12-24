// lesson10.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <mutex>
#include <future>

using namespace std;

//int mythread()
//{
//    cout << "mythread() start"
//        << " threadid=" << std::this_thread::get_id() << endl;
//    // 新的线程id
//    std::chrono::milliseconds dura(2000); // 1s=1000ms,所以5000ms = 5s
//    std::this_thread::sleep_for(dura);    // 休息一定的时长
//    cout << "mythread() end"
//        << " threadid=" << std::this_thread::get_id() << endl;
//    return 5;
//}

//int mythread(int mypar)
//{
//    cout << "mythread() start" << " threadid=" << std::this_thread::get_id() << endl;
//    std::chrono::milliseconds dura(5000); // 1s= 1000ms,所以5000ms= 5s
//    std::this_thread::sleep_for(dura);    // 休息一定的时长
//    return 5;
//}
//
//void mythread2(std::future<int>& tmpf) // 注意参数
////void mythread2(std::shared_future<int>& tmpf) // 注意参数
//{
//    cout << "mythread2() start" << " threadid=" << std::this_thread::get_id() << endl;
//    auto result = tmpf.get(); // 获取值，只能get一次否则会报异常
//    cout << "mythread2 result = " << result << endl;
//    return;
//}

//int g_mycout = 0; // 定义了一个全局量
//std::mutex g_my_mutex; // 创建一个全局互斥量
//std::atomic<int> g_mycout = 0; // 这是一个原子整型类型变量;可以像使用整型变量一样使用
//void mythread()
//{
//    for (int i = 0; i < 10000000; i++) // 1000万
//    {
//        //g_my_mutex.lock();
//        g_mycout++;
//        //g_my_mutex.unlock();
//    }
//    return;
//}

std::atomic<bool> g_ifend = false;//线程退出标记,用原子操作,防止读和写混乱
void mythread()
{
    std::chrono::milliseconds dura(1000);
    while (g_ifend == false) // 不断的读
    {
        // 系统没要求线程退出，所以本线程可以干自己想干的事情
        cout << "thread id=" << std::this_thread::get_id() << "运行中…" << endl;
        std::this_thread::sleep_for(dura); // 每次休息1s
    }
    cout << "thread id=" << std::this_thread::get_id() << "运行结束!" << endl;
    return;
}

int main()
{
    //cout << "main" << " threadid=" << std::this_thread::get_id() << endl;
    //std::future<int> result = std::async(mythread); // 流程并不会卡在这里
    //cout << "continue……!" << endl;
    ////cout << result.get() << endl; // 卡在这里等待线程执行完,但是这种get因为一些内部特殊操作(移动操作)，不能get多次，只能get一次
    ////  future_status 看成一个枚举类型
    //std::future_status status = result.wait_for(std::chrono::seconds(3)); // 等待1秒。注意写法,但如果 async的第一个参数用了std::launch::deferred,则这里是不会做任何等待的，因为线程根本没启动(延迟)
    //if (status == std::future_status::timeout)
    //{
    //    // 超时线程还没执行完
    //    cout << "超时线程没执行完!" << endl;
    //    cout << result.get() << endl; // 没执行完这里也要求卡在这里等线程返回
    //}
    //else if (status == std::future_status::ready)
    //{
    //    // 线程成功返回
    //    cout << "线程成功执行完毕并返回!" << endl;
    //    cout << result.get() << endl;
    //}
    //else if (status == std::future_status::deferred)
    //{
    //    // 如果async的第一个参数被设置为std::launch::deferred,则本条件成立
    //    cout << "线程被延迟执行!" << endl;
    //    cout << result.get() << endl; // 上一节说过,这会导致在主线程中执行了线程入口函数
    //}
    //cout << "main主函数执行结束!" << endl;

    //cout << "main" << " threadid=" << std::this_thread::get_id() << endl;
    //std::packaged_task<int(int)> mypt(mythread); // 把函数 mythread 通过 packaged_task 包装起来
    //std::thread t1(std::ref(mypt), 1);           // 线程直接开始执行,第二个参数作为线程入口函数的参数
    //t1.join(); //调用这个等待线程执行完毕，不调用这个不行，程序会崩溃
    //std::future<int> result = mypt.get_future();
    //// valid,判断 future对象里面的值是否有效
    //bool ifcanget = result.valid(); // 没有被get过表示能通过 get获取,则这里返回true
    //// auto mythreadresult = result. get();//获取值,只能get一次否则会报异常
    ////  ifcanget = result. valid( );//future对象get过了,里面的值就没了,这时就返回false
    //std::shared_future<int> result_s(std::move(result)); // std:: move( result)也可以替换成result. share(),在没针对result调用get时,把result的内容弄到 shared_future 中来,此时 future 中空了
    //ifcanget = result.valid();                           // 因为result 中空了,所以ifcanget为false了,这时不能再用result内容
    //ifcanget = result_s.valid();                         // 因为 result_s里有内容了,所以 ifcanget 为 true 了
    //auto mythreadresult = result_s.get();
    //mythreadresult = result_s.get(); // 可以调用多次，没有问题
    //std::thread t2(mythread2, std::ref(result_s));
    //t2.join(); // 等线程执行完毕
    //cout << "main主函数执行结束!" << endl;
    
    //cout << "main" << " threadid=" << std::this_thread::get_id() << endl;
    //thread mytobj1(mythread);
    //thread mytobj2(mythread);
    //mytobj1.join();
    //mytobj2.join();
    //cout << "两个线程都执行完毕,最终的g_mycout 的结果是" << g_mycout << endl;
    //cout << "main主函数执行结束!" << endl;

    cout << "main" << " threadid= " << std::this_thread::get_id() << endl;
    thread mytobj1(mythread);
    thread mytobj2(mythread);
    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura);
    g_ifend = true; // 对原子对象的写操作，让线程自行运行结束
    mytobj1.join();
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
