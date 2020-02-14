/*
	By.Wheat
	2019.06.21

	文件：Vector3.hpp
	简述：三维点类，提供点标量四则运算。
		  三维向量类，提供向量标量四则运算，矢量内积、外积、旋转方法。
		  三维颜色点类，三维点类附加颜色信息封装。
		  二维点类。
*/
#pragma once
#include <iostream>

#define GENERAL_GPU_CPU restrict(amp, cpu)

//三维点类
template <typename DataTy>
struct Point3 
{
	using ThisTy = Point3<DataTy>;
public:
	DataTy x, y, z;

	/*==================构造器（amp,cpu通用）====================*/
	//【无参构造】数据成员未初始化
	Point3() GENERAL_GPU_CPU { ; }
	//【全参构造】数据成员逐个初始化
	Point3(DataTy x, DataTy y, DataTy z) GENERAL_GPU_CPU { this->x = x; this->y = y; this->z = z; }
	//【拷贝构造】
	Point3(const ThisTy& p) GENERAL_GPU_CPU :ThisTy(p.x, p.y, p.z) {; }

	/*==================同类运算（amp,cpu通用）====================*/
#define DATA_OPERATOR_CALCULATE(a, b, opt)	(a).x opt (b).x, (a).y opt (b).y, (a).z opt (b).z;
	//【+=赋值】数据成员对应+=赋值
	ThisTy& operator+= (const ThisTy& p) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, p, +=); return *this; }
	//【-=赋值】数据成员对应-=赋值
	ThisTy& operator-= (const ThisTy& p) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, p, -=); return *this; }
	//【*=赋值】数据成员对应*=赋值
	ThisTy& operator*= (const ThisTy& p) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, p, *=); return *this; }
	//【/=赋值】数据成员对应/=赋值
	ThisTy& operator/= (const ThisTy& p) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, p, /=); return *this; }
#undef DATA_OPERATOR_CALCULATE

	//【加法】返回数据成员对应相加副本
	ThisTy operator+ (ThisTy p)const GENERAL_GPU_CPU { return p += *this; }
	//【减法】返回数据成员对应相减副本
	ThisTy operator- (ThisTy p)const GENERAL_GPU_CPU { return p -= *this; }
	//【乘法】返回数据成员对应相乘副本
	ThisTy operator* (ThisTy p)const GENERAL_GPU_CPU { return p *= *this; }
	//【除法】返回数据成员对应相除副本
	ThisTy operator/ (ThisTy p)const GENERAL_GPU_CPU { return p /= *this; }

	//【负号】返回数据成员相反数副本
	inline friend ThisTy operator- (ThisTy fv) GENERAL_GPU_CPU { return ThisTy(-fv.x, -fv.y, -fv.z); }

	/*==================异类运算（amp,cpu通用）====================*/
#define DATA_OPERATOR_CALCULATE(a, dat, opt)	(a).x opt dat, (a).y opt dat, (a).z opt dat;
	//【赋值】数据成员对应赋值
	ThisTy& operator=  (DataTy dat) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, dat, = ); return *this; }
	//【+=赋值】数据成员对应+=赋值
	ThisTy& operator+= (DataTy dat) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, dat, +=); return *this; }
	//【-=赋值】数据成员对应-=赋值
	ThisTy& operator-= (DataTy dat) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, dat, -=); return *this; }
	//【*=赋值】数据成员对应*=赋值
	ThisTy& operator*= (DataTy dat) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, dat, *=); return *this; }
	//【/=赋值】数据成员对应/=赋值
	ThisTy& operator/= (DataTy dat) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, dat, /=); return *this; }
#undef DATA_OPERATOR_CALCULATE

	//【加法】返回数据成员对应相加副本
	inline friend ThisTy operator+ (ThisTy p, DataTy dat) GENERAL_GPU_CPU { return p += dat; }
	inline friend ThisTy operator+ (DataTy dat, ThisTy p) GENERAL_GPU_CPU { return p += dat; }
	//【减法】返回数据成员对应相减副本
	inline friend ThisTy operator- (ThisTy p, DataTy dat) GENERAL_GPU_CPU { return p -= dat; }
	inline friend ThisTy operator- (DataTy dat, ThisTy p) GENERAL_GPU_CPU { return p -= dat; }
	//【乘法】返回数据成员对应相乘副本
	inline friend ThisTy operator* (ThisTy p, DataTy dat) GENERAL_GPU_CPU { return p *= dat; }
	inline friend ThisTy operator* (DataTy dat, ThisTy p) GENERAL_GPU_CPU { return p *= dat; }
	//【除法】返回数据成员对应相除副本（异类除法没有右除）
	inline friend ThisTy operator/ (ThisTy p, DataTy dat) GENERAL_GPU_CPU { return p /= dat; }

	//【显式类型转换】
	explicit operator DataTy* () GENERAL_GPU_CPU { return &x; }
	explicit operator const DataTy* ()const GENERAL_GPU_CPU { return &x; }
	//【括号重载--取址器】返回数据指针
	DataTy* operator()() GENERAL_GPU_CPU { return operator DataTy * (); }
	const DataTy* operator()()const GENERAL_GPU_CPU { return operator const DataTy * (); }
	//【索引器】
	DataTy& operator[] (int idx) { return operator DataTy * ()[idx]; }
	const DataTy& operator[] (int idx)const { return operator const DataTy * ()[idx]; }

	/*==================其他常值方法====================*/
	//【模】返回到原点欧式距离
	DataTy Module()const { return sqrt(x * x + y * y + z * z); }
	//【单位化】返回单位化副本
	ThisTy Unitization()const { return *this / Module(); }
	//【标准流输出重载】
	friend std::ostream& operator << (std::ostream& o, const ThisTy& p) { return o << "(" << p.x << ", " << p.y << ", " << p.z << ")"; }

	/*==================其他非常值方法====================*/
	//【单位化】将点单位化
	ThisTy& Unitize() { return *this /= Module(); }

};
typedef Point3<float> Point3F;
typedef Point3<int> Point3I;

