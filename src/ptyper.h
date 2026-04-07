#ifndef PTYPER_H
#define PTYPER_H

#include <cstdlib>
#include <cstdint>
#include <string>
#include <thread>
#include <unordered_map>
#include <initializer_list>

namespace PAL::TYPER
{
	#define TEXT_BASE_DELAY 20
	#define TEXT_DEFAULT_VALUE "Sample"
	#define TEXT_DEFAULT_LENGTH 7

	typedef uint16_t Delay;
	typedef std::unordered_map<char, Delay> DelayMap;

	const DelayMap GetDelayMap( void );

	// Parameters:
	// 1 (std::unordered_map<char, Delay> || DelayMap) Delay unordered_map to set CurrentDelayMap to
	void SetDelayMap( DelayMap );
	void ResetDelayMap( void );

	class Line
	{
	protected:
		Delay _baseDelay = TEXT_BASE_DELAY;
		char* _text = new char[] { TEXT_DEFAULT_VALUE };
		size_t _length = TEXT_DEFAULT_LENGTH;

		// Parameters:
		// 1 ( size_t ): Index of character to print out
		void PrintChar( size_t );
	public:
		// Parameters:
		// 1 (size_t): Current Index
		// 2 (char): Current Character
		// 3 (Delay): Current Delay
		void ( *onCharacterTyped )( size_t, char, Delay ) = nullptr;

		const size_t GetLength( void ) const;
		const char* GetText( void ) const;
		const Delay GetDelay( char ) const;

		void TypeLine( void );

		Line( void );

		// Parameters:
		// 1 (Delay): Delay
		// 2 (char*): Text
		// 3 (size_t): Length/Size of Text
		Line( Delay, char*, size_t );

		// Parameters:
		// 1 (Delay): Delay
		// 2 (std::string): Text as string
		Line( Delay, std::string );

		~Line();
	};

	class ThreadedLine : Line
	{
	protected:
		bool _timerRunning = false;
		bool _isTyping = false;
		bool _paused = false;

		uint64_t* _currentTime = nullptr;
		std::thread* _currentTimerThread = nullptr;
		std::thread* _currentTypingThread = nullptr;

		void BeginTimer( void );
		void EndTimer( void );

		void BeginTyping( void );
		void EndTyping( void );
	public:
		void ( *onBegin )( void ) = nullptr;
		void ( *onEnd )( void ) = nullptr;
		void ( *onCancel )( void ) = nullptr;

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
		// 1 (Delay): Delay
		// 2 (char*): Text
		// 3 (size_t): Length/Size of Text
		ThreadedLine( Delay, char*, size_t );

		// Parameters:
		// 1 (Delay): Delay
		// 2 (std::string): Text as string
		ThreadedLine( Delay, std::string );

		~ThreadedLine();
	};
}

#endif