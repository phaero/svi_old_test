#include "buffer.h"

#include <stdlib.h>
#include <string.h>

struct buffer {
	char* body;
	uint64_t size;
	uint64_t body_length;
	uint64_t gap_start;
	uint64_t gap_length;

	/*How much the buffer will grow in size.*/
	uint64_t grow_size;
};

void buffer_init( struct buffer* buf );

void buffer_init( struct buffer* buf )
{
	buf->body = NULL;
	buf->grow_size = 8;
	buf->size = 0;
	buf->body_length = 0;
	buf->gap_start = 0;
	buf->gap_length = 0; // gap_length == size - body_length
}

struct buffer* buffer_new( void ) {
	struct buffer* buf = malloc( sizeof( struct buffer ) );

	buffer_init( buf );

	return buf;
}

void buffer_del( struct buffer** buf )
{
	if( buf == NULL || *buf == NULL ) {
		return;
	}

	free( (*buf)->body );
	free( *buf );

	// Don't leave a pointer to a delete object, makes error checking easier
	*buf = NULL;
}

uint64_t buffer_length( struct buffer* buf )
{
	return buf->body_length;
}

void buffer_gap_to( struct buffer* buf, uint64_t position )
{
	if ( position != buf->gap_start ) {
		if ( position < buf->gap_start ) {
			memmove(
			    buf->body + position + buf->gap_length,
			    buf->body + position,
			    sizeof(char) * (buf->gap_start - position)
			);
		} else {
			memmove(
			    buf->body + buf->gap_start,
			    buf->body + buf->gap_start + buf->gap_length,
			    sizeof(char) * (position - buf->gap_start)
			);
		}
	} else {
		// gap is already at the correct position
	}
}

void buffer_room_for( struct buffer* buf, uint64_t length )
{
	if ( buf->gap_length <= length ) {
		while ( buf->grow_size < ( buf->size / 6 ) ) {
			buf->grow_size *= 2;
		}

		buffer_reallocate( buf, buf->size + length + buf->grow_size );
	}
}

void buffer_reallocate( struct buffer* buf, uint64_t new_size )
{
	if ( new_size > buf->size ) {
		// move gap to the end
		buffer_gap_to( buf, buf->body_length );

		// allocate new body
		char* new_body = malloc( sizeof(char) * new_size );

		if ( (buf->size != 0) && (buf->body != NULL) ) {
			// copy old body into new
			memmove( new_body, buf->body, sizeof(char)*buf->body_length );
			free( buf->body );
		}

		// fix body pointer and calculate gap
		buf->body = new_body;
		buf->gap_length += new_size - buf->size;
		buf->size = new_size;
	}
}

char buffer_value_at( struct buffer* buf, uint64_t position )
{
	if ( position < buf->gap_start ) {
		return buf->body[ position ];
	} else {
		if ( position >= buf->body_length ) {
			return 0;
		} else {
			return buf->body[ buf->gap_length + position ];
		}
	}
}

void buffer_set_value_at( struct buffer* buf, uint64_t position, char value )
{
	if ( position < buf->gap_start ) {
		buf->body[ position ] = value;
	} else {
		if ( position >= buf->body_length ) {
			;
		} else {
			buf->body[ buf->gap_length + position ] = value;
		}
	}
}

void buffer_insert( struct buffer* buf, uint64_t position, char value )
{
	if ( position > buf->body_length ) {
		return;
	}

	buffer_insert_value( buf, position, 1, value );
}

/**
 *  Insert a nuber of elements into the buffer setting their value.
 *  Iserting at positions outside the current range fails.
 **/
void buffer_insert_value( struct buffer* buf, uint64_t position, uint64_t insert_length, char value )
{
	if ( insert_length > 0 ) {
		if ( position > buf->body_length ) {
			return;
		}

		buffer_room_for( buf, insert_length );
		buffer_gap_to( buf, position );

		for ( uint64_t i = 0; i < insert_length; i++ ) {
			buf->body[ buf->gap_start + i ] = value;
		}

		buf->body_length += insert_length;
		buf->gap_start += insert_length;
		buf->gap_length -= insert_length;
	}
}

void buffer_ensure_length( struct buffer* buf, uint64_t wanted_length )
{
	if ( buf->body_length < wanted_length ) {
		buffer_insert_value( buf, buf->body_length, wanted_length - buf->body_length, 0 );
	}
}

void buffer_insert_from_array( struct buffer* buf, uint64_t position_to_insert, const char str[], uint64_t position_from, uint64_t insert_length )
{
	if ( insert_length > 0 ) {
		if ( position_to_insert > buf->body_length ) {
			return;
		}

		buffer_room_for( buf, insert_length );
		buffer_gap_to( buf, position_to_insert );
		memmove( buf->body + buf->gap_start, str + position_from, sizeof(char) * insert_length );

		buf->body_length += insert_length;
		buf->gap_start += insert_length;
		buf->gap_length -= insert_length;
	}
}

void buffer_delete( struct buffer* buf, uint64_t position )
{
	if ( position >= buf->body_length ) {
		return;
	}

	buffer_delete_range( buf, position, 1 );
}

void buffer_delete_range( struct buffer* buf, uint64_t position, uint64_t delete_length )
{
	if ( (position + delete_length) > buf->body_length ) {
		return;
	}
	if ( (position == 0) && (delete_length == buf->body_length) ) {
		// full deallocation returns storage
		free( buf->body );
		buffer_init( buf );
	} else if ( delete_length > 0 ) {
		buffer_gap_to( buf, position );

		buf->body_length -= delete_length;
		buf->gap_length += delete_length;
	}
}

void buffer_delete_all( struct buffer* buf )
{
	buffer_delete_range( buf, 0, buf->body_length );
}

char* buffer_pointer( struct buffer* buf )
{
	buffer_room_for( buf, 1 );
	buffer_gap_to( buf, buf->body_length );
	buf->body[ buf->body_length ] = 0;

	return buf->body;
}
