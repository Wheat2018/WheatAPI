#include <iostream>
#include <initializer_list>
#include <functional>



//================================================================================
//鲁棒性
#ifndef COUT
#define COUT std::cout
#endif
#ifndef ENDL
#define ENDL std::endl
#endif

#undef AMP_CPU
#undef MyMat_Solve_Name

#ifdef GENERAL_GPU_CPU

#define MyMat_Solve_Name MyMat_Solve_AMP
#define AMP_CPU restrict(amp, cpu)

#else

#define MyMat_Solve_Name MyMat_Solve
#define AMP_CPU

#endif


#ifndef WHEAT_REPORT
//报告措施
#define WHEAT_REPORT(expr, msg) if(!(expr)) COUT << (msg) << ENDL;
#endif
#ifndef WHEAT_ASSERT
//终止措施
#ifdef _ASSERT_EXPR
#define WHEAT_ASSERT(expr, msg) _ASSERT_EXPR(expr, (msg))
#else
#define WHEAT_ASSERT(expr, msg)
#endif
#endif

#if defined(WARNING_ASSERT)
//警告措施为终止
#define WARNING_CHECK(expr, msg) WHEAT_ASSERT(expr, msg)
#elif defined(WARNING_REPORT)
//警告措施为报告
#define WARNING_CHECK(expr, msg) WHEAT_REPORT(expr, msg)
#else
//警告措施为无
#define WARNING_CHECK(expr, msg)
#endif

#if defined(ERROR_ASSERT)
//错误措施为终止
#define ERROR_CHECK(expr, msg) WHEAT_ASSERT(expr, msg)
#elif defined(ERROR_REPORT)
//错误措施为报告
#define ERROR_CHECK(expr, msg) WHEAT_REPORT(expr, msg)
#else
//错误措施为无
#define ERROR_CHECK(expr, msg)
#endif

template<typename MatProperty> struct NoneMatrixMul;

template<typename MatProperty, template<typename M> typename MatrixMul = NoneMatrixMul>
struct MyMat_Solve_Name : public MatProperty
{
public:
	using ThisTy = MyMat_Solve_Name<MatProperty, MatrixMul>;
	using BaseTy = MatProperty;
	using DataTy = decltype(MatProperty::DataTy());
	using PropTy = MatProperty;
	using MMulTy = MatrixMul<MatProperty>;

public:

	/*=========================构造函数============================*/
	//【继承父类的构造函数】
	using BaseTy::BaseTy;

	//【无参构造】
	MyMat_Solve_Name() AMP_CPU;

	//【等号赋值】全部元素赋值为dat
	ThisTy& operator=  (DataTy dat) AMP_CPU;

	//【拷贝构造】调用父类拷贝构造
	MyMat_Solve_Name(const BaseTy& mat_cRef) AMP_CPU;
	MyMat_Solve_Name(const ThisTy& mat_cRef) AMP_CPU;

	//【拷贝赋值】全元素拷贝赋值
	ThisTy& operator=  (const BaseTy& mat_cRef) AMP_CPU;
	ThisTy& operator=  (const ThisTy& mat_cRef) AMP_CPU;

	//【移动构造】调用父类移动构造
	MyMat_Solve_Name(BaseTy&& mat_rRef);
	MyMat_Solve_Name(ThisTy&& mat_rRef);

	//【移动赋值】调用父类移动赋值
	ThisTy& operator=  (ThisTy&& mat_rRef);

	//【不定参构造】
	MyMat_Solve_Name(std::initializer_list<DataTy> il);

	//【数组等号赋值】
	ThisTy& operator=(std::initializer_list<DataTy> il);

	/*=========================索引器==============================*/

	//【一维索引器】
	auto operator[] (int idx) AMP_CPU ->decltype(this->at(idx));
	auto operator[] (int idx) const AMP_CPU ->decltype(this->at(idx));

	//【二维索引器】
	auto operator() (int row, int col) AMP_CPU ->decltype(this->at(row, col));
	auto operator() (int row, int col) const AMP_CPU ->decltype(this->at(row, col));

	//【迭代首尾指针】使用迭代器要求性质类PropTy中数据存放内存地址连续
	auto begin() { return &operator[](0); }
	auto end() { return &operator[](this->length - 1); }

