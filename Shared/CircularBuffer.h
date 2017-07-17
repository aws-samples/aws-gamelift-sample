#pragma once
#include <memory>
#define OUT

class CircularBuffer
{
public:

	CircularBuffer(size_t capacity) : mBRegionPointer(nullptr), mARegionSize(0), mBRegionSize(0), mCapacity(capacity)
	{
		mBuffer = new char[mCapacity] ;
		mBufferEnd = mBuffer + mCapacity ;
		mARegionPointer = mBuffer ;
	}

	~CircularBuffer()
	{
		delete [] mBuffer ;
	}

	void BufferReset()
	{
		mBRegionPointer = nullptr;
		mARegionSize = 0;
		mBRegionSize = 0;
				
		memset(mBuffer, 0, mCapacity);

		mBufferEnd = mBuffer + mCapacity;
		mARegionPointer = mBuffer;
	}

	bool Peek(OUT char* destbuf, size_t bytes) const;
	bool Read(OUT char* destbuf, size_t bytes);
	bool Write(const char* data, size_t bytes);

	/// 버퍼의 첫부분 len만큼 날리기
	void Remove(size_t len) ;

	size_t GetFreeSpaceSize()
	{
		if ( mBRegionPointer != nullptr )
			return GetBFreeSpace() ;
		else
		{
			/// A 버퍼보다 더 많이 존재하면, B 버퍼로 스위치
			if ( GetAFreeSpace() < GetSpaceBeforeA() )
			{
				AllocateB() ;
				return GetBFreeSpace() ;
			}
			else
				return GetAFreeSpace() ;
		}
	}

	size_t GetStoredSize() const
	{
		return mARegionSize + mBRegionSize ;
	}

	size_t GetContiguiousBytes() const 
	{
		if ( mARegionSize > 0 )
			return mARegionSize ;
		else
			return mBRegionSize ;
	}

	/// 쓰기가 가능한 위치 (버퍼의 끝부분) 반환
	char* GetBuffer() const
	{
		if( mBRegionPointer != nullptr )
			return mBRegionPointer + mBRegionSize ;
		else
			return mARegionPointer + mARegionSize ;
	}

	


	/// 커밋(aka. IncrementWritten)
	void Commit(size_t len)
	{
		if ( mBRegionPointer != nullptr )
			mBRegionSize += len ;
		else
			mARegionSize += len ;
	}

	/// 버퍼의 첫부분 리턴
	char* GetBufferStart() const
	{
		if ( mARegionSize > 0 )
			return mARegionPointer ;
		else
			return mBRegionPointer ;
	}


private:

	void AllocateB()
	{
		mBRegionPointer = mBuffer ;
	}

	size_t GetAFreeSpace() const
	{ 
		return (mBufferEnd - mARegionPointer - mARegionSize) ; 
	}

	size_t GetSpaceBeforeA() const
	{ 
		return (mARegionPointer - mBuffer) ; 
	}


	size_t GetBFreeSpace() const
	{ 
		if (mBRegionPointer == nullptr)
			return 0 ; 

		return (mARegionPointer - mBRegionPointer - mBRegionSize) ; 
	}

private:

	char*	mBuffer ;
	char*	mBufferEnd ;

	char*	mARegionPointer ;
	size_t	mARegionSize ;

	char*	mBRegionPointer ;
	size_t	mBRegionSize ;

	size_t	mCapacity;
} ;
