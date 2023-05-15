// https://blog.csdn.net/albertsh/article/details/92385277
// https://zhuanlan.zhihu.com/p/343040291

// 关于 float 类型数据的精度测试
/*
    float 存储模式 1位符号位，8位指数位，23位尾数
*/

// float范围 [-2^128 - 1, 2^128 - 1]

/* 精度表见 float_accuracy.docx*/

#include <iostream>
#include <math.h>
#include <iomanip>
using namespace std;

// 判断电脑内存中存储数据的模式是big-endian还是little-endian
// 对于数据0x1234
// big-endian: 0x12, 0x34
// little-endian: 0x34, 0x12
bool isBigEndian()
{
    union NUM
    {
        int i;
        char b;
    } num;

    num.i = 0x1234;
    if (num.b == 0x12)
    {
        return true;
    }
    else
        return false;
}

// 把float数据在内存中的存储值解析出来
unsigned char *decode_float(float value)
{
    int per_bytes = 8;
    int bytes_count = per_bytes * sizeof(float);
    unsigned char *bytes = new unsigned char(bytes_count);

    float *pf_value = &value;
    char *pb_value = (char *)pf_value;
    for (size_t i = 0; i < sizeof(float); i++, pb_value++)
    {
        for (size_t j = 0; j < per_bytes; j++)
        {
            bytes[i * per_bytes + j] = (*pb_value >> (per_bytes - 1 - j)) & 0x1;
        }
    }
    return bytes;
}

void print_float_bytes(float value, unsigned char* bytes, int bytes_count)
{
    cout << fixed << setprecision(24);
    cout << "In memory, " << value << "f is store as => ";
    for (int index = 0; index < 32; index++)
    {
        std::cout << (int)bytes[index];
        if ((index + 1) % 4 == 0)
            cout << " ";
        if ((index + 1) % 8 == 0 && (index + 1) != bytes_count)
            cout << ",  ";
    }
    cout << endl << endl;
}

int main()
{
    // 测试解码是否正确
    // 5.2f 在内存（little-endian)中存储数据为  0110 0110,  0110 0110,  1010 0110, 0100 0000  <= (101.0011 0011 0011 0011 0011 0)
    float f = 5.2f;
    print_float_bytes(f, decode_float(f), 32);
    cout << boolalpha;
    cout << "5.2f == 5.19999980926513671875000f : " << (5.2f == 5.19999980926513671875000f) << endl << endl;
    cout <<  (5.2f == 5.199999891f) << endl << endl;

    // (2^-23) = 0.0000 0011 9209 2895 5078 125f
    float f1 = 0.00000011920928955078125f;
    print_float_bytes(f1, decode_float(f1), 32);

    // (2^-24) = 0.0000 0005 9604 6447 7539 0625f
    float f2 = 0.000000059604644775390625f;
    print_float_bytes(f2, decode_float(f2), 32);

    // float 指数取值范围 [1,254] ,减于偏移量127后，指数取值范围是[-126, 127]
    print_float_bytes(-0.f, decode_float(0.f), 32);

    float f3 = 16777215.f, f4 = 16777216.f, f5 = 16777217.f, f6 = 16777218.f;
    printf("%f\n", f3);
    printf("%f\n", f4);
    printf("%f\n", f5);
    printf("%f\n", f6);

    print_float_bytes(8.12345671f, decode_float(8.12345672f), 32);
    print_float_bytes(8.12345671f, decode_float(8.12345672f), 32);
    cout << (8.12345671f == 8.12345672f) << endl;



    getchar();
    return 0;
}