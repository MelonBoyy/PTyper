#include "ptyper.h"
#include "ptyper-threaded.h"
#include <thread>
#include <chrono>

void SleepMilliseconds( unsigned int milliseconds )
{
	std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
}

int main()
{
	PAL::TYPER::Line _regularLineTest( 20, "This is a regular line test... Very, very nice!\n" );
	_regularLineTest.TypeLine();

	PAL::TYPER::ThreadedLine _threadedLineTest( 20, "This is a threaded line test... Very, very nice!\n" );
	_threadedLineTest.InitiateTyping();
	SleepMilliseconds( 500 );
	printf( "|PAUSE|" );
	fflush( stdout );
	_threadedLineTest.Pause();
	SleepMilliseconds( 2000 );
	printf( " |PLAY| " );
	fflush( stdout );
	_threadedLineTest.Play();
	SleepMilliseconds( 3500 );
	printf( " |CANCEL| " );
	fflush( stdout );
	_threadedLineTest.CancelTyping();
	SleepMilliseconds( 1000 );

	PAL::TYPER::Line _ending( 20, "\nThank you for viewing the demo! Have a nice day.\n" );
	_ending.TypeLine();

	return 0;
}