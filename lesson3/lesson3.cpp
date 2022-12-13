// lesson3.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>

using namespace std;

class A
{
public:
    A(int a) : m_i(a)
    {
        cout << "A::A(int a)构造函数执行, this=" << this << ", threadid=" << std::this_thread::get_id() << endl;
    }
    A(const A& a)
    {
        cout << "A::A(const A)拷贝构造函数执行, this =" << this << ", threadid = " << std::this_thread::get_id() << endl;
    }
    ~A()
    {
        cout << "~A::A( )析构函数执行, this=" << this << ", threadid=" << std::this_thread::get_id() << endl;
    }
    mutable int m_i;

    void thread_work(int num) // 带一个参数
    {
        cout << "子线程 thread_work 执行, this=" << this << ", threadid=" << std::this_thread::get_id() << endl;
    }

    void operator()(int num)
    {
        cout << "子线程( )执行, this=" << this << " threadid=" << std::this_thread::get_id() << endl;
    }
};

//void myprint(const int& i, char* pmybuf)
//{
//    cout << i << endl;
//    cout << pmybuf << endl;
//    return;
//}

//void myprint(int i, const A& pmybuf)
//{
//    // cout << i<< endl;
//    cout << &pmybuf << endl; // 这里打印对象 pmybuf 的地址
//    return;
//}

//void myprint2(const A& pmybuf)
////void myprint2(const A pmybuf)
//{
//    cout << "子线程 myprint2的参数 paybuf的地址是:" << &pmybuf << ", threadid =" << std::this_thread::get_id() << endl;
//}

//void myprint2(const A& pmybuf)
//{
//    pmybuf.m_i = 199; // 修改该值不会影响到 main 主函数中实参的该成员变量
//    cout << "子线程 myprint2的参数 pmybuf的地址是:" << &pmybuf << ", threadid =" << std::this_thread::get_id() << endl;
//}

void myprint3(unique_ptr<int> pzn)
{
    return;
}

int main()
{
    // 一、传递临时对象作为线程参数
    /*int mvar = 1;
    int& mvary = mvar;
    char mybuf[] = "this is a test!";
    std::thread mytobj(myprint, mvar, mybuf);
    mytobj.join();
    cout << "main主函数执行结束!" << endl;*/
    //int mvar = 1;
    //int mysecondpar = 12;
    ////thread mytobj(myprint, mvar, mysecondpar); // 希望mysecondpar转成A类型对象传递给myprint的第二个参数
    //thread mytobj(myprint, mvar, A(mysecondpar));
    //mytobj. detach();
    ////mytobj.join();
    //cout << "main主函数执行结束!" << endl;

    //二、临时对象作为线程参数
    //cout << "主线程 id=" << std::this_thread::get_id() << endl;
    //int mvar = 1;
    ////std::thread mytobj(myprint2, mvar);
    //std::thread mytobj(myprint2, A(mvar));
    //mytobj.join(); // 用join方便观察
    //cout << "main主函数执行结束!" << endl;

    //三、传递类对象与智能指针作为线程参数
    //A myobj(10);                         // 生成一个类对象
    ////std::thread mytobj(myprint2, myobj); // 将类对象作为线程参数
    //std::thread mytobj(myprint2, std::ref(myobj));
    //mytobj.join();
    //cout << "main 主函数执行结束!" << endl;

    //智能指针作为线程参数
    //unique_ptr<int> myp(new int(100));
    //std::thread mytobj(myprint3, std::move(myp));
    //mytobj.join();
    //cout << "main主函数执行结束!" << endl;

    //四、用成员函数作为线程入口函数
    //A myobj(10);
    ////std::thread mytobj(&A::thread_work, myobj, 15);
    //std::thread mytobj(&A::thread_work, &myobj, 15); // 第二个参数也可以是 std::ref(myobj)
    //mytobj.join();
    //cout << "main主函数执行结束!" << endl;

    A myobj(10);
    //thread mytobj(myobj, 15);
    thread mytobj(std::ref(myobj), 15);//第二个参数无法修改为&myobj,编译会报错
    mytobj.join();
    cout << "main主函数执行结束!" << endl;
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
