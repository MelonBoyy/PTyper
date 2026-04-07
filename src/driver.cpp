#include "ptyper.h"
#include <thread>
#include <chrono>

int main()
{
	PAL::TYPER::ThreadedLine _newLine( 20, "ASS... IT IS ASS!" );
	_newLine.onBegin = [] { printf( "BEGUN!" ); };

	std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
	printf( "DONE" );

	return 0;
}