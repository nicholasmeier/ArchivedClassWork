.data

.balign 4
first: .asciz "Enter your first name: "

.balign 4
inputf: .asciz "%s"

.balign 4
last: .asciz "Enter your last name: "

.balign 4
inputl: .asciz "%s"

.balign 4
output: .asciz "Hello, %s"

.balign 4
outputc: .asciz " %s.\n"

.balign 4
return1: .space 80

.balign 4
return2: .space 80

.text
.global main
main:
	push {lr}
	ldr r0, addr_first //print prompt
	bl printf

	ldr r0, addr_inputf
	ldr r1, addr_return1
	bl scanf

	ldr r0, addr_last
	bl printf

	ldr r0, addr_inputl
	ldr r1, addr_return2
	bl scanf

	ldr r0, addr_output
	ldr r1, addr_return1
	bl printf

	ldr r0, addr_outputc
	ldr r1, addr_return2
	bl printf
	pop {pc}

addr_first: .word first
addr_inputf: .word inputf
addr_inputl: .word inputl
addr_last: .word last
addr_output: .word output
addr_outputc: .word outputc
addr_return1: .word return1
addr_return2: .word return2
