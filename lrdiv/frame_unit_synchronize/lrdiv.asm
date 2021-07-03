; =============================================================================
;	left-right divider test
; -----------------------------------------------------------------------------
;	2021/July/2nd	t.hara
; =============================================================================

	db		0xfe
	dw		start_adr,end_adr,start_adr

msxver		=	0x002d
chgcpu		=	0x0180
getcpu		=	0x0183
vdp_port	=	0x98

	org		0xB000
start_adr:

	scope	change_to_z80
	; turboR なら Z80 にする
	ld		a, [msxver]
	cp		a, 3
	jp		c, not_turbo_r

	call	getcpu
	ld		[backup_cpu], a

	ld		a, 0x80
	call	chgcpu
not_turbo_r:
	endscope

	scope	main_loop
	di
main_loop:
	; V-Sync が来るのを待つ
	ld		c, vdp_port + 1
	ld		a, 2
	out		[c], a
	ld		a, 0x8F
	out		[c], a

wait_v_sync_loop:
	in		a, [vdp_port + 1]
	and		a, 0x40
	jr		z, wait_v_sync_loop

	; R#23 にセットする値を決める
	ld		a, [r23_left]
	inc		a
	ld		[r23_left], a
	ld		d, a

	ld		a, [r23_right]
	dec		a
	ld		[r23_right], a
	ld		e, a

	ld		b, 212
	ld		l, 0x80 + 23

	ld		h, 0x60
wait_not_hv_sync_loop:
	in		a, [vdp_port + 1]
	and		a, h
	jr		nz, wait_not_hv_sync_loop

	ld		h, 0x20
wait_h_sync_loop:
	in		a, [vdp_port + 1]
	and		a, h
	jr		z, wait_h_sync_loop

line_process:
	; [LEFT] VDP clock = 672 cyc --> CPU clock = 112cyc で処理する
	; R#23 (左側) をセットする
	out		[c], d					; 14cyc
	out		[c], l					; 14cyc

	add		a, 0					; 8cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	jp		skip1					; 11cyc
skip1:
	; [RIGHT] VDP clock = 672 cyc --> CPU clock = 112cyc で処理する
	; R#23 (右側) をセットする
	out		[c], e					; 14cyc
	out		[c], l					; 14cyc

	add		a, 0					; 8cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	nop								; 5cyc
	dec		b						; 5cyc
	jp		nz, line_process		; 11cyc

	; タイミング調整
	nop								; 5cyc
	add		a, 0					; 8cyc

	jp		wait_v_sync_loop
	endscope

	scope	restore_cpu
	; turboR なら 元の CPU に戻す
	ld		a, [msxver]
	cp		a, 3
	jp		c, not_turbo_r

	ld		a, [backup_cpu]
	or		a, 0x80
	call	chgcpu
not_turbo_r:
	endscope

	ret
; -----------------------------------------------------------------------------
backup_cpu:
	db		0
r23_left:
	db		0
r23_right:
	db		0
end_adr:
