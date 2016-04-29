stm8/

	#include "mapping.inc"
	#include "regs.inc"

; Calling convention: given function func(arg1,arg2,arg3): do this:
; push arg1
; push arg2
; push arg3
; call func
; pop a
; pop a
; pop a

	segment 'rom'
main.l
	; initialize clock
	mov CLK_CKDIVR, #$00
	mov CLK_PCKENR1, #$ff
	
	;initialize gpio
	bset PB_DDR, #5
	bset PB_CR1, #5
	
	;initialize timer
	bset TIM2_CR1, #7
	mov TIM2_PSCR, #$0f
	mov TIM2_ARRH, #$ff
	mov TIM2_ARRL, #$ff
	bset TIM2_IER, #0
	bset TIM2_CR1, #0
	
	;initialize adc
	bset ADC_CR1,#0
	
	;initialize uart
	call init_uart
	call init_uart_tx
	
	ldw x,#message
	pushw x
	call print
	
	push #3
	call readadc
	pop a
	ld 1, a
	rim
loop
	wfi
	jra loop

; readadc (8-bit int for channel)
; clobbers a
; returns 8-bit ADC result in the input location on stack
readadc
	ld a, ADC_CSR
	and a, #$70
	or a,(3,sp)
	ld ADC_CSR,a
	bset ADC_CR1,#0
readadc_loop
	btjf ADC_CSR,#7,readadc_loop
	ld a,ADC_DRH
	ld (3,sp),a
	ret

	interrupt timer_handler
timer_handler.l
	inc 0
	jrne not_blink
	dec 1
	jrne not_blink
	ldw x,#blink_message
	pushw x
	call print
	popw x
	push #3
	call readadc
	call print_uint8
	pop a
	ld 1, a
	bcpl PB_ODR, #5
not_blink
	iret

; init_uart (no args)
; no clobber
; no return
init_uart
	bres UART1_CR3,#4
	bres UART1_CR3,#5
	mov UART1_BRR2,#3
	mov UART1_BRR1,#$68
	ret

; init_uart_tx (no args)
; no clobber
; no return
init_uart_tx
	bset UART1_CR2,#3
	ret

; print_uint8 (8-bit int to print)
; clobbers x,a
; no return
print_uint8
	ld a,#0
	ld xh,a
	ld a,(3,sp)
	ld xl,a
	ld a,#10
	div x,a
	add a,#'0'
	push a
	ld a,#10
	div x,a
	add a,#'0'
	push a
	ld a,#10
	div x,a
	add a,#'0'
	push a
	call putchar
	pop a
	call putchar
	pop a
	call putchar
	pop a
	ret

; putchar (char to put)
; clobbers a
; no return
putchar
	btjf UART1_SR,#7,putchar
	ld a,(3,sp)
	ld UART1_DR,a
	ret

; print (16-bit ptr to first char of null-terminated string)
; clobbers x,a
; no return
print
	ldw x,(3,sp)
continue_print
	ld a,(x)
	jreq done_print
	push a
	call putchar
	pop a
	incw x
	jra continue_print
done_print
	ret

ram0_start.b EQU $ram0_segment_start
ram0_end.b EQU $ram0_segment_end
ram1_start.w EQU $ram1_segment_start
ram1_end.w EQU $ram1_segment_end
stack_start.w EQU $stack_segment_start
stack_end.w EQU $stack_segment_end

	interrupt NonHandledInterrupt
NonHandledInterrupt.l
	iret

message
	string "This is a message on startup",$0d,$0a,0

blink_message
	string "This is printed every blink",$0d,$0a,0

	segment 'vectit'
	dc.l {$82000000+main}									; reset
	dc.l {$82000000+NonHandledInterrupt}	; trap
	dc.l {$82000000+NonHandledInterrupt}	; irq0
	dc.l {$82000000+NonHandledInterrupt}	; irq1
	dc.l {$82000000+NonHandledInterrupt}	; irq2
	dc.l {$82000000+NonHandledInterrupt}	; irq3
	dc.l {$82000000+NonHandledInterrupt}	; irq4
	dc.l {$82000000+NonHandledInterrupt}	; irq5
	dc.l {$82000000+NonHandledInterrupt}	; irq6
	dc.l {$82000000+NonHandledInterrupt}	; irq7
	dc.l {$82000000+NonHandledInterrupt}	; irq8
	dc.l {$82000000+NonHandledInterrupt}	; irq9
	dc.l {$82000000+NonHandledInterrupt}	; irq10
	dc.l {$82000000+NonHandledInterrupt}	; irq11
	dc.l {$82000000+NonHandledInterrupt}	; irq12
	dc.l {$82000000+timer_handler}	; irq13
	dc.l {$82000000+NonHandledInterrupt}	; irq14
	dc.l {$82000000+NonHandledInterrupt}	; irq15
	dc.l {$82000000+NonHandledInterrupt}	; irq16
	dc.l {$82000000+NonHandledInterrupt}	; irq17
	dc.l {$82000000+NonHandledInterrupt}	; irq18
	dc.l {$82000000+NonHandledInterrupt}	; irq19
	dc.l {$82000000+NonHandledInterrupt}	; irq20
	dc.l {$82000000+NonHandledInterrupt}	; irq21
	dc.l {$82000000+NonHandledInterrupt}	; irq22
	dc.l {$82000000+NonHandledInterrupt}	; irq23
	dc.l {$82000000+NonHandledInterrupt}	; irq24
	dc.l {$82000000+NonHandledInterrupt}	; irq25
	dc.l {$82000000+NonHandledInterrupt}	; irq26
	dc.l {$82000000+NonHandledInterrupt}	; irq27
	dc.l {$82000000+NonHandledInterrupt}	; irq28
	dc.l {$82000000+NonHandledInterrupt}	; irq29

	end
