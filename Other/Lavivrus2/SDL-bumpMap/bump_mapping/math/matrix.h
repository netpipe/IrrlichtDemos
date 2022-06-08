#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <string>

// m - number of rows
// n - number of colums
template<unsigned int m, unsigned int n, class T = float>
class matrix
{
public:

	// Constructor
	matrix();
	matrix(const T & rhs);
	matrix(const matrix<m,n,T> & rhs);
	matrix(T const * const rhs);

	// Operators
	void operator += (const matrix<m,n,T> & rhs);
	void operator -= (const matrix<m,n,T> & rhs);

	template<class U>
	void operator *= (const U rhs)
	{
		for (unsigned int i = 0; i < m*n; i++)
			element[i] *= rhs;				
	}

	matrix<m,n,T> & operator = (const matrix<m,n,T> & rhs);

	bool operator == (const matrix<m,n,T> & rhs) const;
	bool operator != (const matrix<m,n,T> & rhs) const;

	matrix<m,n,T> operator - (void) const;

	T & operator [] (const unsigned int i);
	const T & operator [] (const unsigned int i) const;

	T & operator () (const unsigned int i, const unsigned int j);
	const T & operator () (const unsigned int i, const unsigned int j) const;

	// Member functions
	matrix<m,1,T> get_column(const unsigned int i) const;
	matrix<1,n,T> get_row(const unsigned int i) const;
	void set_column(const unsigned int i, const matrix<m,1,T> & rhs);
	void set_row(const unsigned int i, const matrix<1,n,T> & rhs);

	matrix<m-1,n,T> del_row(unsigned int i) const;
	matrix<m,n-1,T> del_column(unsigned int i) const;

	void set_elements(const T * const rhs);

	// Basic transforms
	void bt_change_columns(unsigned int i, unsigned int j);
	void bt_scale_column(unsigned int i, const T & factor);
	void bt_add_column(unsigned int i, unsigned int j, const T & factor);

	void bt_change_rows(unsigned int i, unsigned int j);
	void bt_scale_row(unsigned int i, const T & factor);
	void bt_add_row(unsigned int i, unsigned int j, const T & factor);

	// Matrix information
	//unsigned int defect() const;
	//unsigned int rank() const;
	bool zero_matrix() const;
	T const * const data_pointer() const;

protected:
	T	element[m*n];
};

template<unsigned int m, class T = float>
struct vec
{
	typedef matrix<1,m,T> row;
	typedef matrix<m,1,T> column;
};

template<unsigned int m, class T = float>
struct mat
{
	typedef matrix<m,m,T> square_matrix;
};


typedef mat<2>::square_matrix	mat2;
typedef mat<3>::square_matrix	mat3;
typedef mat<4>::square_matrix	mat4;

typedef vec<2>::column	column_vec2;
typedef vec<3>::column	column_vec3;
typedef vec<4>::column	column_vec4;

typedef vec<2>::row		row_vec2;
typedef vec<3>::row		row_vec3;
typedef vec<4>::row		row_vec4;

typedef vec<2>::column	vec2;
typedef vec<3>::column	vec3;
typedef vec<4>::column	vec4;		

template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T>::matrix()
{
	for (unsigned int i = 0; i < m*n; i++)
		element[i] = 0;
}


template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T>::matrix(const T & rhs)
{
	for (unsigned int i = 0; i < m*n; i++)
		element[i] = rhs;
}


template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T>::matrix(T const * const rhs)
{
	for (unsigned int i = 0; i < m*n; i++)
		element[i] = rhs[i];
}

