1.假设String类有如下私有成员：
class String
{
    private:
    char * str;//pointer to string allocated by new
    int len;//holds length of string
}
a.下述默认构造函数将产生什么问题？
String::String() {}
答：该函数没有将str初始化，应将指针设为nullptr或者new默认值
b.下述构造函数有什么问题？
String::String(const char *s)
{
str = s;
len = strlen(s);
}
答：没有创建新字符串，只是复制了原本字符串的地址，应当使用new 和strcpy()
c.下述构造函数将产生什么问题？
String::String(const str *s)
{
strcpy(str,s);
len = strlen(s);
}
答：赋值了字符串却没有分配空间,应使用new char[len+1]分配合适空间
2.如果你定义了一个类，其指针成员是用new初始化的，请指出可能出现的3个问题及解决方案
答：1）对象过期，成员指针消失但数据仍保留在内存里，使用析构函数在对象过期时释放分配的内存
2）使用对象初始化另外一个对象后，两个对象都执行析构函数释放内存时，会出现同一个内存被释放两次，定义赋值构造函数
3）对象赋值导致指向相同数据，重载赋值运算符
3.如果没有显式提供雷方法，编译器将自动生成那些类方法，描述这些隐士生成函数的行为。
答：默认构造函数，复制构造函数，赋值运算符，默认析构函数，地址运算符
4.找出并改正下述类声明中的错误
class nifty
{
//data
    char personality[];
    //char * personality;
    int talents;
//methods
//public:
    nifty();
    //nifty(const char * s);
    //nifty(const nifty & n);
    //~nifty(delete[] personality;)
    nifty(char *s);
    //nifty & operator=(const nifty &n)const;
    //frinend ostream & operator<<(ostream & os,const nifty &n);
    cstream & operator<<(ostream & os,nifty & n);
}

nifty:nifty()
{
    personality = nullptr;
    talents = 0;
}
//nifty::nifty(const char *s)
nifty:nifty(char * s)
{
    personality = new char[strlen(s)];
    personality = s;
    talents = 0;
}
//cstream & nifty:operator<<(ostream & os,const nifty & n)
cstream & nifty:operator<<(ostream & os,nifty & n)
{
    os << n;
}

5.下面的类声明
class Golfer
{
    private:
    char * fullname;
    int game;
    int * scores;
public:
    Golfer();
    Golfer(const char *name,int g = 0);
    Golfer(const Golfer & g);
   ~Golfer();
}
a.下列各条语句调用什么类方法？
Golfer nancy;//Golfer();
Golfer lulu("little lulu");//Golfer(const char *name,int g = 0);
Golfer roy("roy Hobbs",12);//Golfer(const char * name,int g);
Golfer * par = new Golfer;//Golfer()
Golfer next = lulu;//Golfer(const Golfer &g)
Golfer hazzard = "weed Thwacker";//Golfer(const char * name,int g)
*par = nancy;//default assignment operator
nancy = "Nancy Putter"//Golfer(const char * name,int g),default assignment operator