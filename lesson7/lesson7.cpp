// lesson7.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

std::mutex resource_mutex;
std::once_flag g_flag; // 这是一个系统定义的标记

class MyCAS // 这是一个单例类
{
    static void CreateInstance()
    {
        // 如下两行是测试代码
        //std::chrono::milliseconds dura(20000); //1s = 1000ms,所以20000ms= 20s
        //std:: this_thread:: sleep_for( dura); //休息一定的时长
        m_instance = new MyCAS();
        cout << "CreateInstance()执行完毕"; // 测试用
        static CGarhuishou cl;
    }
private:
    MyCAS() {} // 构造函数是私有的
private:
    static MyCAS* m_instance;

public:
    //static MyCAS* GetInstance()
    //{
    //    std::unique_lock<std::mutex> mymutex(resource_mutex); // 自动加锁
    //    if (m_instance == NULL)
    //    {
    //        m_instance = new MyCAS();
    //        static CGarhuishou cl; // 生命周期一直到程序退出
    //    }
    //    return m_instance;
    //}
    //static MyCAS* GetInstance()
    //{
    //    if (m_instance == NULL)  //双重锁定（双重检查）
    //    {
    //        std::unique_lock<std::mutex> mymutex(resource_mutex); // 自动加锁
    //        if (m_instance == NULL)
    //        {
    //            m_instance = new MyCAS();
    //            static CGarhuishou cl; // 生命周期一直到程序退出
    //        }
    //    }
    //    return m_instance;
    //}
    static MyCAS* GetInstance()
    {
        if (m_instance == NULL) // 同样为提高效率
        {
            std::call_once(g_flag, CreateInstance); // 两个线程同时执行到这里时,其中一个线程卡在//这行等另外一个线程的该行执行完毕(所以可以把g_flag 看成一把锁)
        }  
        return m_instance;
    }
    class CGarhuishou // 类中套类，用于释放对象
    {
    public:
        ~CGarhuishou()
        {
            if (MyCAS::m_instance)
            {
                delete MyCAS::m_instance;
                MyCAS::m_instance = NULL;
            }
        }
    };
    void func() // 普通成员函数，方便做一些测试调用
    {
        cout << "测试" << endl;
    }
};

MyCAS* MyCAS::m_instance = NULL; // 类静态成员变量定义并初始化

void mythread()
{
    cout << "我的线程开始执行了" << endl;
    MyCAS* p_a = MyCAS::GetInstance(); // 在这里初始化就很可能出现问题
    cout << "我的线程执行完毕了" << endl;
    return;
}

int main()
{
    //MyCAS* p_a = MyCAS::GetInstance(); // 创建单例类MyCAS类的对象
    ////MyCAS* p_b = MyCAS::GetInstance();
    //p_a->func();                       // 一条测试语句，用于打印结果
    //MyCAS::GetInstance()->func();      // 这种写法的测试语句也可以打印结果

    std::thread mytobj1(mythread);
    std::thread mytobj2(mythread);
    mytobj1.join();
    mytobj2.join();
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
