#include <stdio.h>
#include <stdlib.h>

typedef void *list; // lisp(list process)中,一切都是列表

int TypeTag(list v)
{
    // 获得元素的类型
    // 从后面的基础类型的结构体可以得知,结构体的第一个元素是int型(用来表示类型)
    // 所以这里通过强制类型转换,截取第一个int长度的字节.
    return *((int *) v);
}

// --------------------------------------------


int main()
{
    printf("Hello, World!\n");
    return 0;
}