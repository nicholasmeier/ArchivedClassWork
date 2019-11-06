.data
.balign 4
prompt1: .asciz "Enter in string: "

.balign 4
prompt2: .asciz "Enter in start index (inclusive) : "

.balign 4
prompt3: .asciz "Enter in end index (exclusive) : "

.balign 4
input1: .asciz "%s"

.balign 4
input2: .asciz "%d"

.balign 4
output: .asciz "The substring is %s\n"

.balign 4
return: .skip 100

.balign 4
int1: .word 0
.balign 4
int2: .word 0

.text
.global main
main:
	push {lr}

	//print prompt for string
	ldr r0, =prompt1
	bl printf

	//store user input in var return
	ldr r0, =input1
	ldr r1, =return
	bl scanf

	//print prompt for start index
	ldr r0, =prompt2
	bl printf

	//store user input in var int1
	ldr r0, =input2
	ldr r1, =int1
	bl scanf

	//print prompt for end index
	ldr r0, =prompt3
	bl printf

	//store user input in var int2
	ldr r0, =input2
	ldr r1, =int2
	bl scanf

	//stick start/end ints in r5 and r6
	ldr r5, =int1
	ldr r6, =int2

	//fill in arguments for c function
	ldr r0, =return
	ldr r1, [r5]
	ldr r2, [r6]
	//branch to c function
	bl sub_string

	//mov output of c function to r1
	mov r1, r0

	//print out the substring
	ldr r0, =output
	bl printf

	pop {pc}

