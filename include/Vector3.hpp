/*
	By.Wheat
	2019.06.21

	�ļ���Vector3.hpp
	��������ά���࣬�ṩ������������㡣
		  ��ά�����࣬�ṩ���������������㣬ʸ���ڻ����������ת������
		  ��ά��ɫ���࣬��ά���฽����ɫ��Ϣ��װ��
		  ��ά���ࡣ
*/
#pragma once
#include <iostream>

#define GENERAL_GPU_CPU restrict(amp, cpu)

//��ά����
template <typename DataTy>
struct Point3 
{
	using ThisTy = Point3<DataTy>;
public:
	DataTy x, y, z;

	/*==================��������amp,cpuͨ�ã�====================*/
	//���޲ι��졿���ݳ�Աδ��ʼ��
	Point3() GENERAL_GPU_CPU { ; }
	//��ȫ�ι��졿���ݳ�Ա�����ʼ��
	Point3(DataTy x, DataTy y, DataTy z) GENERAL_GPU_CPU { this->x = x; this->y = y; this->z = z; }
	//���������졿
	Point3(const ThisTy& p) GENERAL_GPU_CPU :ThisTy(p.x, p.y, p.z) {; }

	/*==================ͬ�����㣨amp,cpuͨ�ã�====================*/
#define DATA_OPERATOR_CALCULATE(a, b, opt)	(a).x opt (b).x, (a).y opt (b).y, (a).z opt (b).z;
	//��+=��ֵ�����ݳ�Ա��Ӧ+=��ֵ
	ThisTy& operator+= (const ThisTy& p) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, p, +=); return *this; }
	//��-=��ֵ�����ݳ�Ա��Ӧ-=��ֵ
	ThisTy& operator-= (const ThisTy& p) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, p, -=); return *this; }
	//��*=��ֵ�����ݳ�Ա��Ӧ*=��ֵ
	ThisTy& operator*= (const ThisTy& p) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, p, *=); return *this; }
	//��/=��ֵ�����ݳ�Ա��Ӧ/=��ֵ
	ThisTy& operator/= (const ThisTy& p) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, p, /=); return *this; }
#undef DATA_OPERATOR_CALCULATE

	//���ӷ����������ݳ�Ա��Ӧ��Ӹ���
	ThisTy operator+ (ThisTy p)const GENERAL_GPU_CPU { return p += *this; }
	//���������������ݳ�Ա��Ӧ�������
	ThisTy operator- (ThisTy p)const GENERAL_GPU_CPU { return p -= *this; }
	//���˷����������ݳ�Ա��Ӧ��˸���
	ThisTy operator* (ThisTy p)const GENERAL_GPU_CPU { return p *= *this; }
	//���������������ݳ�Ա��Ӧ�������
	ThisTy operator/ (ThisTy p)const GENERAL_GPU_CPU { return p /= *this; }

	//�����š��������ݳ�Ա�෴������
	inline friend ThisTy operator- (ThisTy fv) GENERAL_GPU_CPU { return ThisTy(-fv.x, -fv.y, -fv.z); }

	/*==================�������㣨amp,cpuͨ�ã�====================*/
#define DATA_OPERATOR_CALCULATE(a, dat, opt)	(a).x opt dat, (a).y opt dat, (a).z opt dat;
	//����ֵ�����ݳ�Ա��Ӧ��ֵ
	ThisTy& operator=  (DataTy dat) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, dat, = ); return *this; }
	//��+=��ֵ�����ݳ�Ա��Ӧ+=��ֵ
	ThisTy& operator+= (DataTy dat) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, dat, +=); return *this; }
	//��-=��ֵ�����ݳ�Ա��Ӧ-=��ֵ
	ThisTy& operator-= (DataTy dat) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, dat, -=); return *this; }
	//��*=��ֵ�����ݳ�Ա��Ӧ*=��ֵ
	ThisTy& operator*= (DataTy dat) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, dat, *=); return *this; }
	//��/=��ֵ�����ݳ�Ա��Ӧ/=��ֵ
	ThisTy& operator/= (DataTy dat) GENERAL_GPU_CPU { DATA_OPERATOR_CALCULATE(*this, dat, /=); return *this; }
