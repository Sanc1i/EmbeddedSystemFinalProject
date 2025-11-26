#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>

#define FIFO_SIZE 256

typedef struct {
    char buffer[FIFO_SIZE + 1];
    uint16_t head;
    uint16_t tail;
} FIFO_Typedefine;

void fifo_init (FIFO_Typedefine *Fifo) {
    Fifo->head = 0;
    Fifo->tail = 0;
}

uint8_t is_fifo_empty (FIFO_Typedefine *Fifo) {
    return (Fifo->head == Fifo->tail);
}

void reset_fifo (FIFO_Typedefine *Fifo) {
    Fifo->head = Fifo->tail;
}

uint8_t is_fifo_full (FIFO_Typedefine *Fifo) {
    return ((Fifo->head + 1) % (FIFO_SIZE + 1) == Fifo->tail);
}

int fifo_put (FIFO_Typedefine *Fifo, char Data) {
    if (is_fifo_full(Fifo)) {
        return -1;
    }
    Fifo->buffer[Fifo->head] = Data;
    Fifo->head = (Fifo->head + 1) % (FIFO_SIZE + 1);
    return 0;
}

int fifo_get (FIFO_Typedefine *Fifo, char *Data) {
    if (is_fifo_empty(Fifo)) {
        return -1;
    }
    *Data = Fifo->buffer[Fifo->tail];
    Fifo->tail = (Fifo->tail + 1) % (FIFO_SIZE + 1);
    return 0;
}


#endif /* FIFO_H_ */
