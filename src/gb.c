#include "gb.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


#ifdef DEBUG
#define CHECK_INVARIANTS(buf, strict)                                  \
    do {                                                               \
        GapInvariantResult r = gb_check_invariants(buf, strict);           \
        if (r != GAP_OK) {                                             \
            fprintf(stderr, "invariant failed: %d\n", r);              \
            abort();                                                   \
        }                                                              \
    } while (0)
#else
#define CHECK_INVARIANTS(buf, strict) ((void)0)
#endif

typedef enum {
    GAP_OK = 0,
    GAP_NULL,
    GAP_BOUNDS,
    GAP_TEXT_SIZE,
    GAP_CURSOR_RANGE,
    GAP_CURSOR_NOT_AT_GAP
} GapInvariantResult;


static const size_t GAP_SIZE = 64;

struct GapBuffer {
    char *buffer;
    size_t buf_size;
    size_t text_size;
    size_t gap_start;
    size_t gap_end;
    size_t cursor_pos;
};


static GapInvariantResult gb_check_invariants(const GapBuffer *Buffer, bool require_cursor_at_gap) {
    if (!Buffer) return GAP_NULL;

    size_t gap_size = Buffer->gap_end - Buffer->gap_start;

    // structural bounds
    if (!(Buffer->gap_start <= Buffer->gap_end &&
            Buffer->gap_end <= Buffer->buf_size)) {
        return GAP_BOUNDS;
    }

    // text size consistency
    if (Buffer->text_size != Buffer->buf_size - gap_size) {
        return GAP_TEXT_SIZE;
    }

    // cursor validity
    if (Buffer->cursor_pos > Buffer->text_size) {
        return GAP_CURSOR_RANGE;
    }

    if (require_cursor_at_gap &&
            Buffer->cursor_pos != Buffer->gap_start) {
        return GAP_CURSOR_NOT_AT_GAP;
    }


    return GAP_OK;
}

static void *gb_xmalloc(size_t size) {
    void *ptr = malloc(size);
      if (!ptr) {
            fprintf(stderr, "out of memory\n");
            abort();
      }

      return ptr;
}

size_t gb_length(const GapBuffer *Buffer) {
    if (!Buffer) return 0;
    size_t length = Buffer->text_size;

    return length;
}

size_t gb_cursor(const GapBuffer *Buffer) {
    if (!Buffer) return 0;
    size_t cursor = Buffer->cursor_pos;

    return cursor;
}

size_t gb_copy_text(const GapBuffer *Buffer, char *out, size_t out_size) {
    if (!Buffer || !out || out_size == 0) return 0;
    
    size_t to_copy = Buffer->text_size;
    if (to_copy > out_size) {
        to_copy = out_size;
    }

    size_t before_gap = Buffer->gap_start;
    size_t after_gap = Buffer->text_size - before_gap;

    if (before_gap > to_copy) {
        before_gap = to_copy;
        after_gap = 0;
    } else if (before_gap + after_gap > to_copy) {
        after_gap = to_copy - before_gap;
    }

    if (before_gap + after_gap < out_size) {
        out[before_gap + after_gap] = '\0';
    }

    memcpy(out, Buffer->buffer, before_gap);
    memcpy(out + before_gap, Buffer->buffer + Buffer->gap_end, after_gap);

    return before_gap + after_gap;
}

void gb_dest(GapBuffer *Buffer) {
    if (!Buffer) return;
    free(Buffer->buffer);
    free(Buffer);
}

static GapBuffer *gb_grow_gap(GapBuffer *Buffer, size_t n){
    size_t old_gap = Buffer->gap_end - Buffer->gap_start;
    size_t req_gap = n + GAP_SIZE;
    if(req_gap <= old_gap) return Buffer;
    size_t new_buf_size = Buffer->buf_size + (req_gap - old_gap);

    GapBuffer *NewBuffer = gb_xmalloc(sizeof(GapBuffer));
    NewBuffer->buffer = gb_xmalloc(new_buf_size);
    // allocates only exact amount needed, for future improvements i can make it allocate more than needed to reduce overuse of gb_grow_gap()
    NewBuffer->buf_size = new_buf_size; 
    NewBuffer->gap_start = Buffer->gap_start;
    NewBuffer->gap_end = NewBuffer->gap_start + req_gap;
    NewBuffer->cursor_pos = NewBuffer->gap_start;
    NewBuffer->text_size = Buffer->text_size;

    memcpy(
            NewBuffer->buffer,
            Buffer->buffer,
            NewBuffer->gap_start
    );

    memcpy(
            NewBuffer->buffer + NewBuffer->gap_end,
            Buffer->buffer + Buffer->gap_end,
            Buffer->buf_size - Buffer->gap_end
    );

    gb_dest(Buffer);

    CHECK_INVARIANTS(NewBuffer, 1);

    return NewBuffer;
}

