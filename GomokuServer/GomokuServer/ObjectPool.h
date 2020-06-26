#pragma once

template <class T>
class ObjectPool
{
public:

	enum
	{
		POOL_MAX_SIZE = 1024, ///< must be power of 2
		POOL_SIZE_MASK = POOL_MAX_SIZE - 1
	};

	/// memory pre- allocation is just optional :)
	static void PrepareAllocation()
	{
		for (int i = 0; i < POOL_MAX_SIZE; ++i)
			mPool[i] = _aligned_malloc(sizeof(T), MEMORY_ALLOCATION_ALIGNMENT);

		InterlockedAdd64(&mTailPos, POOL_MAX_SIZE);
	}

	static void* operator new(size_t objSize)
	{
		LONGLONG popPos = InterlockedIncrement64(&mHeadPos) - 1;

		void* popVal = InterlockedExchangePointer(&mPool[popPos & POOL_SIZE_MASK], nullptr);
		if (popVal != nullptr)
			return popVal;

		return _aligned_malloc(objSize, MEMORY_ALLOCATION_ALIGNMENT);;
	}

	static void	operator delete(void* obj)
	{
		LONGLONG insPos = InterlockedIncrement64(&mTailPos) - 1;

		void* prevVal = InterlockedExchangePointer(&mPool[insPos & POOL_SIZE_MASK], obj);
		if (prevVal != nullptr)
			_aligned_free(prevVal);
	}


private:

	static PVOID volatile mPool[POOL_MAX_SIZE];
	static LONGLONG volatile mHeadPos;
	static LONGLONG volatile mTailPos;

	void* __powerof2check__[((POOL_MAX_SIZE & POOL_SIZE_MASK) == 0x0) & 0x1];
};

template <class T>
PVOID volatile ObjectPool<T>::mPool[POOL_MAX_SIZE] = {};

template <class T>
LONGLONG volatile ObjectPool<T>::mHeadPos(0);

template <class T>
LONGLONG volatile ObjectPool<T>::mTailPos(0);

