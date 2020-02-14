#pragma once

#ifdef _DEBUG
//#define ERROR_ASSERT
#define ERROR_REPORT
#define WARNING_REPORT
#endif

#ifndef _MyMat_Solve_HPP_
#define _MyMat_Solve_HPP_

#undef GENERAL_GPU_CPU
#include <MyMat_Solve.hpp>
#undef MyMat_Solve_Name
#endif

#ifndef _MyMat_Solve_AMP_HPP_
#define _MyMat_Solve_AMP_HPP_

#define GENERAL_GPU_CPU
#include <MyMat_Solve.hpp>
#undef MyMat_Solve_Name

#endif


#undef AMP_CPU
#define AMP_CPU restrict(amp, cpu)


template<typename Ty, int m, int n>
struct MyMat_Reference_Static
{
public:
	using ThisTy = MyMat_Reference_Static<Ty, m, n>;
	using DataTy = Ty;
	static constexpr int rows		= m;
	static constexpr int cols		= n;
	static constexpr int length	= m * n;
public:
	DataTy* data[rows] = { 0 };

	DataTy& at(int x) { return data[x / cols][x % cols]; }
	const DataTy& at(int x) const { return data[x / cols][x % cols]; }
	DataTy& at(int row, int col) { return data[row][col]; }
	const DataTy& at(int row, int col) const { return data[row][col]; }
};
template<typename DataTy, int m, int n>
using MyRoiMatx = MyMat_Solve<MyMat_Reference_Static<DataTy, m, n>>;

template<typename Ty, int m, int n>
struct MyMat_Data_Static
{
public:
	using ThisTy = MyMat_Data_Static<Ty, m, n>;
	using DataTy = Ty;
	static constexpr int rows		= m;
	static constexpr int cols		= n;
	static constexpr int length	= m * n;

	template<int h, int w> using RefePorpTy = MyRoiMatx<Ty, h, w>;
public:
	DataTy data[length];

	DataTy& at(int x) AMP_CPU { return data[x]; }
	const DataTy& at(int x) const AMP_CPU { return data[x]; }
	DataTy& at(int row, int col) AMP_CPU { return data[row * cols + col]; }
	const DataTy& at(int row, int col) const AMP_CPU { return data[row * cols + col]; }

	template<int h, int w>
	RefePorpTy<h, w> GetROI(int row, int col)
	{
		ERROR_CHECK(row + h <= this->rows && col + w <= this->cols, "MyMat_Data Error: GetROI overflow!");
		RefePorpTy<h, w> roi;
		for (int i = 0; i < h && i + row < this->rows; i++)
		{
			roi.data[i] = &(this->at(row + i, col));
		}
		return roi;
	}

	RefePorpTy<1, cols> Row(int row) { return GetROI<1, cols>(row, 0); }
	RefePorpTy<rows, 1> Col(int col) { return GetROI<rows, 1>(0, col); }
};


template<typename MatProperty>
struct StaticMatrixMul {
public:
	using PropTy = MatProperty;
	using SupeTy = MyMat_Solve<MatProperty, StaticMatrixMul>;
	using DataTy = decltype(MatProperty::DataTy(0));

public:
	template<template<typename Ty, int m, int n> typename Property, int n1>
	static auto mul(const MatProperty& left, const Property<DataTy, MatProperty::cols, n1>& right) /*AMP_CPU*/
	{
		MyMat_Solve<MyMat_Data_Static<DataTy, MatProperty::rows, n1>> res;
		res = 0;
		for (int i = 0; i < res.rows; i++)
			for (int j = 0; j < res.cols; j++)
				for (int k = 0; k < MatProperty::cols; k++)
					res.at(i, j) += left.at(i, k) * right.at(k, j);
		return res;
	}
	static void mul_amp(const MatProperty&, void*) /*AMP_CPU*/
	{
		;
	}
};


template<typename DataTy, int m, int n>
using MyMatx_AMP = MyMat_Solve_AMP<MyMat_Data_Static<DataTy, m, n>, StaticMatrixMul>;

template<typename DataTy, int m, int n>
using MyMatx = MyMat_Solve<MyMat_Data_Static<DataTy, m, n>, StaticMatrixMul>;


template<typename Ty>
struct MyMat_Reference_Dynamic
{
public:
	using ThisTy = MyMat_Reference_Dynamic<Ty>;
	using DataTy = Ty;
public:
	int rows = 0;
	int cols = 0;
	int length = 0;
	DataTy** data = nullptr;
	MyMat_Reference_Dynamic() { ; }
	~MyMat_Reference_Dynamic() { if(data) delete[] data; }
	MyMat_Reference_Dynamic(int rows, int cols) :rows(rows), cols(cols), length(rows* cols) { data = new DataTy*[rows]; }