#undef DATA_OPERATOR_CALCULATE

	//���ӷ����������ݳ�Ա��Ӧ��Ӹ���
	inline friend ThisTy operator+ (ThisTy p, DataTy dat) GENERAL_GPU_CPU { return p += dat; }
	inline friend ThisTy operator+ (DataTy dat, ThisTy p) GENERAL_GPU_CPU { return p += dat; }
	//���������������ݳ�Ա��Ӧ�������
	inline friend ThisTy operator- (ThisTy p, DataTy dat) GENERAL_GPU_CPU { return p -= dat; }
	inline friend ThisTy operator- (DataTy dat, ThisTy p) GENERAL_GPU_CPU { return p -= dat; }
	//���˷����������ݳ�Ա��Ӧ��˸���
	inline friend ThisTy operator* (ThisTy p, DataTy dat) GENERAL_GPU_CPU { return p *= dat; }
	inline friend ThisTy operator* (DataTy dat, ThisTy p) GENERAL_GPU_CPU { return p *= dat; }
	//���������������ݳ�Ա��Ӧ����������������û���ҳ���
	inline friend ThisTy operator/ (ThisTy p, DataTy dat) GENERAL_GPU_CPU { return p /= dat; }

	//����ʽ����ת����
	explicit operator DataTy* () GENERAL_GPU_CPU { return &x; }
	explicit operator const DataTy* ()const GENERAL_GPU_CPU { return &x; }
	//����������--ȡַ������������ָ��
	DataTy* operator()() GENERAL_GPU_CPU { return operator DataTy * (); }
	const DataTy* operator()()const GENERAL_GPU_CPU { return operator const DataTy * (); }
	//����������
	DataTy& operator[] (int idx) { return operator DataTy * ()[idx]; }
	const DataTy& operator[] (int idx)const { return operator const DataTy * ()[idx]; }

	/*==================������ֵ����====================*/
	//��ģ�����ص�ԭ��ŷʽ����
	DataTy Module()const { return sqrt(x * x + y * y + z * z); }
	//����λ�������ص�λ������
	ThisTy Unitization()const { return *this / Module(); }
	//����׼��������ء�
	friend std::ostream& operator << (std::ostream& o, const ThisTy& p) { return o << "(" << p.x << ", " << p.y << ", " << p.z << ")"; }

	/*==================�����ǳ�ֵ����====================*/
	//����λ�������㵥λ��
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


//��ά������
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
	/*==================��������amp,cpuͨ�ã�====================*/
	//���޲ι��졿
	Vector3() GENERAL_GPU_CPU :BaseTy() { ; }
	//��ȫ�ι��졿���ݳ�Ա�����ʼ��
	Vector3(DataTy x, DataTy y, DataTy z) GENERAL_GPU_CPU :ThisTy() { this->x = x; this->y = y; this->z = z; }
	//���������졿
	Vector3(const ThisTy& v) GENERAL_GPU_CPU :BaseTy(v) { ; }
	//�����࿽�����졿
	Vector3(const BaseTy& v) GENERAL_GPU_CPU : BaseTy(v) { ; }


	/*==================��������أ�amp,cpuͨ�ã�====================*/
	//����ֵ�����ݳ�Ա��Ӧ��ֵ
	template <typename OpeTy> ThisTy& operator= (const OpeTy& dat) GENERAL_GPU_CPU { this->BaseTy::operator=(dat); return *this; }
	//�������ڻ�����ˡ��ڻ����÷�������Ϊ��д���ǣ�
	DataTy operator* (const ThisTy& v) GENERAL_GPU_CPU { return x * v.x + y * v.y + z * v.z; }
	//�������������ˡ�������
	ThisTy operator^ (const ThisTy& v) GENERAL_GPU_CPU { return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x }; }

	/*==================�����ǳ�ֵ����====================*/
	//����ת������ά����n��ת����rad����ת�������ֶ���
	template<typename NDataTy, typename RadTy>
	ThisTy& Rotate(const Vector3<NDataTy>& n, RadTy rad)
	{
		//��������������ת����
		ThisTy m1(n.x * n.x + (1 - n.x * n.x) * cos(rad)		,n.x * n.y * (1 - cos(rad)) - n.z * sin(rad)	,n.x * n.z * (1 - cos(rad)) + n.y * sin(rad));
		ThisTy m2(n.x * n.y * (1 - cos(rad)) + n.z * sin(rad)	,n.y * n.y + (1 - n.y * n.y) * cos(rad)			,n.y * n.z * (1 - cos(rad)) - n.x * sin(rad));
		ThisTy m3(n.x * n.z * (1 - cos(rad)) - n.y * sin(rad)	,n.y * n.z * (1 - cos(rad)) + n.x * sin(rad)	,n.z * n.z + (1 - n.z * n.z) * cos(rad)		);

		//�������
		ThisTy vp = { m1 * (*this), m2 * (*this), m3 * (*this) };

		return *this = vp;
	}
	//��ƽ�滯����xyƽ��ͶӰ
	ThisTy& Planarity() GENERAL_GPU_CPU { (*this).y = 0; return *this; }

};
typedef Vector3<float> Vector3F;
typedef Vector3<int> Vector3I;

//��ά����
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
