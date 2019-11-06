.data

.balign 4
string: .skip 100

.balign 4
startint: .word 0

.balign 4
endint: .word 0

.balign 4
substring: .skip 100

.text
.global sub_string
sub_string:
	mov r5, r0
	mov r6, r1
	mov r7, r2


	//loop:
	ldr r8, [r5, r6]
	add r6, r6, #1
	mov r0, r8
	/*bl printf
	
	cmp r6, r7
	bne loop
	*/

	bx lr //return to the main function

