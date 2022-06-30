	add	1,2,3
	sub	2,3,4
	slt	3,4,5
one	or	5,4,6
	nand	6,7,8
	addi	7,8,30
	ori	8,9,40
	slti	10,11,200
	lui	12,2
two	lw	13,14,one
	lw	14,15,100
	sw	1,2,two
	sw	2,3,0
	beq	1,4,one
	jalr	1,5
jump	j	jump
	halt
twlv	.fill	12
jone	.fill	one
	