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
	; turboR �Ȃ� Z80 �ɂ���
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
	; V-Sync ������̂�҂�
	ld		c, vdp_port + 1
	ld		a, 2
	out		[c], a
	ld		a, 0x8F
	out		[c], a

wait_v_sync_loop:
	in		a, [c]
	and		a, 0x40
	jr		z, wait_v_sync_loop

	; R#23 �ɃZ�b�g����l�����߂�
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

wait_not_v_sync_loop:
	in		a, [c]
	and		a, 0x40
	jr		nz, wait_not_v_sync_loop

	; H-Sync ������̂�҂�
wait_h_sync_loop:
	in		a, [c]
	and		a, 0x20
	jr		z, wait_h_sync_loop

	; R#23 (����) ���Z�b�g����
	out		[c], d					; 14cyc
	out		[c], l					; 14cyc

	; VDP clock = 684 cyc --> CPU clock = 114cyc ���炢�ҋ@����
	ld		h, 2					; 8cyc	�����̃T�C�N�����������Ȃ��Bwait����������ĂȂ����ۂ��B
left_wait:
	nop								; 5cyc	�����̃T�C�N�����������Ȃ��Bwait����������ĂȂ����ۂ��B
	dec		h						; 5cyc	�����̃T�C�N�����������Ȃ��Bwait����������ĂȂ����ۂ��B
	jp		nz, left_wait			; 11cyc	�����̃T�C�N�����������Ȃ��Bwait����������ĂȂ����ۂ��B

	; R#23 (�E��) ���Z�b�g����
	out		[c], e					; 14cyc
	out		[c], l					; 14cyc

	djnz	wait_h_sync_loop
	jp		wait_v_sync_loop
	endscope

	scope	restore_cpu
	; turboR �Ȃ� ���� CPU �ɖ߂�
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
