.text
.global main
main:
	push {lr}

	//store zero in r4 and r5
	mov r4, #0
	mov r5, #0

	//create a loop
	loop:
		ldr r0, =input
		sub sp, sp, #4 //stuff with the stack, something that makes scanf work
		mov r1, sp
		bl scanf
		ldr r2, [sp, #0]
		add sp, sp, #4

		add r4, r4, r2 /*value at r4 = input value + value at r4*/

		add r5, r5, #1 /*value at r5 = value+1*/

		//check if value in r5 >= 5, if it is, then branch to end label
		cmp r5, #5
		blt loop
		b end
	//print out the added up array
	end:
	ldr r0, =format
	mov r1, r4
	bl printf

	pop {pc}

.data

.balign 4
input: .asciz "%d"

.balign 4
format: .asciz "%d\n" 
