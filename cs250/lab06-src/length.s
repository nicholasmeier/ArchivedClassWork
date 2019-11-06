.data
.balign 4
format: .asciz "%s"

.balign 4
inputstore: .word 128

.balign 4
output: .asciz "%d\n"

.balign 4
length: .space 128

.text
print:
	ldr r0, =output
	ldr r1, =length
	ldr r1, [r1]
	bl printf
	pop {pc}
count:
	add r0, r0, #1
	str r0, [r1]
	ldr r3, [r2, r0]
	cmp r3, #0
	beq print
	b count

.global main
main:
	push {lr}
	ldr r0, =format
	ldr r1, =inputstore
	bl scanf

	mov r0, #-1
	ldr r1, =length
	ldr r2, =inputstore
	bl count

addr_inputstore: .word inputstore
addr_format: .word format
addr_output: .word output
addr_len: .word length


