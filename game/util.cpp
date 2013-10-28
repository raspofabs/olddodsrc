#include "util.h"

#include <signal.h>

#ifndef __CYGWIN__
#include <execinfo.h>

#include <fcntl.h>

bool verbose = true;
int loggingLevel = 3;
bool gQuit = false;

void PrintTrace() {
	const int tracedepth = 64;
	void *array[tracedepth];
	size_t size;
	size = backtrace(array, tracedepth);
	backtrace_symbols_fd(array, size, 2);
}

void BailTrace( int sig ) {
	const int tracedepth = 64;
	void *array[tracedepth];
	size_t size;

	// get void*'s for all entries on the stack
	size = backtrace(array, tracedepth);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, 2);
	int mode =
		S_IRUSR|S_IWUSR|
		S_IRGRP|S_IWGRP|
		S_IROTH|S_IWOTH;
		
	int fdelog = open( "error.log", O_CREAT|O_WRONLY|O_TRUNC, mode );
	fprintf(stderr, "Opened %i for writing error log\n", fdelog );
	backtrace_symbols_fd(array, size, fdelog);
	exit(1);
}
#else
void BailTrace( int sig ) {
	fprintf(stderr, "Error: signal %d:\n", sig);
	exit(1);
}
#endif

void BasicCtrlCHandler( int ) {
	if( gQuit ) {
		printf( "Force quit detected, attempt forced shutdown.\n" );
		exit(1);
	}
	printf( "Quit request detected, attempt graceful shutdown.\n" );
	gQuit = true;	
}
void NullHandler( int ) {
	eprintf( "SIGPIPE : from goodness knows where\n" );
}

void SetupCrashdump() {
	static struct sigaction action;
	memset(&action, 0, sizeof( action ) );
	action.sa_handler = BailTrace;
	sigaction(SIGSEGV, &action, nullptr );
	action.sa_handler = BasicCtrlCHandler;
	sigaction(SIGINT, &action, nullptr );
	sigaction(SIGTERM, &action, nullptr );
	//action.sa_handler = NullHandler;
	//sigaction(SIGPIPE, &action, nullptr );
	signal(SIGPIPE, SIG_IGN);
}

#include <sys/types.h>
#include <sys/stat.h>

time_t get_mtime(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) == -1) {
		return -1;
	}
	return statbuf.st_mtime;
}

time_t get_ctime(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) == -1) {
		return -1;
	}
	return statbuf.st_ctime;
}

uint32_t GoodRandom() {
	uint32_t goodrand = time(0);
	if( FILE *fp = fopen( "/dev/urandom", "rb" ) ) {
		int bytes = fread( &goodrand, sizeof( goodrand ), 1, fp );
		if( bytes != 1 ) {
			eprintf( "Can't read enough dev/rand\n" );
		}
		fclose(fp);
	} else {
		eprintf( "can't get good seed\n" );
	}
	return goodrand;
}

#include <unistd.h>
void stripcolours( char *str ) {
	const char *in = str;
	char *out = str;
	while( *in ) {
		if( in[0] == '\033' && in[1] == '[' ) {
			//in colour section
			in += 1;
			while( in[0] && in[0] != 'm' ) {
				++in;
			}
			if( in[0] )
				++in;
		}
		*out = *in;
		++in;
		++out;
	}
	*out = *in;
}

int is_redirected(){
   if (!isatty(fileno(stdout))){
       return 1;
   }
   return 0;
}

