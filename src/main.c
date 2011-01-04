#include "buffer.h"

#include <stdio.h>

int main( int argc, char* argv[] )
{
	struct buffer* buf = buffer_new();

	buffer_insert_from_array( buf, 0, "Hello World!", 0, 12 );
	printf( "%s\n", buffer_pointer( buf ) );

	buffer_insert( buf, buffer_length( buf ), '\n' );

	buffer_insert_from_array( buf, buffer_length( buf ), "Hello World!", 0, 12 );
	printf( "%s\n", buffer_pointer( buf ) );

	buffer_del( &buf );

	return 0;
}
