// lesson2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>

using namespace std;

//void myprint()
//{
//    cout << "我的线程执行完毕了1" << endl;
//    cout << "我的线程执行完毕了2" << endl;
//    cout << "我的线程执行完毕了3" << endl;
//    cout << "我的线程执行完毕了4" << endl;
//    cout << "我的线程执行完毕了5" << endl;
//    cout << "我的线程执行完毕了6" << endl;
//    cout << "我的线程执行完毕了7" << endl;
//    cout << "我的线程执行完毕了8" << endl;
//    cout << "我的线程执行完毕了9" << endl;
//    cout << "我的线程执行完毕了10" << endl;
//    return;
//}

//class TA
//{
//public:
//    // 可调用对象：重载圆括号
//    //void operator()() // 不带参数
//    //{
//    //    cout << "TA::operator()开始执行了" << endl;
//    //    // ……
//    //    cout << "TA::operator()执行结束了" << endl;
//    //}
//
//    TA(int& i) : m_i(i)
//    {
//        printf("TA()构造函数执行, m_i=%d, this=%p\n", m_i, this);
//    }
//    ~TA()
//    {
//        printf("~TA()析构函数执行, m_i=%d, this=%p\n", m_i, this);
//    }
//    TA(const TA& ta) : m_i(ta.m_i)
//    {
//        printf("TA()拷贝构造函数执行, m_i=%d, this=%p\n", m_i, this);
//    }
//    void operator()()
//    {
//        cout << "mi1的值为:" << m_i << endl; // 隐患，m_i可能没有有效值
//        cout << "mi2的值为:" << m_i << endl;
//        cout << "mi3的值为:" << m_i << endl;
//        cout << "mi4的值为:" << m_i << endl;
//        cout << "mi5的值为:" << m_i << endl;
//        cout << "mi6的值为:" << m_i << endl;
//    }
//    int& m_i; // 引入一个引用类型的成员变量
//};

int main()
{
    //一、范例演示线程运行的开始和结束
	//thread mytobj(myprint);		   // 这就是创建线程的代码,显然这是一个线程对象,然后给的参数是一个函数名，代表这个线程是从myprint这个函数(初始函数)开始运行
	//mytobj.join();						   // join会卡在这里，等待myprint线程执行完毕，程序流程才会继续往下走

    //mytobj.detach(); // 加入detach后，程序也不再报异常，能正常运行

    //if (mytobj.joinable())
    //{
    //    cout << "1: joinable() == true" << endl; // 成立
    //}
    //else
    //{
    //    cout << "1: joinable() == false" << endl;
    //}

 //   if (mytobj.joinable())
 //   {
 //       mytobj.join();
 //   }

    //二、其他创建线程的写法
    //1.用类来创建线程
    //TA ta;
    //thread mytobj3(ta); // ta,可调用对象:这里不可以是临时对象thread mytobj3(TA());否则编译无//法通过
    //mytobj3.join();     // 为保证等待线程执行结束，这里使用join cout<<"main 主函数执行结束!"<<endl;

    //int myi = 6;
    //TA ta(myi);
    //thread mytobj3(ta); // 创建并执行子线程
    ////mytobj3.detach();
    //mytobj3.join();

    //2.用lambda表达式来创建线程
    auto mylamthread = []
    {
        cout << "我的线程开始执行了" << endl;
        // ⋯⋯
        cout << "我的线程执行完毕了" << endl;
    };
    thread mytobj4(mylamthread);
    mytobj4.join();
    cout << "main主函数执行结束!" << endl; // 这行由主线程执行,主线程从main返回,则整个进程执行完毕
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
