movz w1, #0x200

movk w2, #0x3f20, lsl #16
movk w2, #0x001c

movk w3, #0x3f20, lsl #16
movk w3, #0x0028

movk w4, #0x3f20, lsl #16
movz w5, #0x0800, lsl #16
str w5, [w4]

before_set:
    movz w0, #50, lsl #16
    b loop_set

set:
    str w1, [w2]
    b before_clear

before_clear:
    movz w0, #50, lsl #16
    b loop_clear

clear:
    str w1, [w3]
    b before_set

loop_set:
    subs w0, w0, #1
    b.ne loop_set
    b set

loop_clear:
    subs w0, w0, #1
    b.ne loop_clear
    b clear
