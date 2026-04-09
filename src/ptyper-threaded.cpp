#include "ptyper-threaded.h"
#include <chrono>

namespace PAL::TYPER
{
	// PACK THIS LATER
	static inline void SleepMilliseconds( Delay milliseconds )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
	}

	template <typename TKey, typename TValue>
	static inline bool UnorderedMapContainsValue( std::unordered_map<TKey, TValue>& unorderedMap, TValue value )
	{
		return unorderedMap.count( value ) == 1;
	}

	static inline bool IsValidThreadPtrAndJoinable( std::thread* threadPtr )
	{
		return threadPtr && threadPtr->joinable();
	}

	template <typename TPtr>
	static inline void SafeDelete( TPtr* ptr )
	{
		if ( ptr ) delete ptr;
	}
	// END

	void ThreadedLine::BeginTimer( void )
	{
		if ( _timerRunning ) return;
		_timerRunning = true;

		_currentTime = new uint64_t{ 0 };
		_currentTimerThread = new std::thread( [this]
			{
				while ( _timerRunning )
				{
					if ( IsPaused() ) continue;

					( *_currentTime )++;
					SleepMilliseconds( 1 );
				}
			} );
	}

	void ThreadedLine::EndTimer( void )
	{
		if ( !_timerRunning ) return;
		_timerRunning = false;

		if ( IsValidThreadPtrAndJoinable( _currentTimerThread ) ) _currentTimerThread->join();

		SafeDelete( _currentTime );
		SafeDelete( _currentTimerThread );
	}

	void ThreadedLine::BeginTyping( void )
	{
		if ( IsTyping() ) return;
		_isTyping = true;

		BeginTimer();
		Play();

		_currentTypingThread = new std::thread( [this]
			{
				uint64_t _nextTime = 0;
				size_t i = 0;
				while ( i < _length && IsTyping() )
				{
					if ( IsPaused() ) continue;
					if ( GetCurrentTime() < _nextTime ) continue;

					PrintChar( i );
					_nextTime += GetDelay( GetText()[i] );
					i++;
				}
			} );

		if ( onBegin ) onBegin();
	}

	void ThreadedLine::EndTyping( void )
	{
		if ( !IsTyping() ) return;
		_isTyping = false;

		EndTimer();
		if ( IsValidThreadPtrAndJoinable( _currentTypingThread ) ) _currentTypingThread->join();

		SafeDelete( _currentTypingThread );

		if ( onEnd ) onEnd();
	}

	const uint64_t ThreadedLine::GetCurrentTime( void ) const
	{
		return *_currentTime;
	}

	const bool ThreadedLine::IsPaused( void ) const
	{
		return _paused;
	}

	const bool ThreadedLine::IsTyping( void ) const
	{
		return _isTyping;
	}

	void ThreadedLine::Pause( void )
	{
		if ( IsPaused() ) return;
		_paused = true;
	}

	void ThreadedLine::Play( void )
	{
		if ( !IsPaused() ) return;
		_paused = false;
	}

	void ThreadedLine::InitiateTyping( void )
	{
		if ( IsTyping() ) return;
		BeginTyping();
	}

	void ThreadedLine::CancelTyping( void )
	{
		if ( !IsTyping() ) return;
		EndTyping();

		if ( onCancel ) onCancel();
	}

	void ThreadedLine::Join( void )
	{
		if ( !IsTyping() ) return;

		if ( !_currentTypingThread ) return;
		if ( _currentTypingThread->joinable() ) _currentTypingThread->join();

		EndTyping();
	}

	ThreadedLine::ThreadedLine( void ) : Line() {}

	ThreadedLine::ThreadedLine( Delay baseDelay, char* text, size_t size ) : Line( baseDelay, text, size ) {}

	ThreadedLine::ThreadedLine( Delay baseDelay, std::string text ) : Line( baseDelay, text ) {}

	ThreadedLine::~ThreadedLine()
	{
		Join();
	}
}
