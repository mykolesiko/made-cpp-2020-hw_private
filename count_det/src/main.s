/// Your code goes here...

.text
.global count_det

count_det:
	ldr r1, [r0, #16]
	ldr r2, [r0, #32]
	mul  r1, r1, r2
	push {r1}
	ldr r1, [r0, #20]
	ldr r2, [r0, #28]
	mul  r1, r1, r2
	pop {r2}
	sub r1, r2, r1
	ldr r2, [r0, #0]
	mul r3, r1, r2

	ldr r1, [r0, #12]
	ldr r2, [r0, #32]
	mul  r1, r1, r2
	push {r1}
	ldr r1, [r0, #24]
	ldr r2, [r0, #20]
	mul  r1, r1, r2
	pop {r2}
	sub r1, r1, r2
	ldr r2, [r0, #4]
	mul r1, r1, r2
	add r3, r3, r1

	ldr r1, [r0, #12]
	ldr r2, [r0, #28]
	mul  r1, r1, r2
	push {r1}
	ldr r1, [r0, #24]
	ldr r2, [r0, #16]
	mul  r1, r1, r2
	pop {r2}
	sub r1, r2, r1
	ldr r2, [r0, #8]
	mul r2, r1, r2
	add r0, r3, r2
	bx lr

