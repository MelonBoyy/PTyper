#include "ptyper.h"
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
		if ( !GetText() ) return;

		char _currentCharacter = GetText()[index];

		printf( "%c", _currentCharacter );
		fflush( stdout );

		if ( onCharacterTyped ) onCharacterTyped( index, _currentCharacter, GetDelay( _currentCharacter ) );
	}

	const size_t Line::GetLength( void ) const
	{
		return _length;
	}

	const char* Line::GetText( void ) const
	{
		return _text;
	}

	const Delay Line::GetDelay( char c ) const
	{
		DelayMap _currentDelayMap = *CurrentDelayMap;
		bool _hasC = UnorderedMapContainsValue<char, Delay>( _currentDelayMap, c );

		if ( _hasC ) return _currentDelayMap.at( c );
		else return _baseDelay;
	}

	void Line::TypeLine( void )
	{
		for ( size_t i = 0; i < _length; i++ )
		{
			Delay _currentDelay = GetDelay( GetText()[i] );

			PrintChar( i );
			SleepMilliseconds( _currentDelay );
		}
	}

	Line::Line( void ) {}

	Line::Line( Delay baseDelay, char* text, size_t size )
	{
		_baseDelay = baseDelay;
		_text = new char[size];
		_length = size;

		for ( size_t i = 0; i < size; i++ )
		{
			*( _text + i ) = *( text + i );
		}
	}

	Line::Line( Delay baseDelay, std::string text ) : Line( baseDelay, &text[0], text.size() ) {}

	Line::~Line()
	{
		SafeDelete( _text );
	}
}