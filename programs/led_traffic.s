movz w7, #0x080
movz w8, #0x100
movz w9, #0x200

movk w1, #0x3f20, lsl #16
movk w1, #0x001c

movk w0, #0x3f20, lsl #16
movk w0, #0x0028

movk w4, #0x3f20, lsl #16
movz w5, #0x0920, lsl #16
str w5, [w4]

b ready

before_steady:
    movz w12, #50, lsl #16
    b loop_steady

steady:
    str w8, [w1]
    b before_go

before_go:
    movz w12, #50, lsl #16
    b loop_go

go:
    str w7, [w0]
    str w8, [w0]
    str w9, [w1]
    b before_stop

before_stop:
    movz w12, #50, lsl #16
    b loop_stop

stop:
    str w9, [w0]
    str w8, [w1]
    b before_ready

before_ready:
    movz w12, #50, lsl #16
    b loop_ready

ready:
    str w8, [w0]
    str w7, [w1]
    b before_steady

loop_steady:
    subs w12, w12, #1
    b.ne loop_steady
    b steady

loop_go:
    subs w12, w12, #1
    b.ne loop_go
    b go

loop_stop:
    subs w12, w12, #1
    b.ne loop_stop
    b stop

loop_ready:
    subs w12, w12, #1
    b.ne loop_ready
    b ready
