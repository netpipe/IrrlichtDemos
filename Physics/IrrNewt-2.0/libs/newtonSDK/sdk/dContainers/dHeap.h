/****************************************************************************
*
*  Visual C++ 6.0 created by: Julio Jerez
*
****************************************************************************/
#ifndef __dHeapBase__
#define __dHeapBase__


#include <dContainersStdAfx.h>



template <class OBJECT, class KEY>
class dHeapBase
{
	protected:
	struct RECORD 
	{
		KEY m_key;
		OBJECT m_obj;

		RECORD (KEY key, const OBJECT& obj)
			:m_key(key), m_obj(obj)
		{
		}
	};

	dHeapBase (int maxElements);
	~dHeapBase ();
	

	public:
	void Flush (); 
	KEY MaxValue() const; 
	KEY Value(int i = 0) const;
	int GetCount() const;
	int GetMaxCount() const;
	const OBJECT& operator[] (int i) const;
	int Find (OBJECT &obj);
	int Find (KEY key);

	int m_curCount;
	int m_maxCount;
	bool m_allocated;
	RECORD *m_pool;
};

template <class OBJECT, class KEY>
class dDownHeap: public dHeapBase<OBJECT, KEY>
{
	public:
	dDownHeap (int maxElements);
	dDownHeap (const void *buffer, int sizeInBytes);

	void Pop ();
	void Push (OBJECT &obj, KEY key);
	void Sort ();
	void Remove (int Index);

};

template <class OBJECT, class KEY>
class dUpHeap: public dHeapBase<OBJECT, KEY>
{
	public:
	dUpHeap (int maxElements);
	dUpHeap (const void *buffer, int sizeInBytes);

	void Pop ();
	void Push (OBJECT &obj, KEY key);
	void Sort ();
	void Remove (int Index);

};



template <class OBJECT, class KEY>
dHeapBase<OBJECT,KEY>::dHeapBase (int maxElements)
{
	m_allocated = true;
	m_pool = (RECORD *)malloc (maxElements * sizeof (RECORD));
	m_maxCount = maxElements;
	Flush();
}


template <class OBJECT, class KEY>
dHeapBase<OBJECT,KEY>::~dHeapBase ()
{   
	if (m_allocated == true) {
		free (m_pool);
	}
}


template <class OBJECT, class KEY>
KEY dHeapBase<OBJECT,KEY>::Value(int i) const
{
	return m_pool[i].m_key;
}


template <class OBJECT, class KEY>
int dHeapBase<OBJECT,KEY>::GetCount() const
{ 
	return m_curCount;
}


template <class OBJECT, class KEY>
void dHeapBase<OBJECT,KEY>::Flush () 
{
	m_curCount = 0;

	#ifdef _DEBUG
//	dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_key = KEY (0);
	#endif
}


template <class OBJECT, class KEY>
KEY dHeapBase<OBJECT,KEY>::MaxValue() const 
{
	return m_pool[0].m_key;
}


template <class OBJECT, class KEY>
int dHeapBase<OBJECT,KEY>::GetMaxCount() const
{ 
	return m_maxCount;
}


template <class OBJECT, class KEY>
int dHeapBase<OBJECT,KEY>::Find (OBJECT &obj)
{
	int i;
	// For now let perform a linear search
	// this is efficient if the size of the heap is small
	// ex: m_curCount < 32
	// this will be change to a binary search in the heap should the 
	// the size of the heap get larger than 32
	//	_ASSERTE (m_curCount <= 32);
	for (i = 0; i < m_curCount; i ++) {
		if (m_pool[i].obj == obj) {
			return i;
		}
	}
	return - 1;
}


template <class OBJECT, class KEY>
int dHeapBase<OBJECT,KEY>::Find (KEY key)
{
	int i;
	// ex: m_curCount < 32
	// this will be change to a binary search in the heap shoud the 
	// the size of the heap get larger than 32
	_ASSERTE (m_curCount <= 32);
	for (i = 0; i < m_curCount; i ++)	{
		if (m_pool[i].m_key == key) {
			return i;
		}
	}
	return - 1;
}


template <class OBJECT, class KEY>
const OBJECT& dHeapBase<OBJECT,KEY>::operator[] (int i) const
{ 
	_ASSERTE (i<= m_curCount);
	return m_pool[i].m_obj;
}


// **************************************************************************
//
// down Heap
//
// **************************************************************************
template <class OBJECT, class KEY>
dDownHeap<OBJECT,KEY>::dDownHeap (int maxElements)
	:dHeapBase<OBJECT, KEY> (maxElements)
{
}

template <class OBJECT, class KEY>
dDownHeap<OBJECT,KEY>::dDownHeap (const void *buffer, int sizeInBytes)
	:dHeapBase<OBJECT, KEY> (buffer, sizeInBytes)
{
}


