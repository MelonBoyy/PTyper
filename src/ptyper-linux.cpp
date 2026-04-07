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

	void ThreadedLine::BeginTimer( void )
	{
	}

	void ThreadedLine::EndTimer( void )
	{
	}

	void ThreadedLine::BeginTyping( void )
	{
	}

	void ThreadedLine::EndTyping( void )
	{
	}

	const uint64_t ThreadedLine::GetCurrentTime( void ) const
	{
		return *_currentTime;
	}

	const bool ThreadedLine::IsTyping( void ) const
	{
		return _isTyping;
	}

	void ThreadedLine::PauseTyping( void )
	{
	}

	void ThreadedLine::PlayTyping( void )
	{
	}

	void ThreadedLine::InitiateTyping( void )
	{
	}

	void ThreadedLine::CancelTyping( void )
	{
	}

	void ThreadedLine::Join( void )
	{
	}

	void ThreadedLine::Detatch( void )
	{
	}

	ThreadedLine::ThreadedLine( void ) : Line()
	{
		_currentTime = 0;
		_currentThread = nullptr;
	}

	ThreadedLine::ThreadedLine( uint16_t baseDelay, char* text, size_t size ) : Line( baseDelay, text, size ) {}

	ThreadedLine::ThreadedLine( uint16_t baseDelay, std::string text ) : Line( baseDelay, text ) {}

	ThreadedLine::ThreadedLine( uint16_t baseDelay, std::stringstream textStream ) : Line( baseDelay, textStream.str() ) {}
}

#endif