	//【拷贝赋值】
	ThisTy& operator=  (const ThisTy& mat_cRef) = delete;

	//【拷贝构造】
	MyMat_Reference_Dynamic(const ThisTy& mat_cRef) :ThisTy(mat_cRef.rows, mat_cRef.cols)
	{
		memcpy(data, mat_cRef.data, rows * sizeof(DataTy*));
	}

	//【移动赋值】
	ThisTy& operator=  (ThisTy&& mat_rRef)
	{
		if (this != &mat_rRef)
		{
			if (data) delete[] data;
			rows = mat_rRef.rows;
			cols = mat_rRef.cols;
			length = mat_rRef.length;
			data = mat_rRef.data;
			mat_rRef.data = nullptr;
		}
		return *this;
	}
	MyMat_Reference_Dynamic(ThisTy&& mat_cRef) noexcept { operator = (std::move(mat_cRef)); }

	DataTy& at(int x) { return data[x / cols][x % cols]; }
	const DataTy& at(int x) const { return data[x / cols][x % cols]; }
	DataTy& at(int row, int col) { return data[row][col]; }
	const DataTy& at(int row, int col) const { return data[row][col]; }
};
template<typename DataTy>
using MyRoiMat = MyMat_Solve<MyMat_Reference_Dynamic<DataTy>>;


template<typename Ty>
struct MyMat_Data_Dynamic
{
public:
	using ThisTy = MyMat_Data_Dynamic<Ty>;
	using DataTy = Ty;

	using RefePorpTy = MyRoiMat<Ty>;
public:
	int rows = 0;
	int cols = 0;
	int length = 0;
	DataTy* data = nullptr;

	MyMat_Data_Dynamic() { ; }
	~MyMat_Data_Dynamic() { if (data) delete[] data; }
	MyMat_Data_Dynamic(int rows, int cols) :rows(rows), cols(cols) , length(rows * cols) { data = new DataTy[length]; }
	//【拷贝赋值】
	ThisTy& operator=  (const ThisTy& mat_cRef) = delete;
	//【拷贝构造】
	MyMat_Data_Dynamic(const ThisTy& mat_cRef) :ThisTy(mat_cRef.rows, mat_cRef.cols)
	{
		memcpy(data, mat_cRef.data, length * sizeof(DataTy));
	}
	//【移动赋值】
	ThisTy& operator=  (ThisTy&& mat_rRef)
	{
		if (this != &mat_rRef)
		{
			if (data) delete[] data;
			rows = mat_rRef.rows;
			cols = mat_rRef.cols;
			length = mat_rRef.length;
			data = mat_rRef.data;
			mat_rRef.data = nullptr;
		}
		return *this;
	}
	MyMat_Data_Dynamic(ThisTy&& mat_cRef) noexcept { operator = (std::move(mat_cRef)); }

	DataTy& at(int x) { return data[x]; }
	const DataTy& at(int x) const { return data[x]; }
	DataTy& at(int row, int col) { return data[row * cols + col]; }
	const DataTy& at(int row, int col) const { return data[row * cols + col]; }

	RefePorpTy GetROI(int i, int j, int rows, int cols)
	{
		ERROR_CHECK(rows + i <= this->rows && cols + j <= this->cols, "MyMat_Data Error: GetROI overflow!");
		RefePorpTy roi(rows, cols);
		for (int idx = 0; idx < rows && idx + i < this->rows; idx++)
		{
			roi.data[idx] = &(this->at(i + idx, j));
		}
		return std::move(roi);
	}

	RefePorpTy Row(int row) { return GetROI(row, 0, 1, cols); }
	RefePorpTy Col(int col) { return GetROI(0, col, rows, 1); }

};
template<typename MatProperty>
struct DynamicMatrixMul {
public:
	using PropTy = MatProperty;
	using SupeTy = MyMat_Solve<MatProperty, DynamicMatrixMul>;
	using DataTy = decltype(MatProperty::DataTy(0));

public:
	template<template<typename Ty> typename Property>
	static auto mul(const MatProperty& left, const Property<DataTy>& right) /*AMP_CPU*/
	{
		MyMat_Solve<MyMat_Data_Dynamic<DataTy>> res(left.rows, right.cols);
		res = 0;
		for (int i = 0; i < res.rows; i++)
			for (int j = 0; j < res.cols; j++)
				for (int k = 0; k < left.cols; k++)
					res.at(i, j) += left.at(i, k) * right.at(k, j);
		return std::move(res);
	}
	static void mul_amp(const MatProperty&, void*) /*AMP_CPU*/
	{
		;
	}
};
template<typename DataTy>
using MyMat = MyMat_Solve<MyMat_Data_Dynamic<DataTy>, DynamicMatrixMul>;