template <class OBJECT, class KEY>
void dDownHeap<OBJECT,KEY>::Push (OBJECT &obj, KEY key)
{
	int i;
	int j;
	_ASSERTE (m_curCount < m_maxCount);
	dHeapBase<OBJECT,KEY>::m_curCount ++;

	for (i = dHeapBase<OBJECT,KEY>::m_curCount; i; i = j) {
		j = i >> 1;
		if (!j || (dHeapBase<OBJECT,KEY>::m_pool[j - 1].m_key > key)) {
			break;
		}
		dHeapBase<OBJECT,KEY>::m_pool[i - 1] = dHeapBase<OBJECT,KEY>::m_pool[j - 1];
	}
	_ASSERTE (i);
	dHeapBase<OBJECT,KEY>::m_pool[i - 1].m_key = key;
	dHeapBase<OBJECT,KEY>::m_pool[i - 1].m_obj = obj;
}


template <class OBJECT, class KEY>
void dDownHeap<OBJECT,KEY>::Remove (int index)
{
	int j;
	int k;

	dHeapBase<OBJECT,KEY>::m_curCount--;
	KEY key (dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_key);
	for (k = index + 1; k <= (dHeapBase<OBJECT,KEY>::m_curCount>>1); k = j) {
		j = k + k;
		if ((j < dHeapBase<OBJECT,KEY>::m_curCount) && 
			(dHeapBase<OBJECT,KEY>::m_pool[j - 1].m_key < dHeapBase<OBJECT,KEY>::m_pool[j].m_key)) {
				j ++;
		}
		if (key >= dHeapBase<OBJECT,KEY>::m_pool[j - 1].m_key) {
			break;
		}
		dHeapBase<OBJECT,KEY>::m_pool[k - 1] = dHeapBase<OBJECT,KEY>::m_pool[j - 1];
	}
	dHeapBase<OBJECT,KEY>::m_pool[k - 1].m_key = key;
	dHeapBase<OBJECT,KEY>::m_pool[k - 1].m_obj = dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_obj;
}

template <class OBJECT, class KEY>
void dDownHeap<OBJECT,KEY>::Pop ()
{
	int j;
	int k;

	dHeapBase<OBJECT,KEY>::m_curCount--;
	KEY key (dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_key);
	for (k = 1; k <= (dHeapBase<OBJECT,KEY>::m_curCount>>1); k = j) {
		j = k + k;
		if ((j < dHeapBase<OBJECT,KEY>::m_curCount) && 
			(dHeapBase<OBJECT,KEY>::m_pool[j - 1].m_key < dHeapBase<OBJECT,KEY>::m_pool[j].m_key)) {
			j ++;
		}
		if (key >= dHeapBase<OBJECT,KEY>::m_pool[j - 1].m_key) {
			break;
		}
		dHeapBase<OBJECT,KEY>::m_pool[k - 1] = dHeapBase<OBJECT,KEY>::m_pool[j - 1];
	}
	dHeapBase<OBJECT,KEY>::m_pool[k - 1].m_key = key;
	dHeapBase<OBJECT,KEY>::m_pool[k - 1].m_obj = dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_obj;
}


template <class OBJECT, class KEY>
void dDownHeap<OBJECT,KEY>::Sort ()
{
	int i;
	int count;

	count = dHeapBase<OBJECT,KEY>::m_curCount;
	for (i = 1; i < count; i ++) {
		KEY key (dHeapBase<OBJECT,KEY>::m_pool[0].m_key);
		OBJECT obj (dHeapBase<OBJECT,KEY>::m_pool[0].m_obj);

		Pop();

		dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_key = key;
		dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_obj = obj;
	}

	dHeapBase<OBJECT,KEY>::m_curCount = count;
	for (i = 0; i < count / 2; i ++) {
		KEY key (dHeapBase<OBJECT,KEY>::m_pool[i].m_key);
		OBJECT obj (dHeapBase<OBJECT,KEY>::m_pool[i].m_obj);

		dHeapBase<OBJECT,KEY>::m_pool[i].m_key = dHeapBase<OBJECT,KEY>::m_pool[count - i - 1].m_key;
		dHeapBase<OBJECT,KEY>::m_pool[i].m_obj = dHeapBase<OBJECT,KEY>::m_pool[count - i - 1].m_obj;

		dHeapBase<OBJECT,KEY>::m_pool[count - i - 1].m_key = key;
		dHeapBase<OBJECT,KEY>::m_pool[count - i - 1].m_obj = obj;
	}
}


// **************************************************************************
//
// down Heap
//
// **************************************************************************
template <class OBJECT, class KEY>
dUpHeap<OBJECT,KEY>::dUpHeap (int maxElements)
	:dHeapBase<OBJECT, KEY> (maxElements)
{
}

