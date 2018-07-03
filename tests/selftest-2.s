.text
main:
_start:
	addiu	$t0,$0,4        #$t0 = $t0 + 4
	addiu	$t0,$t0,100000	#$t0 = $t0 + 100000
	addiu	$t1,$t1,-1      #$t1 = $t1 - 1
	addiu	$t1,$t1,0x7fff  #$t1 = $t1 + 32767
	
	addu	$t1,$0,$0       #$t1 = $0 + $0
	addu	$t1,$t1,$t0     #$t1 = $t1 + $t0
	addu	$t1,$t1,$t0     #$t1 = $t1 + $t0
	subu	$t1,$t1,$t0     #$t1 = $t1 - $t0
	addiu	$t1,$t1,5       #$t1 = $t1 + 5       
	addu	$t1,$0,$0       #$t1 = $0 + $0
	addiu	$t1,$t1,16      #$t1 = $t1 + 16
	sll	$t0,$t1,2       #$t0 = $t1 << 2 (shifting left by 2 bits)
	srl	$t0,$t1,4       #$t0 = $t1 >> 4 (shifting right by 4 bits)
	addiu	$t0,$0,3        #$t0 = $0 + 3
loop:	slt	$t2,$t1,$t0     #loop: $t2 = $t1 < $t0? (T/F?)
	beq	$t2,$0,go       #$if $t1 == 0 -> go
	addiu	$t0,$t0,-1      #$t0 = $t0 - 1
	j	loop            #loop again
	
go:
	addu	$t1,$0,$0       #$t1 = 0
	addu	$t2,$0,$0       #$t2 = 0      
	jr	$ra             #return address
	
	
