; =============================================================================
;	CHGCPU
;	CPUを切り替えるツールです。
; =============================================================================

		include		"msxbios.asm"
		include		"msxdos1.asm"

		org		0x100

entry::
		; 初期化
		ld			sp, [TPA_BOTTOM]

		; 起動メッセージ
		ld			de, msg_entry
		call		puts

		; パラメータを取得
		ld			hl, 0x0081
get_parameter:
		ld			a, [hl]
		inc			hl
		or			a, a
		jp			z, usage

		cp			a, '0'
		jp			c, get_parameter
		cp			a, '3' + 1
		jp			nc, get_parameter

		sub			a, '0'
		call		my_chgcpu

exit_program::
		ld			b, 0
		ld			c, D1F_TERM0
		jp			bdos

usage::
		ld			de, msg_usage
		call		puts
		jp			exit_program

; =============================================================================
;	CPUを切り替える
; =============================================================================
my_chgcpu::
		ld			[target_cpu], a

		; BIOSの 002Dh番地を読む
		ld			a, [exptbl]
		ld			hl, 0x002D
		call		rdslt
		ld			[version_id], a

		; turboRか?
		cp			a, 3
		jp			c, my_chgcpu_for_panasonic2plus

		ld			a, [target_cpu]
		cp			a, 3
		jp			nc, this_is_not_panasonic2plus

		or			a, 0x80
		ld			iy, [exptbl]
		ld			ix, chgcpu
		call		calslt

		ld			a, [target_cpu]
		add			a, a
		ld			l, a
		ld			h, 0
		ld			de, msg_changed_table
		add			hl, de
		ld			e, [hl]
		inc			hl
		ld			d, [hl]
		call		puts

		jp			exit_program

my_chgcpu_for_panasonic2plus::
		; パナソニックレジスタの存在チェック
		ld			a, 8
		out			[0x40], a
		in			a, [0x40]
		cp			a, ~8
		jp			nz, this_is_not_turbo_r

		ld			a, [target_cpu]
		or			a, a
		jp			z, change_z80_mode_for_pana2plus
		cp			a, 3
		jp			z, change_z80_5mhz_mode_for_pana2plus
		jp			this_is_not_turbo_r

change_z80_mode_for_pana2plus::
		ld			a, 1
		out			[0x41], a

		ld			de, msg_z80
		call		puts
		jp			exit_program

change_z80_5mhz_mode_for_pana2plus::
		xor			a, a
		out			[0x41], a

		ld			de, msg_z80_5mhz
		call		puts
		jp			exit_program

this_is_not_turbo_r::
		ld			de, msg_this_is_not_turbo_r
		call		puts
		jp			exit_program

this_is_not_panasonic2plus::
		ld			de, msg_this_is_not_panasonic2plus
		call		puts
		jp			exit_program

; =============================================================================
;	Subroutines
; =============================================================================
		include		"stdio.asm"

; =============================================================================
;	Data area
; =============================================================================
target_cpu::
		db			0
version_id::
		db			0
msg_crlf::
		db			0x0D, 0x0A, 0
msg_entry::
		ds			"CHGCPU v1.0 Programmed by HRA! 2021\r\n"
		db			0
msg_usage::
		ds			"Usage:\r\n"
		ds			"  >CHGCPU <parameter>\r\n"
		ds			"\r\n"
		ds			"  <parameter>\r\n"
		ds			"    0 ... Z80 3.579545MHz mode\r\n"
		ds			"    1 ... R800 ROM mode (turboR Only)\r\n"
		ds			"    2 ... R800 DRAM mode (turboR Only)\r\n"
		ds			"    3 ... Z80 5.3693175MHz mode (Panasonic MSX2+ Only)\r\n"
		db			0
msg_this_is_not_turbo_r::
		ds			"This is not MSXturboR.\r\n"
		db			0
msg_this_is_not_panasonic2plus::
		ds			"This is not FS-A1FX/WX/WSX.\r\n"
		db			0
msg_changed_table::
		dw			msg_z80, msg_r800_rom, msg_r800_dram, msg_z80_5mhz
msg_z80::
		ds			"Changed to Z80 3.579545MHz mode\r\n"
		db			0
msg_r800_rom::
		ds			"Changed to R800 ROM mode\r\n"
		db			0
msg_r800_dram::
		ds			"Changed to R800 DRAM mode\r\n"
		db			0
msg_z80_5mhz::
		ds			"Changed to 5.3693175MHz mode\r\n"
		db			0
