#pragma once
#include <iostream>

#define min(a,b)            (((a) < (b)) ? (a) : (b))

template <class T>
struct MyArray {
private:
	bool destruct = true;		//是否析构：若数据指针arr是由Load函数加载的外部指针，由于申请内存不在类内部进行，释放内存也不应在类内部进行

	inline static void CopyElem(T* dst, T*src, size_t size) {
		for (size_t i = 0; i < size; i++) *(dst++) = *(src++);
	}
	inline int Periodic_func(int x)const { return int(x + abs(x / int(len) - 1) * len) % len; }	//函数返回值基于x并限制在区间[0, size)内
	//通用交换函数
	template <class DataType>
	static inline void Swap(DataType& i, DataType& j) { DataType temp = i; i = j; j = temp; }
	//Shell插入排序
	template <class DataType>
	static inline void SheSort(DataType v[], size_t length, int(*compare)(const DataType&, const DataType&)) {
		for (int space = length / 4 - 1; space > 0; space /= 2) {			//间距逐渐缩小，初始值为总长度的三分之一
			for (int i = 0; i < space; i++) {								//一个区间中起始点i往前推，最终不到达第二个区间
				for (int j = 0; i + j * space < length; j++) {			//间距确定的直接插入法
					for (int k = j - 1; k >= 0; k--) {
						if (compare(v[i + (k + 1)*space], v[i + k * space]) < 0)
							Swap(v[i + (k + 1)*space], v[i + k * space]);
						else break;
					}
				}
			}
		}
	}
protected:
public:
	size_t len = 0;
	T *arr = nullptr;
	MyArray(size_t len = 0) :len(len){ if (len > 0) arr = new T[len]; }
	MyArray(const MyArray<T>& v) { *this = v; }
	MyArray(MyArray<T>&& v) { *this = (MyArray<T>&&)v; }
	~MyArray() { if (arr && destruct) delete[] arr; }

	inline size_t Len()const { return len; }

	T* operator + (int index) { return arr + index; }
	const T* operator + (int index)const { return arr + index; }
	T& operator [] (int index) { return arr[index]; }
	const T& operator [] (int index)const { return arr[index]; }

	T& CirIndex(int index) { return arr[Periodic_func(index)]; }
	const T& CirIndex(int index)const { return arr[Periodic_func(index)]; }

	MyArray<T>& operator = (const MyArray<T>& v) {
		if (this != &v) {
			Resize(v.len);
			CopyElem(arr, v.arr, v.len);   //memcpy(arr, v.arr, sizeof(T)*v.len);
			len = v.len;
		}
		return *this;
	}
	MyArray<T>& operator = (MyArray<T>&& v) {
		if (this != &v) {
			if (arr && destruct) delete[] arr;
			len = v.len;
			arr = v.arr;
			v.arr = nullptr;
		}
		return *this;
	}

	MyArray<T>& operator = (std::initializer_list<T> il) {
		size_t ilSize = il.size();
		for (size_t i = 0; i < len; i++) {
			if (i < ilSize) arr[i] = *(il.begin() + i);
			else arr[i] = 0;
		}
		return *this;
	}

	inline void Load(T* arr, size_t len) {				//加载一个外部指针，将实例绑定到该指针上，析构不释放该指针内存
		this->len = len;
		if (this->arr && destruct) delete[] arr;
		this->arr = arr;
		destruct = false;
	}

	inline void Set(T e) {
		T* p = arr;
		for (int i = len; i > 0; i--) *(p++) = e;
	}

	inline void Resize(size_t len) {
		if (len == this->len) return;

		if (arr && destruct) delete[] arr;
		this->len = len;
		if (this->len) arr = new T[len];
		else arr = nullptr;
	}
	inline void CopyFrom(const MyArray<T>& src,int length, size_t begin_dst = 0,size_t begin_src = 0) {
		if (!arr || !src.arr || begin_dst >= len || begin_src >= src.len || length <= 0) return;
		length = min(min(length, len - begin_dst), src.len - begin_src);
		CopyElem(arr + begin_dst, (T*)src.arr + begin_src, length);
	}
	
	inline void ShellSort(size_t begin, size_t lenght) {
		SheSort<T>(arr + begin, lenght,
			[](const T& elem1, const T& elem2) {
				return int(elem1 > elem2);
			});
	}