	/*========================流运算符=============================*/

	template<typename Ty1, typename Ty2>
	void Print(std::ostream& o, Ty1 row_inter_symbol, Ty2 col_inter_symbol) const;

	template<typename MP, template<typename M> typename MM>
	friend std::ostream& operator<< (std::ostream& o, const MyMat_Solve_Name<MP, MM>& mat);

	/*==================四则运算（amp,cpu通用）====================*/

	//【+=赋值】数据成员对应+=赋值
	auto operator+= (const ThisTy& mat) AMP_CPU ->ThisTy &;
	auto operator+= (DataTy dat) AMP_CPU ->ThisTy &;

	//【-=赋值】数据成员对应-=赋值
	auto operator-= (const ThisTy& mat) AMP_CPU->ThisTy &;
	auto operator-= (DataTy dat) AMP_CPU ->ThisTy &;

	//【*=赋值】数据成员对应*=赋值
	auto operator*= (const ThisTy& mat) AMP_CPU->ThisTy &;
	auto operator*= (DataTy dat) AMP_CPU ->ThisTy &;

	//【/=赋值】数据成员对应/=赋值
	auto operator/= (const ThisTy& mat) AMP_CPU->ThisTy &;
	auto operator/= (DataTy dat) AMP_CPU ->ThisTy &;

	//【负号】返回数据成员相反数副本
	template<typename MP, template<typename M> typename MM>
	friend auto operator- (MyMat_Solve_Name<MP, MM> mat) AMP_CPU ->decltype(mat);

	//【加法】返回数据成员对应相加副本
	template<typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator+ (const MyMat_Solve_Name<MP, MM>& mat1, const MyMat_Solve_Name<MP, MM>& mat2) AMP_CPU;

	template<typename Ty, typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator+ (const MyMat_Solve_Name<MP, MM>& mat, Ty dat) AMP_CPU;

	template<typename Ty, typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator+ (Ty dat, const MyMat_Solve_Name<MP, MM>& mat) AMP_CPU;

	//【减法】返回数据成员对应相减副本
	template<typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator- (const MyMat_Solve_Name<MP, MM>& mat1, const MyMat_Solve_Name<MP, MM>& mat2) AMP_CPU;

	template<typename Ty, typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator- (const MyMat_Solve_Name<MP, MM>& mat, Ty dat) AMP_CPU;

	template<typename Ty, typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator- (Ty dat, const MyMat_Solve_Name<MP, MM>& mat) AMP_CPU;

	//【乘法】返回数据成员对应相乘副本
	template<typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator* (const MyMat_Solve_Name<MP, MM>& mat1, const MyMat_Solve_Name<MP, MM>& mat2) AMP_CPU;

	template<typename Ty, typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator* (const MyMat_Solve_Name<MP, MM>& mat, Ty dat) AMP_CPU;

	template<typename Ty, typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator* (Ty dat, const MyMat_Solve_Name<MP, MM>& mat) AMP_CPU;

	//【除法】返回数据成员对应相除副本
	template<typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator/ (const MyMat_Solve_Name<MP, MM>& mat1, const MyMat_Solve_Name<MP, MM>& mat2) AMP_CPU;
	
	template<typename Ty, typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator/ (const MyMat_Solve_Name<MP, MM>& mat, Ty dat) AMP_CPU;
	
	template<typename Ty, typename MP, template<typename M> typename MM>
	friend MyMat_Solve_Name<MP, MM> operator/ (Ty dat, const MyMat_Solve_Name<MP, MM>& mat) AMP_CPU;

	/*==================其他运算=================================*/
	
	//【拷贝】
	void CopyForm(const BaseTy& src);

	//【矩阵乘法】
	template<typename MatTy>
	auto mul(const MatTy& mat) const
	{
		auto res = MMulTy::mul(*this, mat);
		return std::move(static_cast<MyMat_Solve<decltype(res)::PropTy, MatrixMul>>(res));
	}
	template<typename MatTy>
	auto mul_amp(MatTy mat) const
	{
		auto res = MMulTy::mul_amp(*this, mat);
		return std::move(static_cast<MyMat_Solve<decltype(res)::PropTy, MatrixMul>>(res));
	}

	
};