template <class OBJECT, class KEY>
dUpHeap<OBJECT,KEY>::dUpHeap (const void *buffer, int sizeInBytes)
	:dHeapBase<OBJECT, KEY> (buffer, sizeInBytes)
{
}



template <class OBJECT, class KEY>
void dUpHeap<OBJECT,KEY>::Push (OBJECT &obj, KEY key)
{
	int i;
	int j;
	_ASSERTE (m_curCount < m_maxCount);
	dHeapBase<OBJECT,KEY>::m_curCount ++;

	for (i = dHeapBase<OBJECT,KEY>::m_curCount; i; i = j) {
		j = i >> 1;
		if (!j || (dHeapBase<OBJECT,KEY>::m_pool[j - 1].m_key < key)) {
			break;
		}
		dHeapBase<OBJECT,KEY>::m_pool[i - 1] = dHeapBase<OBJECT,KEY>::m_pool[j - 1];
	}
	_ASSERTE (i);
	dHeapBase<OBJECT,KEY>::m_pool[i - 1].m_key = key;
	dHeapBase<OBJECT,KEY>::m_pool[i - 1].m_obj = obj;
}


template <class OBJECT, class KEY>
void dUpHeap<OBJECT,KEY>::Sort ()
{
	int i;
	int count;

	count = dHeapBase<OBJECT,KEY>::m_curCount;
	for (i = 1; i < count; i ++) {
		KEY key (dHeapBase<OBJECT,KEY>::m_pool[0].m_key);
		OBJECT obj (dHeapBase<OBJECT,KEY>::m_pool[0].m_obj);

		Pop();

		dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_key = key;
		dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_obj = obj;
	}

	dHeapBase<OBJECT,KEY>::m_curCount = count;
	for (i = 0; i < count / 2; i ++) {
		KEY key (dHeapBase<OBJECT,KEY>::m_pool[i].m_key);
		OBJECT obj (dHeapBase<OBJECT,KEY>::m_pool[i].m_obj);

		dHeapBase<OBJECT,KEY>::m_pool[i].m_key = dHeapBase<OBJECT,KEY>::m_pool[count - i - 1].m_key;
		dHeapBase<OBJECT,KEY>::m_pool[i].m_obj = dHeapBase<OBJECT,KEY>::m_pool[count - i - 1].m_obj;

		dHeapBase<OBJECT,KEY>::m_pool[count - i - 1].m_key = key;
		dHeapBase<OBJECT,KEY>::m_pool[count - i - 1].m_obj = obj;
	}
}


template <class OBJECT, class KEY>
void dUpHeap<OBJECT,KEY>::Remove (int index)
{
	int j;
	int k;

	dHeapBase<OBJECT,KEY>::m_curCount--;
	KEY key (dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_key);
	for (k = index + 1; k <= (dHeapBase<OBJECT,KEY>::m_curCount>>1); k = j) {
		j = k + k;
		if ((j < dHeapBase<OBJECT,KEY>::m_curCount) && 
			(dHeapBase<OBJECT,KEY>::m_pool[j - 1].m_key > dHeapBase<OBJECT,KEY>::m_pool[j].m_key)) {
				j ++;
		}
		if (key <= dHeapBase<OBJECT,KEY>::m_pool[j - 1].m_key) {
			break;
		}
		dHeapBase<OBJECT,KEY>::m_pool[k - 1] = dHeapBase<OBJECT,KEY>::m_pool[j - 1];
	}
	dHeapBase<OBJECT,KEY>::m_pool[k - 1].m_key = key;
	dHeapBase<OBJECT,KEY>::m_pool[k - 1].m_obj = dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_obj;
}


template <class OBJECT, class KEY>
void dUpHeap<OBJECT,KEY>::Pop ()
{
	int j;
	int k;

	dHeapBase<OBJECT,KEY>::m_curCount--;
	KEY key (dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_key);
	for (k = 1; k <= (dHeapBase<OBJECT,KEY>::m_curCount>>1); k = j) {
		j = k + k;
		if ((j < dHeapBase<OBJECT,KEY>::m_curCount) && 
			(dHeapBase<OBJECT,KEY>::m_pool[j - 1].m_key > dHeapBase<OBJECT,KEY>::m_pool[j].m_key)) {
				j ++;
		}
		if (key <= dHeapBase<OBJECT,KEY>::m_pool[j - 1].m_key) {
			break;
		}
		dHeapBase<OBJECT,KEY>::m_pool[k - 1] = dHeapBase<OBJECT,KEY>::m_pool[j - 1];
	}
	dHeapBase<OBJECT,KEY>::m_pool[k - 1].m_key = key;
	dHeapBase<OBJECT,KEY>::m_pool[k - 1].m_obj = dHeapBase<OBJECT,KEY>::m_pool[dHeapBase<OBJECT,KEY>::m_curCount].m_obj;
}


#endif

