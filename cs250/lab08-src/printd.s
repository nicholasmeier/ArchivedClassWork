.data

.balign 4
inputint: .word 16

.text
.global printd
printd:
	str r0, inputint
