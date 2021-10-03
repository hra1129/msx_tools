; =============================================================================
;	MSX-JE CHECKER
; -----------------------------------------------------------------------------
;	2021/Oct/03rd	t.hara
; =============================================================================

HOKVLD			:=		0xFB20
EXTBIO			:=		0xFFCA
RAMAD0			:=		0xF341
RAMAD1			:=		0xF342
RAMAD2			:=		0xF343
RAMAD3			:=		0xF344

bdos			:=		0x0005

; -----------------------------------------------------------------------------
;	MSX-JE Information
;
DEVICE_MSX_JE			:=		16
MJE_CAPABILITY_VECTOR	:=		0
	MJE_CV_ACTIVE		:=		0x01			; bit0 : 0 = found MSX-JE, 1 = not found
	MJE_CV_VIRTUAL_IF	:=		0x02			; bit1 : 0 = found Virtual terminal I/F, 1 = not found
	MJE_CV_DICT_IF		:=		0x04			; bit2 : 0 = found Dictionary I/F, 1 = not found
	MJE_CV_DICT_REG		:=		0x08			; bit3 : 0 = founc Dictionary registration I/F, 1 = not found

			org		0x100

			; if not exist ExtBIOS goto ext_bios_not_found
			ld		a, [HOKVLD]
			and		a, 1
			jp		z, ext_bios_not_found

			di
			; call ExtBIOS
			ld		d, DEVICE_MSX_JE
			ld		e, 0
			ld		a, [RAMAD0]
			ld		b, a
			ld		hl, return_info
			call	EXTBIO

			; Dump information
			ld		b, 16
			ld		hl, return_info
loop:
			push	bc
			ld		c, [hl]			; C = MJE_CAPABILITY_VECTOR
			inc		hl
			ld		b, [hl]			; B = SLOT#
			inc		hl
			ld		e, [hl]			; E = Low Address
			inc		hl
			ld		d, [hl]			; D = High Address
			inc		hl
			push	hl

			; check end mark
			ld		a, c
			or		a, b
			or		a, e
			or		a, d
			jp		z, pop_exit

			; dump Slot#
			call	dump_slot_number

			; check capability
			call	check_capability_vector

			pop		hl
			pop		bc
			djnz	loop

exit_prog:
			ei
			ld		c, 0x62
			ld		b, 0
			jp		bdos

pop_exit:
			pop		hl
			pop		bc
			jr		exit_prog

; -----------------------------------------------------------------------------
dump_slot_number::
			push	de
			ld		[save_bc], bc
			ld		de, slot_number
			ld		c, 9
			call	bdos

			ld		bc, [save_bc]
			ld		a, b
			and		a, 0x03
			add		a, '0'
			ld		e, a
			ld		c, 2
			call	bdos

			ld		bc, [save_bc]
			ld		a, b
			and		a, 0x80
			jr		z, crlf

			ld		e, '-'
			ld		c, 2
			call	bdos

			ld		bc, [save_bc]
			ld		a, b
			rrca
			rrca
			and		a, 0x03
			add		a, '0'
			ld		e, a
			ld		c, 2
			call	bdos
crlf::
			ld		de, msg_crlf
			ld		c, 9
			call	bdos
			ld		bc, [save_bc]
			pop		de
			ret
save_bc::
			dw		0
slot_number::
			ds		"SLOT#$"
msg_crlf::
			ds		"\r\n$"

; -----------------------------------------------------------------------------
check_capability_vector::
			push	de
			push	bc
			ld		a, c
			ld		de, msg_virtual_terminal_if
			and		a, MJE_CV_VIRTUAL_IF
			call	z, msg_puts

			ld		a, c
			ld		de, msg_dictionary_if
			and		a, MJE_CV_DICT_IF
			call	z, msg_puts

			ld		a, c
			ld		de, msg_registration_if
			and		a, MJE_CV_DICT_REG
			call	z, msg_puts
			pop		bc
			pop		de
			ret
msg_puts::
			push	bc
			ld		c, 9
			call	bdos
			pop		bc
			ret
msg_virtual_terminal_if::
			ds		"  Support the virtual terminal I/F\r\n$"
msg_dictionary_if::
			ds		"  Support the dictionary I/F\r\n$"
msg_registration_if::
			ds		"  Support the registration I/F\r\n$"

; -----------------------------------------------------------------------------
ext_bios_not_found::
			ld		de, msg_ext_bios_not_found
			ld		c, 9
			call	bdos
			ld		c, 0x62
			ld		b, 1
			jp		bdos

; -----------------------------------------------------------------------------
msg_ext_bios_not_found::
			ds		"ExtBIOS not found.\r\n$"

return_info::
			space	64, 0					; 64bytes, 0 fill
