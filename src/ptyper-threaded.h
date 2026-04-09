#ifndef PTYPER_THREADED_H
#define PTYPER_THREADED_H

#include "ptyper.h"

namespace PAL::TYPER
{
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