	inline void ShellSort(size_t begin, size_t lenght, int(*compare)(const T&, const T&)) {
		SheSort<T>(arr + begin, lenght, compare);
	}

	inline void Sort(size_t begin, size_t lenght) {
		qsort(arr + begin, lenght, sizeof(T), 
			[](const void* elem1, const void* elem2) {
				return int(*(T*)elem1 > *(T*)elem2); 
			});
	}
	inline void Sort(size_t begin, size_t lenght, int (*compare)(const void*, const void*)) { 
		qsort(arr + begin, lenght, sizeof(T), compare);
	}
};

template <class T>
struct MyVector {
private:
	inline int Periodic_func(int x)const { return (x + abs(x / size - 1) * size) % size; }	//函数返回值基于x并限制在区间[0, size)内

public:

	int size = 0;
	MyArray<T> arr;

	MyVector(size_t maxLen = 10) :arr(maxLen) { ; }
	MyVector(const MyVector<T>& v) { *this = v; }
	MyVector(MyVector<T>&& v) { *this = (MyVector<T>&&)v; }
	MyVector(const MyArray<T>& arr) :size(arr.Len()), arr (arr) { ; }
	MyVector(MyArray<T>&& arr) :size(arr.Len()), arr(std::move(arr)) { ; }

	MyVector<T>& operator = (const MyVector<T>& v) {
		if (this != &v) {
			arr.Resize(v.arr.Len());
			arr.CopyFrom(v.arr, v.size);
			size = v.size;
		}
		return *this;
	}
	MyVector<T>& operator = (MyVector<T>&& v) {
		if (this != &v) {
			size = v.size;
			arr = std::move(v.arr);
		}
		return *this;
	}

	T& operator [] (int index) { return arr[index]; }
	const T& operator [] (int index)const { return arr[index]; }

	T& CirIndex(int index) { return arr[Periodic_func(index)]; }
	const T& CirIndex(int index)const { return arr[Periodic_func(index)]; }

	template<class printT>
	inline void CustomPrint(std::ostream& o, printT intervalSymbol) {
		o << '[' << intervalSymbol;
		for (int i = 0; i < size; i++) {
			o << arr[i] << intervalSymbol;
		}
		o << "]";
	}

	friend std::ostream& operator << (std::ostream& o,const MyVector& v) {
		v.CustomPrint(o, ' ');
		return o;
	}

	inline void join_back(const MyVector<T>& v) {
		if (size + v.size > arr.Len()) Dilatation();
		arr.CopyFrom(v.arr, v.size, size, 0);
		size += v.size;
	}

	template <class T2>
	inline void push_back(const T2& element) {
		if (size >= arr.Len()) Dilatation();
		arr[size++] = element;
	}

	template <class T2>
	inline void push_back(T2&& element) {
		if (size >= arr.Len()) Dilatation();
		arr[size++] = std::move(element);
	}

	inline void Delete(int index) {
		if (index >= size) return;
		size--;
		if (index == size) return;

		arr.CopyFrom(arr, size - index, index, index + 1);
	}
	inline void Fill() { size = arr.Len(); }
	inline void Clear() { size = 0; }
	inline void Resize(size_t maxLen) {
		Clear();

		arr.Resize(maxLen);
	}
	inline void Copy(const MyVector<T> src) {
		if (&src != this) {
			if (src.size > arr.Len()) arr.Resize(src.size);
		
			size = src.size;
			arr.CopyFrom(src.arr, src.size);
		}
	}

	inline void Sort() { arr.Sort(0, size); }
	inline void Sort(int(*compare)(const void*, const void*)) { arr.Sort(0, size, compare); }
	inline void ShellSort() { arr.ShellSort(0, size); }
	inline void ShellSort(int(*compare)(const T&, const T&)) { arr.ShellSort(0, size, compare); }

private:

	inline void Dilatation(size_t maxLen = 0) {
		//std::cout << "MyVector: Dilatation!" << std::endl;
		if (maxLen > 0 && maxLen <= arr.Len())return;
		MyArray<T> tmp = std::move(arr);
		if (maxLen == 0) {
			if (arr.Len()) maxLen = arr.Len() * 2;
			else maxLen = 1;
		}
		arr.Resize(maxLen);
		arr.CopyFrom(tmp, tmp.Len());		//memcpy(arr, temp, sizeof(T)*size);
	}
};