GapBuffer *gb_insert(GapBuffer *Buffer, const char *str) {
    size_t n = strlen(str);
    if(n == 0) return Buffer;

    size_t gap_size = Buffer->gap_end - Buffer->gap_start;

    if(n > gap_size){
        Buffer = gb_grow_gap(Buffer, n);
    }

    memcpy(Buffer->buffer + Buffer->gap_start, str, n);
    Buffer->gap_start += n;
    Buffer->cursor_pos += n;
    Buffer->text_size += n;

    CHECK_INVARIANTS(Buffer, 1);

    return Buffer;
}

static GapBuffer *gb_move_gap(GapBuffer *Buffer) {
    size_t gap_size = Buffer->gap_end - Buffer->gap_start;
    
    size_t physical_pos;
    if (Buffer->cursor_pos <= Buffer->gap_start) {
        physical_pos = Buffer->cursor_pos;
    } else {
        physical_pos = Buffer->cursor_pos + gap_size;
    }

    if (physical_pos < Buffer->gap_start) {
        size_t move_len = Buffer->gap_start - physical_pos;
        memmove(
                Buffer->buffer + physical_pos + gap_size,
                Buffer->buffer + physical_pos,
                move_len
                );
        Buffer->gap_start = physical_pos;
        Buffer->gap_end = physical_pos + gap_size;
    } else if (physical_pos > Buffer->gap_end) {
        size_t move_len = physical_pos - Buffer->gap_end;
        memmove(
                Buffer->buffer + Buffer->gap_start,
                Buffer->buffer + Buffer->gap_end,
                move_len
               );
        Buffer->gap_start += move_len;
        Buffer->gap_end += move_len;
    }

    Buffer->cursor_pos = Buffer->gap_start;

    CHECK_INVARIANTS(Buffer, 1);

    return Buffer;
}

GapBuffer *gb_move_left(GapBuffer *Buffer, size_t to_move) {
    if (to_move >= Buffer->cursor_pos) {
        Buffer->cursor_pos = 0;
    } else {
        Buffer->cursor_pos = Buffer->cursor_pos - to_move;
    }

    gb_move_gap(Buffer);

    return Buffer;
}

GapBuffer *gb_move_right(GapBuffer *Buffer, size_t to_move) {
    if (to_move >= Buffer->text_size - Buffer->cursor_pos) {
        Buffer->cursor_pos = Buffer->text_size;
    } else {
        Buffer->cursor_pos = Buffer->cursor_pos + to_move;
    }

    gb_move_gap(Buffer);

    return Buffer;
}

GapBuffer *gb_delete(GapBuffer *Buffer, size_t n) {
    gb_move_gap(Buffer);

    size_t max_n = Buffer->text_size - Buffer->cursor_pos;
    if (n > max_n) {
        n = max_n;
    }

    Buffer->gap_end += n;
    Buffer->text_size -= n;

    CHECK_INVARIANTS(Buffer, 1);

    return Buffer;
}

GapBuffer *gb_backspace(GapBuffer *Buffer, size_t n) {
    gb_move_gap(Buffer);

    size_t max_n = Buffer->cursor_pos;
    if (n > max_n) {
        n = max_n;
    }

    Buffer->gap_start -= n;
    Buffer->cursor_pos -= n;
    Buffer->text_size -= n;

    CHECK_INVARIANTS(Buffer, 1);

    return Buffer;
}


GapBuffer *gb_init(const char *str) {
    size_t str_size = strlen(str);

    GapBuffer *Buffer = gb_xmalloc(sizeof(GapBuffer));
    Buffer->buf_size = str_size + GAP_SIZE;
    Buffer->text_size = str_size;
    Buffer->buffer = gb_xmalloc(Buffer->buf_size);

    memcpy(Buffer->buffer, str, str_size);

    Buffer->gap_start = str_size;
    Buffer->gap_end = str_size + GAP_SIZE;
    Buffer->cursor_pos = Buffer->gap_start;

    CHECK_INVARIANTS(Buffer, 1);

    return Buffer;
}
