#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H


#include <stddef.h>


typedef struct GapBuffer GapBuffer;


GapBuffer *gb_init(const char *init_text);
void       gb_dest(GapBuffer *gb); 

GapBuffer *gb_move_left(GapBuffer *gb, size_t n);
GapBuffer *gb_move_right(GapBuffer *gb, size_t n);

GapBuffer *gb_insert(GapBuffer *gb, const char *text);
GapBuffer *gb_delete(GapBuffer *gb, size_t n);
GapBuffer *gb_backspace(GapBuffer *gb, size_t n);

size_t gb_length(const GapBuffer *gb);
size_t gb_cursor(const GapBuffer *gb);

size_t gb_copy_text(const GapBuffer *gb, char *out, size_t out_size);


#endif
