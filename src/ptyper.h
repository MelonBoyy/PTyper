#ifndef PTYPER_H
#define PTYPER_H

#include <cstdlib>
#include <cstdint>
#include <string>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <initializer_list>

namespace PAL::TYPER
{
	typedef std::unordered_map<char, uint16_t> DelayMap;

	const DelayMap GetDelayMap( void );

	// Parameters:
	// 1 (std::unordered_map<char, uint16_t> || DelayMap) Delay unordered_map to set CurrentDelayMap to
	void SetDelayMap( DelayMap );
	void ResetDelayMap( void );

	struct Line
	{
	protected:
		uint16_t _baseDelay;
		char* _text;
		size_t _length;

		// Parameters:
		// 1 ( size_t ): Index of character to print out
		void PrintChar( size_t );
	public:
		// Parameters:
		// 1 (size_t): Current Index
		// 2 (char): Current Character
		// 3 (uint16_t): Current Delay
		void ( *typingAction )( size_t, char, uint16_t );

		const size_t GetLength( void ) const;
		const char* GetText( void ) const;
		const uint16_t GetDelay( char ) const;

		void TypeLine( void );

		Line( void );

		// Parameters:
		// 1 (uint16_t): Delay
		// 2 (char*): Text
		// 3 (size_t): Length/Size of Text
		Line( uint16_t, char*, size_t );

		// Parameters:
		// 1 (uint16_t): Delay
		// 2 (std::string): Text as string
		Line( uint16_t, std::string );

		// Parameters:
		// 1 (uint16_t): Delay
		// 2 (std::stringstream): Text as stream
		Line( uint16_t, std::stringstream );

		~Line();
	};

	struct ThreadedLine : Line
	{
	protected:
		bool _timerRunning;
		bool _isTyping;
		bool _paused;

		uint64_t* _currentTime;
		std::thread* _currentTimerThread;
		std::thread* _currentTypingThread;

		void BeginTimer( void );
		void EndTimer( void );

		void BeginTyping( void );
		void EndTyping( void );
	public:
		void ( *onBegin )( );
		void ( *onEnd )( );
		void ( *onCancel )( );

		const uint64_t GetCurrentTime( void ) const;
		const bool IsPaused( void ) const;
		const bool IsTyping( void ) const;

		void Pause( void );
		void Play( void );

		void InitiateTyping( void );
		void CancelTyping( void );

		void Join( void );

		ThreadedLine( void );

		// Parameters:
		// 1 (uint16_t): Delay
		// 2 (char*): Text
		// 3 (size_t): Length/Size of Text
		ThreadedLine( uint16_t, char*, size_t );

		// Parameters:
		// 1 (uint16_t): Delay
		// 2 (std::string): Text as string
		ThreadedLine( uint16_t, std::string );

		// Parameters:
		// 1 (uint16_t): Delay
		// 2 (std::stringstream): Text as stream
		ThreadedLine( uint16_t, std::stringstream );

		~ThreadedLine();
	};

	struct SkippableLine : ThreadedLine
	{
	protected:
		bool _skippable;
	public:
		const bool GetSkippable( void ) const;

		void TypeLine( void );
		void SkipLine( void );

		SkippableLine( void );

		// Parameters:
		// 1 (uint16_t): Delay
		// 2 (char*): Text
		// 3 (size_t): Length/Size of Text
		SkippableLine( uint16_t, char*, size_t );

		// Parameters:
		// 1 (uint16_t): Delay
		// 2 (std::string): Text as string
		SkippableLine( uint16_t, std::string );

		// Parameters:
		// 1 (uint16_t): Delay
		// 2 (std::stringstream): Text as stream
		SkippableLine( uint16_t, std::stringstream );
	};

	void TypeLine( Line& );

	template <size_t SIZE>
	void TypeLines( Line[SIZE] );
	void TypeLines( std::initializer_list<Line> );
}

#endif