#ifndef NONEMATRIXMUL
#define NONEMATRIXMUL
template<typename MatProperty>
struct NoneMatrixMul {
	static void mul(const MatProperty&, void*)
	{
		WARNING_CHECK(0, "NoneMatrixMul Warnning: mul non-func!");
	}
	static void mul_amp(const MatProperty&, void*)
	{
		WARNING_CHECK(0, "NoneMatrixMul Warnning: mul_amp non-func!");
	}
};
#endif




/*====================================================函数实现================================================*/

template<typename MatProperty, template<typename M> typename MatrixMul> inline
MyMat_Solve_Name<MatProperty, MatrixMul>::MyMat_Solve_Name()
AMP_CPU :BaseTy()
{
	;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
MyMat_Solve_Name<MatProperty, MatrixMul>& MyMat_Solve_Name<MatProperty, MatrixMul>::operator=(DataTy dat)
AMP_CPU
{
	for (int i = 0; i < this->length; i++) this->at(i) = dat;
	return *this;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
MyMat_Solve_Name<MatProperty, MatrixMul>::MyMat_Solve_Name(const BaseTy& mat_cRef)
AMP_CPU :BaseTy(mat_cRef)
{
	;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
MyMat_Solve_Name<MatProperty, MatrixMul>::MyMat_Solve_Name(const ThisTy& mat_cRef)
AMP_CPU :BaseTy(mat_cRef)
{
	;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
MyMat_Solve_Name<MatProperty, MatrixMul>& MyMat_Solve_Name<MatProperty, MatrixMul>::operator=(const BaseTy& mat_cRef)
AMP_CPU
{
	for (int i = 0; i < this->length; i++) this->at(i) = mat_cRef.at(i);
	return *this;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
MyMat_Solve_Name<MatProperty, MatrixMul>& MyMat_Solve_Name<MatProperty, MatrixMul>::operator=(const ThisTy& mat_cRef)
AMP_CPU
{
	for (int i = 0; i < this->length; i++) this->at(i) = mat_cRef.at(i);
	return *this;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
MyMat_Solve_Name<MatProperty, MatrixMul>::MyMat_Solve_Name(BaseTy&& mat_rRef)
:BaseTy(std::move(mat_rRef))
{
	;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
MyMat_Solve_Name<MatProperty, MatrixMul>::MyMat_Solve_Name(ThisTy&& mat_rRef)
:BaseTy(std::move(static_cast<BaseTy&>(mat_rRef)))
{
	;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
MyMat_Solve_Name<MatProperty, MatrixMul>& MyMat_Solve_Name<MatProperty, MatrixMul>::operator=(ThisTy&& mat_rRef)

{
	this->BaseTy::operator = (std::move(static_cast<BaseTy&>(mat_rRef)));
	return *this;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline 
MyMat_Solve_Name<MatProperty, MatrixMul>::MyMat_Solve_Name(std::initializer_list<DataTy> il) 
{ 
	this->operator=(std::move(il));
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline 
MyMat_Solve_Name<MatProperty, MatrixMul>& MyMat_Solve_Name<MatProperty, MatrixMul>::operator=(std::initializer_list<DataTy> il)
{
	WARNING_CHECK(il.size() <= this->length, "MyMat_Solve Warning: the number of input must be less than length of mat!");
	int i = 0;
	for (const auto& elem : il)
	{
		if (i >= this->length) return *this;
		this->at(i++) = elem;
	}
	for (; i < this->length; i++) this->at(i) = DataTy(0);
	return *this;
}

/*=========================索引器==============================*/

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator[](int idx)
AMP_CPU ->decltype(this->at(idx))
{
	return this->at(idx);
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator[](int idx)
const AMP_CPU ->decltype(this->at(idx))
{
	return this->at(idx);
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator()(int row, int col)
AMP_CPU ->decltype(this->at(row, col))
{
	return this->at(row, col);
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator()(int row, int col)
const AMP_CPU ->decltype(this->at(row, col))
{
	return this->at(row, col);
}

/*========================流运算符=============================*/

template<typename MatProperty, template<typename M> typename MatrixMul> template<typename Ty1, typename Ty2> inline
void MyMat_Solve_Name<MatProperty, MatrixMul>::Print(std::ostream& o, Ty1 row_inter_symbol, Ty2 col_inter_symbol)
const
{
	int i, j;
	for (i = 0; i < this->rows; i++)
	{
		o << "[";
		for (j = 0; j < this->cols; j++)
		{
			o << this->at(i,j);
			if (j != this->cols - 1) o << col_inter_symbol;
		}
		o << "]";
		if (i != this->rows - 1) o << row_inter_symbol;
	}

}

template<typename MP, template<typename M> typename MM>
std::ostream& operator<< (std::ostream& o, const MyMat_Solve_Name<MP, MM>& mat)
{
	mat.Print(o, '\n', ", ");
	return o;
}

/*==================四则运算（amp,cpu通用）====================*/

#define DATA_OPERATOR_CALCULATE(a, b, opt) for(int i = 0; i < (a).length; i++) (a).at(i) opt (b).at(i);

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator+=(const ThisTy& mat)
AMP_CPU ->ThisTy &
{
	DATA_OPERATOR_CALCULATE(*this, mat, +=); 
	return *this;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator-=(const ThisTy& mat)
AMP_CPU ->ThisTy &
{
	DATA_OPERATOR_CALCULATE(*this, mat, -=);
	return *this;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator*=(const ThisTy& mat)
AMP_CPU ->ThisTy &
{
	DATA_OPERATOR_CALCULATE(*this, mat, *=);
	return *this;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator/=(const ThisTy& mat)
AMP_CPU ->ThisTy &
{
	DATA_OPERATOR_CALCULATE(*this, mat, /=); 
	return *this;
}

template<typename MP, template<typename M> typename MM> inline
auto operator-(MyMat_Solve_Name<MP, MM> mat)
AMP_CPU ->decltype(mat)
{
	DATA_OPERATOR_CALCULATE(mat, mat, = -);
#ifndef GENERAL_GPU_CPU
	return std::move(mat);
#else
	return mat;
#endif
}

#undef DATA_OPERATOR_CALCULATE

#define DATA_OPERATOR_CALCULATE_L(mat, dat, opt) for(int i = 0; i < (mat).length; i++) (mat).at(i) = (mat).at(i) opt (dat);
#define DATA_OPERATOR_CALCULATE_R(dat, mat, opt) for(int i = 0; i < (mat).length; i++) (mat).at(i) = (dat) opt (mat).at(i);

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator+=(DataTy dat)
AMP_CPU ->ThisTy &
{
	DATA_OPERATOR_CALCULATE_L(*this, dat, +=);
	return *this;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator-=(DataTy dat)
AMP_CPU ->ThisTy &
{
	DATA_OPERATOR_CALCULATE_L(*this, dat, -=);
	return *this;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator*=(DataTy dat)
AMP_CPU ->ThisTy &
{
	DATA_OPERATOR_CALCULATE_L(*this, dat, *=);
	return *this;
}

template<typename MatProperty, template<typename M> typename MatrixMul> inline
auto MyMat_Solve_Name<MatProperty, MatrixMul>::operator/=(DataTy dat)
AMP_CPU ->ThisTy &
{
	DATA_OPERATOR_CALCULATE_L(*this, dat, /=);
	return *this;
}

template<typename Ty, typename MP, template<typename M> typename MM>
MyMat_Solve_Name<MP, MM> operator+(const MyMat_Solve_Name<MP, MM>& mat, Ty dat)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> res(mat);
	DATA_OPERATOR_CALCULATE_L(res, dat, +);
#ifndef GENERAL_GPU_CPU
	return std::move(res);
#else
	return res;
#endif
}

template<typename Ty, typename MP, template<typename M> typename MM>
MyMat_Solve_Name<MP, MM> operator+(Ty dat, const MyMat_Solve_Name<MP, MM>& mat)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> res(mat);
	DATA_OPERATOR_CALCULATE_R(dat, res, +);
#ifndef GENERAL_GPU_CPU
	return std::move(res);
#else
	return res;
#endif
}

template<typename Ty, typename MP, template<typename M> typename MM>
MyMat_Solve_Name<MP, MM> operator-(const MyMat_Solve_Name<MP, MM>& mat, Ty dat)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> res(mat);
	DATA_OPERATOR_CALCULATE_L(res, dat, -);
#ifndef GENERAL_GPU_CPU
	return std::move(res);
#else
	return res;
#endif
}

template<typename Ty, typename MP, template<typename M> typename MM>
MyMat_Solve_Name<MP, MM> operator-(Ty dat, const MyMat_Solve_Name<MP, MM>& mat)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> res(mat);
	DATA_OPERATOR_CALCULATE_R(dat, res, -);
#ifndef GENERAL_GPU_CPU
	return std::move(res);
#else
	return res;
#endif
}

template<typename Ty, typename MP, template<typename M> typename MM>
MyMat_Solve_Name<MP, MM> operator*(const MyMat_Solve_Name<MP, MM>& mat, Ty dat)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> res(mat);
	DATA_OPERATOR_CALCULATE_L(res, dat, *);
#ifndef GENERAL_GPU_CPU
	return std::move(res);
#else
	return res;
#endif
}

template<typename Ty, typename MP, template<typename M> typename MM>
MyMat_Solve_Name<MP, MM> operator*(Ty dat, const MyMat_Solve_Name<MP, MM>& mat)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> res(mat);
	DATA_OPERATOR_CALCULATE_R(dat, res, *);
#ifndef GENERAL_GPU_CPU
	return std::move(res);
#else
	return res;
#endif
}

template<typename Ty, typename MP, template<typename M> typename MM>
MyMat_Solve_Name<MP, MM> operator/(const MyMat_Solve_Name<MP, MM>& mat, Ty dat)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> res(mat);
	DATA_OPERATOR_CALCULATE_L(res, dat, /);
#ifndef GENERAL_GPU_CPU
	return std::move(res);
#else
	return res;
#endif
}

template<typename Ty, typename MP, template<typename M> typename MM>
MyMat_Solve_Name<MP, MM> operator/(Ty dat, const MyMat_Solve_Name<MP, MM>& mat)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> res(mat);
	DATA_OPERATOR_CALCULATE_R(dat, res, /);
#ifndef GENERAL_GPU_CPU
	return std::move(res);
#else
	return res;
#endif
}

#undef DATA_OPERATOR_CALCULATE_L
#undef DATA_OPERATOR_CALCULATE_R



template<typename MP, template<typename M> typename MM> inline
MyMat_Solve_Name<MP, MM> operator+(const MyMat_Solve_Name<MP, MM>& mat1, const MyMat_Solve_Name<MP, MM>& mat2)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> mat(mat1);
	mat += mat2;
#ifndef GENERAL_GPU_CPU
	return std::move(mat);
#else
	return mat;
#endif
}

template<typename MP, template<typename M> typename MM> inline
MyMat_Solve_Name<MP, MM> operator-(const MyMat_Solve_Name<MP, MM>& mat1, const MyMat_Solve_Name<MP, MM>& mat2)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> mat(mat1);
	mat -= mat2;
#ifndef GENERAL_GPU_CPU
	return std::move(mat);
#else
	return mat;
#endif
}

template<typename MP, template<typename M> typename MM> inline
MyMat_Solve_Name<MP, MM> operator*(const MyMat_Solve_Name<MP, MM>& mat1, const MyMat_Solve_Name<MP, MM>& mat2)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> mat(mat1);
	mat *= mat2;
#ifndef GENERAL_GPU_CPU
	return std::move(mat);
#else
	return mat;
#endif
}

template<typename MP, template<typename M> typename MM> inline
MyMat_Solve_Name<MP, MM> operator/(const MyMat_Solve_Name<MP, MM>& mat1, const MyMat_Solve_Name<MP, MM>& mat2)
AMP_CPU
{
	MyMat_Solve_Name<MP, MM> mat(mat1);
	mat /= mat2;
#ifndef GENERAL_GPU_CPU
	return std::move(mat);
#else
	return mat;
#endif
}

/*==================其他运算=================================*/

template<typename MatProperty, template<typename M> typename MatrixMul> inline
void MyMat_Solve_Name<MatProperty, MatrixMul>::CopyForm(const BaseTy& src)
{
	operator= (src);
}

#undef MyMat_Solve_Name
