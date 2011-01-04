#ifndef BUFFER_H_INCLUDES__
#define BUFFER_H_INCLUDES__

#include <stdint.h>

struct buffer;

struct buffer* buffer_new( void );
void buffer_del( struct buffer** buf );
uint64_t buffer_length( struct buffer* buf );
void buffer_gap_to( struct buffer* buf, uint64_t position );
void buffer_room_for( struct buffer* buf, uint64_t length );
void buffer_reallocate( struct buffer* buf, uint64_t new_size );
char buffer_value_at( struct buffer* buf, uint64_t position );
void buffer_set_value_at( struct buffer* buf, uint64_t position, char value );
void buffer_insert( struct buffer* buf, uint64_t position, char value );
void buffer_insert_value( struct buffer* buf, uint64_t position, uint64_t insert_length, char value );
void buffer_ensure_length( struct buffer* buf, uint64_t wanted_length );
void buffer_insert_from_array( struct buffer* buf, uint64_t position_to_insert, const char str[], uint64_t position_from, uint64_t insert_length );
void buffer_delete( struct buffer* buf, uint64_t position );
void buffer_delete_range( struct buffer* buf, uint64_t position, uint64_t delete_length );
void buffer_delete_all( struct buffer* buf );
char* buffer_pointer( struct buffer* buf );

#endif

