; =============================================================================
;	test of SoundCartridge (for SD Snatcher)
; =============================================================================

		include		"msxbios.asm"
		include		"msxdos1.asm"

scc_mode_register	:=	0xBFFE
bank0_register		:=	0x5000
bank1_register		:=	0x7000
bank2_register		:=	0x9000
bank3_register		:=	0xB000

		org		0x100
entry::
		; Initialization
		ld			sp, [TPA_BOTTOM]

		; display entry message
		ld			de, msg_entry
		call		puts

		; Page1 changes to slot#1
		ld			a, 0x01
		ld			h, 0x40
		call		enaslt

		; Page2 changes to slot#1
		ld			a, 0x01
		ld			h, 0x80
		call		enaslt

		; Initialize bank registers
		ld			a, 8
		ld			[bank0_register], a
		ld			a, 9
		ld			[bank1_register], a
		ld			a, 10
		ld			[bank2_register], a
		ld			a, 11
		ld			[bank3_register], a

		ld			a, 0x3F
		ld			[scc_mode_register], a

		; 4000h の値を読み出して反転して書き込む
		ld			hl, 0x4000
		ld			a, [hl]
		cpl
		ld			[hl], a
		push		af
		push		hl

		; 4000h の内容と、書き込んだ値が一致するか確認する
		cp			a, [hl]

		ld			de, msg_success
		jp			z, skip1

		ld			de, msg_error
skip1:
		call		puts

		; 1.5秒くらい待つ
		xor			a, a
		ld			[0xFCA2], a
		ei
wait_loop:
		ld			a, [0xFCA2]
		cp			a, 90
		jp			c, wait_loop

		; 4000h の内容と、書き込んだ値が一致するか確認する
		pop			hl
		pop			af
		cp			a, [hl]

		ld			de, msg_success
		jp			z, skip2

		ld			de, msg_error
skip2:
		call		puts

		; Page1 changes to slot#1
		ld			a, [ramad1]
		ld			h, 0x40
		call		enaslt

		; Page2 changes to slot#1
		ld			a, [ramad2]
		ld			h, 0x80
		call		enaslt

		ld			b, 0
		ld			c, D1F_TERM0
		jp			bdos

; =============================================================================
;	Subroutines
; =============================================================================
		include		"stdio.asm"

; =============================================================================
;	Data area
; =============================================================================
msg_crlf::
		db			0x0D, 0x0A, 0
msg_entry::
		ds			"SoundCartridge TEST Programmed by HRA! 2021\r\n"
		ds			"Slot#1 only\r\n"
		db			0
msg_success::
		ds			"Success.\r\n"
		db			0
msg_error::
		ds			"Error.\r\n"
		db			0
