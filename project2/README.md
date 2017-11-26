# 实现语义分析
## hashTable.c以及hashTable.h文件：
1.用的时候提供数据结构**HashTable**和**HashNode**

2.HashTable\* \*hashInit(int size);
  
该函数用来创建一个HashTable，并传回指针
  
3.HashTable提供了两个函数hashPush和hashFind可以使用"->"进行调用

参数均为要查询的散列表和要查询的字符串

hashPush返回1表示移入散列表成功，0表示字符串已存在移入失败

hashFind返回一个HashNode结构体指针，若查询成功则返回正确的指针，若为NULL表示未查询到字符串。
