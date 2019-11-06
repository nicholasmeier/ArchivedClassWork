.data

.balign 4
first: .asciz "Enter the first number: "

.balign 4
inputf: .asciz "%d"

.balign 4
second: .asciz "Enter the second number: "

.balign 4
inputs: .asciz "%d"

.balign 4
outeq1: .asciz "%d "
.balign 4
outeq2: .asciz "and %d are equal.\n"

.balign 4
outnoteq1: .asciz "%d "
.balign 4
outnoteq2: .asciz "is strictly greater than %d"
.balign 4
outnoteq3: .asciz " by %d.\n"

.balign 4
return1: .space 80

.balign 4
return2: .space 80

.balign 4
return3: .space 80

.text
.global main
main:
	//read first number
	push {lr}
	ldr r0, addr_first
	bl printf

	ldr r0, addr_inputf
	sub sp, sp, #4
	mov r1, sp
	bl scanf
	ldr r4, [sp, #0]
	add sp, sp, #4

	//read second number
	ldr r0, addr_second
	bl printf

	ldr r0, addr_inputs
	sub sp, sp, #4
	mov r1, sp
	bl scanf
	ldr r5, [sp, #0]
	add sp, sp, #4

	//compare the values
	mov r0, r4
	mov r1, r5
	cmp r0, r1
	bgt fgts //branch if int1 > int2
	blt sgtf //branch ig int2 > int1

	//does equalto handlinging printing
	ldr r0, addr_outeq1
	mov r1, r4
	bl printf

	ldr r0, addr_outeq2
	mov r1, r5
	bl printf
	b end

	fgts:
	mov r0, r4
	mov r1, r5
	sub r6, r0, r1

	ldr r0, addr_outnoteq1
	mov r1, r4
	bl printf

	ldr r0, addr_outnoteq2
	mov r1, r5
	bl printf

	ldr r0, addr_outnoteq3
	mov r1, r6
	bl printf

	b end

	sgtf:
	mov r0, r4
	mov r1, r5
	sub r6, r1, r0

	ldr r0, addr_outnoteq1
	mov r1, r5
	bl printf

	ldr r0, addr_outnoteq2
	mov r1, r4
	bl printf

	ldr r0, addr_outnoteq3
	mov r1, r6
	bl printf

	end:

	pop {pc}


addr_first: .word first
addr_inputf: .word inputf
addr_second: .word second
addr_inputs: .word inputs
addr_outeq1: .word outeq1
addr_outeq2: .word outeq2
addr_outnoteq1: .word outnoteq1
addr_outnoteq2: .word outnoteq2
addr_outnoteq3: .word outnoteq3
addr_return1: .word return1
addr_return2: .word return2
addr_return3: .word return3