typedef unsigned long COLORREF;
template <typename DataTy>
struct CPoint3 :Point3<DataTy> 
{
	using ThisTy = CPoint3<DataTy>;
	using BaseTy = Point3<DataTy>;
public:
	COLORREF color;
	CPoint3() { ; }
	CPoint3(BaseTy p, COLORREF color) :BaseTy(p), color(color) { ; }
	CPoint3(DataTy x, DataTy y, DataTy z, COLORREF color) :BaseTy(x, y, z), color(color) { ; }
	ThisTy& operator= (DataTy x) { BaseTy::operator=(x); return *this; }
	friend std::ostream& operator << (std::ostream& o, const ThisTy& p) { return o << "(" << p.x << ", " << p.y << ", " << p.z << "||" << p.color << ")"; }
};
typedef CPoint3<float> CPoint3F;
typedef CPoint3<int> CPoint3I;


//三维向量类
template <typename DataTy>
struct Vector3 :public Point3<DataTy> 
{
	using ThisTy = Vector3<DataTy>;
	using BaseTy = Point3<DataTy>;
	using BaseTy::x;
	using BaseTy::y;
	using BaseTy::z;
private:

public:
	/*==================构造器（amp,cpu通用）====================*/
	//【无参构造】
	Vector3() GENERAL_GPU_CPU :BaseTy() { ; }
	//【全参构造】数据成员逐个初始化
	Vector3(DataTy x, DataTy y, DataTy z) GENERAL_GPU_CPU :ThisTy() { this->x = x; this->y = y; this->z = z; }
	//【拷贝构造】
	Vector3(const ThisTy& v) GENERAL_GPU_CPU :BaseTy(v) { ; }
	//【父类拷贝构造】
	Vector3(const BaseTy& v) GENERAL_GPU_CPU : BaseTy(v) { ; }


	/*==================运算符重载（amp,cpu通用）====================*/
	//【赋值】数据成员对应赋值
	template <typename OpeTy> ThisTy& operator= (const OpeTy& dat) GENERAL_GPU_CPU { this->BaseTy::operator=(dat); return *this; }
	//【向量内积】点乘、内积（该方法可视为重写覆盖）
	DataTy operator* (const ThisTy& v) GENERAL_GPU_CPU { return x * v.x + y * v.y + z * v.z; }
	//【向量外积】叉乘、向量积
	ThisTy operator^ (const ThisTy& v) GENERAL_GPU_CPU { return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x }; }

	/*==================其他非常值方法====================*/
	//【旋转】绕三维向量n旋转弧度rad，旋转符合右手定则
	template<typename NDataTy, typename RadTy>
	ThisTy& Rotate(const Vector3<NDataTy>& n, RadTy rad)
	{
		//向量绕任意轴旋转矩阵
		ThisTy m1(n.x * n.x + (1 - n.x * n.x) * cos(rad)		,n.x * n.y * (1 - cos(rad)) - n.z * sin(rad)	,n.x * n.z * (1 - cos(rad)) + n.y * sin(rad));
		ThisTy m2(n.x * n.y * (1 - cos(rad)) + n.z * sin(rad)	,n.y * n.y + (1 - n.y * n.y) * cos(rad)			,n.y * n.z * (1 - cos(rad)) - n.x * sin(rad));
		ThisTy m3(n.x * n.z * (1 - cos(rad)) - n.y * sin(rad)	,n.y * n.z * (1 - cos(rad)) + n.x * sin(rad)	,n.z * n.z + (1 - n.z * n.z) * cos(rad)		);

		//矩阵相乘
		ThisTy vp = { m1 * (*this), m2 * (*this), m3 * (*this) };

		return *this = vp;
	}
	//【平面化】在xy平面投影
	ThisTy& Planarity() GENERAL_GPU_CPU { (*this).y = 0; return *this; }

};
typedef Vector3<float> Vector3F;
typedef Vector3<int> Vector3I;

//二维点类
template <typename DataTy>
struct Point2 {
	using ThisTy = Point2<DataTy>;
	DataTy x, y;

	Point2() { ; }
	Point2(DataTy x, DataTy y) { this->x = x; this->y = y; }

	friend std::ostream& operator << (std::ostream& o, const ThisTy& p) { return o << "(" << p.x << ", " << p.y << ")"; }

};
typedef Point2<float> Point2F;
typedef Point2<int> Point2I;
