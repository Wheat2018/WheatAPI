# WheatAPI
Wheat同学的一套C++接口及方法

在2020-02-14更新的仓库中，API源文件有： 
1.MyTimer静态链接库项目：Wheat同学封装的计时器类，可以实现在Windows/Linux下的毫秒级（或更精确）的计时操作。并支持多点计时、帧率检测、帧率控制、控制台输出等操作。详细见MyTimer.h头文件。
2.ParamaterManager静态链接库项目：Wheat同学封装的参数管理类。你可以将该类引入自己的程序，打造成可以由Windows CMD进行参数调用的程序。该类实现了一套方便的字符串参数管理及多种类型定向转换方法。详细见描述文档。另外这个项目中还定义了几个全局通用函数（窄/宽字符分割函数、基于PPL库及Lambda的通用并发处理函数、获取目录下所有文件名列表等）
3.MyVector.hpp：容器类，包括动态数组MyArray、变长向量MyVector
4.MyMat.hpp、MyMat_Solve.hpp：矩阵类（试验阶段）
5.Vector3.hpp：三维向量类（试验阶段）