/**
 * @author caofuxiang
 *         2016-01-28 11:30:30.
 */

#ifndef EVO_IR_REF_H
#define EVO_IR_REF_H

typedef enum {
    LOCAL,
    CAPTURED,
    EXTERNAL,
} ir_ref_type_t;

typedef struct {
    ir_ref_type_t type;
    int index;
} ir_ref_t;

static inline void ir_ref_init(ir_ref_t * ref, ir_ref_type_t type, int index) {
    ref->type = type;
    ref->index = index;
}

#endif //EVO_IR_REF_H
