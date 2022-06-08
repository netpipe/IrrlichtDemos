#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

template<class T>
T factorial(T val)
{
	if (val == 0)
		return 1;
	T tmp = 1;
	for (T i = 1; i <= val; i++)
		tmp *= i;

	return tmp;
}

template<class T>
T binomial(T n, T k)
{
	/*k = max(k,n-k);
	T tmp = 1;
	for (T i = k+1; i <= n; i++)
		tmp *= i;

    return tmp / factorial(n-k);*/
	return factorial(n) / (factorial(k) * factorial(n-k));
}

#endif