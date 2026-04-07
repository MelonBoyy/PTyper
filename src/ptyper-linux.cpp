#ifdef __linux__

#include "ptyper.h"
#include <chrono>

namespace PAL::TYPER
{
	static inline void SleepMilliseconds( uint16_t milliseconds )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
	}

	static DelayMap* __DefaultDelayMap = new DelayMap
	{
		{ '.', 1000 },
		{ '!', 1000 },
		{ '?', 1000 },
		{ ':', 500 },
		{ ';', 500 },
		{ ',', 250 },
		{ '-', 0 },
		{ '\n', 0 }
	};
	static DelayMap* CurrentDelayMap = __DefaultDelayMap;

	const DelayMap GetDelayMap( void )
	{
		return *CurrentDelayMap;
	}

	void SetDelayMap( DelayMap delayMap )
	{
		*CurrentDelayMap = delayMap;
	}
	void ResetDelayMap( void )
	{
		CurrentDelayMap = __DefaultDelayMap;
	}

	void Line::PrintChar( size_t index )
	{
		if ( GetText() == nullptr ) return;

		printf( "%c", GetText()[index] );
		fflush( stdout );
	}

	const size_t Line::GetLength( void ) const
	{
		return _length;
	}

	const char* Line::GetText( void ) const
	{
		return _text;
	}

	const uint16_t Line::GetDelay( char c ) const
	{
		DelayMap _currentDelayMap = *CurrentDelayMap;
		bool _hasC = _currentDelayMap.count( c ) == 1;

		if ( _hasC ) return _currentDelayMap.at( c );
		else return _baseDelay;
	}

	void Line::TypeLine( void )
	{
		for ( size_t i = 0; i < _length; i++ )
		{
			uint16_t _currentDelay = GetDelay( GetText()[i] );

			PrintChar( i );
			SleepMilliseconds( _currentDelay );
		}
	}

	Line::Line( void )
	{
		_baseDelay = 20;
		_text = new char[7] { "Sample" };
		_length = 7;
	}

	Line::Line( uint16_t baseDelay, char* text, size_t size )
	{
		_baseDelay = baseDelay;
		_text = new char[size];
		_length = size;

		for ( size_t i = 0; i < size; i++ )
		{
			*( _text + i ) = *( text + i );
		}
	}

	Line::Line( uint16_t baseDelay, std::string text ) : Line( baseDelay, &text[0], text.size() ) {}

	Line::Line( uint16_t baseDelay, std::stringstream textStream ) : Line( baseDelay, textStream.str() ) {}

	Line::~Line()
	{
		if ( _text != nullptr ) delete _text;
	}

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

		if ( _currentTimerThread->joinable() ) _currentTimerThread->join();

		delete _currentTime;
		delete _currentTimerThread;
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

		if ( onBegin != nullptr ) onBegin();
	}

	void ThreadedLine::EndTyping( void )
	{
		if ( !IsTyping() ) return;
		_isTyping = false;

		EndTimer();
		if ( _currentTypingThread->joinable() ) _currentTypingThread->join();

		delete _currentTypingThread;

		if ( onEnd != nullptr ) onEnd();
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

		if ( onCancel != nullptr ) onCancel();
	}

	void ThreadedLine::Join( void )
	{
		if ( !IsTyping() ) return;

		if ( _currentTypingThread == nullptr ) return;
		if ( _currentTypingThread->joinable() ) _currentTypingThread->join();

		EndTyping();
	}

	ThreadedLine::ThreadedLine( void ) : Line()
	{
		_currentTime = 0;
		_currentTimerThread = nullptr;
	}

	ThreadedLine::ThreadedLine( uint16_t baseDelay, char* text, size_t size ) : Line( baseDelay, text, size ) {}

	ThreadedLine::ThreadedLine( uint16_t baseDelay, std::string text ) : Line( baseDelay, text ) {}

	ThreadedLine::ThreadedLine( uint16_t baseDelay, std::stringstream textStream ) : Line( baseDelay, textStream.str() ) {}

	ThreadedLine::~ThreadedLine()
	{
		Join();
	}
}

#endif