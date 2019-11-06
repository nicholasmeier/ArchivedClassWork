.text
.global main
main:
	push {lr}
	/*scanf()*/
	ldr r0, =input
	sub sp, sp, #4 /*this does something important for scanf idk what*/
	mov r1, sp
	bl scanf /*call scanf as subroutine*/
	ldr r2, [sp, #0]
	add sp, sp, #4 /*maybe it is something to do with the subroutine idk its a stack thing*/

	mov r4, r2 /*pup scanned value in more permanant register*/

	/*scanf() part 2 electric boogaloo*/
	ldr r0, =input
	sub sp, sp, #4
	mov r1, sp
	bl scanf
	ldr r2, [sp, #0]
	add sp, sp, #4
	mov r5, r2 /*put scanned value in more permanant register*/


	cmp r4, r5 /*compare the scanned values*/

	bgt greater_than //jump to greater than label

        /*printf()*/ //print out if the value in r4 is less than r5
	//prints r5
        ldr r0, =format
        mov r1, r5
        bl printf

	b skip_greater_than

	greater_than:
	/*printf()*/
	ldr r0, =format
	mov r1, r4
	bl printf

	skip_greater_than:


	pop {pc}
.data

.balign 4
input: .asciz "%d"

.balign 4
format: .asciz "%d\n"
