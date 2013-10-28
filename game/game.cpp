#include "util.h"

int main( int argc, char *argv[] ) {
	for( int i = 0; i < argc; ++ i ) {
		logf( 3, "Arg %i: [%s]\n", i, argv[i] );
	}
}