template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T>::matrix(const matrix<m,n,T> & rhs)
{
	for (unsigned int i = 0; i < m*n; i++)
		element[i] = rhs[i];
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::operator += (const matrix<m,n,T> & rhs)
{
	for (unsigned int i = 0; i < m*n; i++)
		element[i] += rhs[i];
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::operator -= (const matrix<m,n,T> & rhs)
{
	for (unsigned int i = 0; i < m*n; i++)
		element[i] -= rhs[i];
}

template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T> & matrix<m,n,T>::operator = (const matrix<m,n,T> & rhs)
{
	for (unsigned int i = 0; i < m*n; i++)
		element[i] = rhs[i];

	return *this;
}

template<unsigned int m, unsigned int n, class T>
inline bool matrix<m,n,T>::operator == (const matrix<m,n,T> & rhs) const
{
	for (unsigned int i = 0; i < m*n; i++)
		if (element[i] != rhs[i])
			return false;

	return true;
}

template<unsigned int m, unsigned int n, class T>
inline bool matrix<m,n,T>::operator != (const matrix<m,n,T> & rhs) const
{
	return !(rhs == *this);
}

template<unsigned int m, unsigned int n, class T>
inline T & matrix<m,n,T>::operator [] (const unsigned int i)
{
	#ifdef _DEBUG
	if (i >= m*n)
		throw 0;
	#endif
	return element[i];
}

template<unsigned int m, unsigned int n, class T>
inline const T & matrix<m,n,T>::operator [] (const unsigned int i) const
{
	#ifdef _DEBUG
	if (i >= m*n)
		throw 0;
	#endif
	return element[i];
}

template<unsigned int m, unsigned int n, class T>
inline T & matrix<m,n,T>::operator () (const unsigned int i, const unsigned int j)
{
	#ifdef _DEBUG
	if ((i >= m) || (j >= n))
		throw 0;
	#endif
	return element[i*n+j];
}

template<unsigned int m, unsigned int n, class T>
inline const T & matrix<m,n,T>::operator () (const unsigned int i, const unsigned int j) const
{
	#ifdef _DEBUG
	if ((i >= m) || (j >= n))
		throw 0;
	#endif
	return element[i*n+j];
}

template<unsigned int m, unsigned int n, class T>
inline matrix<m,n,T> matrix<m,n,T>::operator - (void) const
{
	matrix<m,n,T> tmp;
	for (unsigned int i = 0; i < m*n; i++)
		tmp[i] = -element[i];

	return tmp;
}

template<unsigned int m, unsigned int n, class T>
inline matrix<m,1,T> matrix<m,n,T>::get_column(const unsigned int i) const
{
	#ifdef _DEBUG
	if (i >= n)
		throw 0;
	#endif

	matrix<m,1,T> tmp;
	for (unsigned int j = 0; j < m; j++)
		tmp[j] = element[j*n + i];
	
	return tmp;
}

template<unsigned int m, unsigned int n, class T>
inline matrix<1,n,T> matrix<m,n,T>::get_row(const unsigned int i) const
{
	#ifdef _DEBUG
	if (i >= m)
		throw 0;
	#endif

	matrix<1,n,T> tmp;
	for (unsigned int j = 0; j < n; j++)
		tmp[j] = element[i*n + j];
	
	return tmp;		
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::set_column(const unsigned int i, const matrix<m,1,T> & rhs)
{
	#ifdef _DEBUG
	if (i >= n)
		throw 0;
	#endif


	for (unsigned int j = 0; j < m; j++)
		element[j*n + i] = rhs[j];
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::set_row(const unsigned int i, const matrix<1,n,T> & rhs)
{
	#ifdef _DEBUG
	if (i >= m)
		throw 0;
	#endif


	for (unsigned int j = 0; j < n; j++)
		element[i*n + j] = rhs[j];
}


template<unsigned int m, unsigned int n, class T>
inline matrix<m,n-1,T> matrix<m,n,T>::del_column(unsigned int i) const
{
	#ifdef _DEBUG
	if (i >= n)
		throw 0;
	#endif


	matrix<m,n-1,T> tmp;
	unsigned int j = 0;
	unsigned int k = 0;
	for (; j < n-1; j++, k++)
	{
		if (j == i)
			k++;
		tmp.set_column(j, get_column(k));
	}

	return tmp;
}

template<unsigned int m, unsigned int n, class T>
inline matrix<m-1,n,T> matrix<m,n,T>::del_row(unsigned int i) const
{
	#ifdef _DEBUG
	if (i >= m)
		throw 0;
	#endif


	matrix<m-1,n,T> tmp;
	unsigned int j = 0;
	unsigned int k = 0;
	for (; j < m-1; j++, k++)
	{
		if (j == i)
			k++;
		tmp.set_row(j, get_row(k));
	}

	return tmp;
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::set_elements(const T * const rhs)
{
	for (unsigned int i = 0; i < m*n; i++)
        element[i] = rhs[i];
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::bt_change_columns(unsigned int i, unsigned int j)
{
	#ifdef _DEBUG
	if ((i >= n) || (j >= n))
		throw 0;
	#endif

	matrix<m,1,T> tmp = get_column(i);
	set_column(i, get_column(j));
	set_column(j, tmp);
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::bt_change_rows(unsigned int i, unsigned int j)
{
	#ifdef _DEBUG
	if ((i >= m) || (j >= m))
		throw 0;
	#endif

	matrix<1,n,T> tmp = get_row(i);
	set_row(i, get_row(j));
	set_row(j, tmp);
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::bt_scale_column(unsigned int i, const T & factor)
{
	#ifdef _DEBUG
	if (i >= n)
		throw 0;
	#endif

	for (unsigned int j = 0; j < m; j++)
		element[j*n+i] *= factor;
		
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::bt_scale_row(unsigned int i, const T & factor)
{
	#ifdef _DEBUG
	if (i >= m)
		throw 0;
	#endif

	for (unsigned int j = 0; j < n; j++)
		element[i*n+j] *= factor;
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::bt_add_column(unsigned int dest, unsigned int src, const T & factor)
{
	#ifdef _DEBUG
	if ((dest >= n) || (src >= n))
		throw 0;
	#endif

	for (unsigned int k = 0; k < m; k++)
		element[k*n+dest] += element[k*n+src]*factor;
}

template<unsigned int m, unsigned int n, class T>
inline void matrix<m,n,T>::bt_add_row(unsigned int dest, unsigned int src, const T & factor)
{
	#ifdef _DEBUG
	if ((dest >= n) || (src >= n))
		throw 0;
	#endif

	for (unsigned int k = 0; k < m; k++)
		element[dest*n+k] += element[src*n+k]*factor;

}

template<unsigned int m, unsigned int n, class T>
inline bool matrix<m,n,T>::zero_matrix() const
{
	for (unsigned int i = 0; i < m*n; i++)
		if (element[i] != static_cast<T> 0)
			return false;

	return true;
}

template<unsigned int m, unsigned int n, class T>
inline T const * const matrix<m,n,T>::data_pointer() const
{ return element; }

#endif