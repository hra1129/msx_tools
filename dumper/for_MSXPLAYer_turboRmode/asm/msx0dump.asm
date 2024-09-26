; =============================================================================
;	MSX0 BIOS Dumper
; -----------------------------------------------------------------------------
;	Programmed by t.hara
; =============================================================================

			org			0x100

bdos		:= 0x0005
rdslt		:= 0x000C
enaslt		:= 0x0024

; BDOS function call
_TERM0		:= 0x00
_CONIN		:= 0x01
_CONOUT		:= 0x02
_AUXIN		:= 0x03
_AUXOUT		:= 0x04
_LSTOUT		:= 0x05
_DIRIO		:= 0x06
_DIRIN		:= 0x07
_INNOE		:= 0x08
_STROUT		:= 0x09
_BUFIN		:= 0x0A
_CONST		:= 0x0B
_CPMVER		:= 0x0C
_DSKRST		:= 0x0D
_SELDSK		:= 0x0E
_FOPEN		:= 0x0F
_FCLOSE		:= 0x10
_SFIRST		:= 0x11
_SNEXT		:= 0x12
_FDEL		:= 0x13
_RDSEQ		:= 0x14
_WRSEQ		:= 0x15
_FMAKE		:= 0x16
_FREN		:= 0x17
_LOGIN		:= 0x18
_CURDRV		:= 0x19
_SETDTA		:= 0x1A
_ALLOC		:= 0x1B
_RDRND		:= 0x21
_WRRND		:= 0x22
_FSIZE		:= 0x23
_SETRND		:= 0x24
_WRBLK		:= 0x26
_RDBLK		:= 0x27
_WRSER		:= 0x28
_GDATE		:= 0x2A
_SDATE		:= 0x2B
_GTIME		:= 0x2C
_STIME		:= 0x2D
_VERIFY		:= 0x2E
_RDABS		:= 0x2F
_WRABS		:= 0x30
_DPARM		:= 0x31
_FFIRST		:= 0x40
_FNEXT		:= 0x41
_FNEW		:= 0x42

; =============================================================================
;	Main Program
			scope	main
main::
			ld		de, s_title
			ld		c, _STROUT
			call	bdos

			; MAIN-ROM
			ld		de, s_main_rom
			ld		c, _STROUT
			call	bdos
			ld		a, 0x80				; SLOT#0-0
			ld		hl, 0x0000
			ld		de, buffer
			ld		bc, 0x8000
			call	rom_dump

			ld		hl, s_main_rom_file_name
			ld		de, fcb
			call	sub_fcreate
			or		a, a
			call	nz, put_error
			ld		hl, fcb
			ld		de, buffer
			ld		bc, 0x8000
			call	sub_fwrite
			ld		hl, fcb
			call	sub_fclose
			call	put_crlf

			ld		de, s_sub_rom
			ld		c, _STROUT
			call	bdos
			ld		a, 0x8B				; SLOT#3-2
			ld		hl, 0x0000
			ld		de, buffer
			ld		bc, 0x4000
			call	rom_dump

			; SUB-ROM
			ld		hl, s_sub_rom_file_name
			ld		de, fcb
			call	sub_fcreate
			or		a, a
			call	nz, put_error
			ld		hl, fcb
			ld		de, buffer
			ld		bc, 0x4000
			call	sub_fwrite
			ld		hl, fcb
			call	sub_fclose
			call	put_crlf

			ld		de, s_iotbasic
			ld		c, _STROUT
			call	bdos
			ld		a, 0x84				; SLOT#0-1
			ld		hl, 0x4000
			ld		de, buffer
			ld		bc, 0x4000
			call	rom_dump

			; IoT-BASIC
			ld		hl, s_iotbasic_rom_file_name
			ld		de, fcb
			call	sub_fcreate
			or		a, a
			call	nz, put_error
			ld		hl, fcb
			ld		de, buffer
			ld		bc, 0x4000
			call	sub_fwrite
			ld		hl, fcb
			call	sub_fclose
			call	put_crlf

			ld		de, s_xbasic
			ld		c, _STROUT
			call	bdos
			ld		a, 0x88				; SLOT#0-2
			ld		hl, 0x4000
			ld		de, buffer
			ld		bc, 0x4000
			call	rom_dump

			; MSXべーしっ君たーぼ
			ld		hl, s_xbasic_rom_file_name
			ld		de, fcb
			call	sub_fcreate
			or		a, a
			call	nz, put_error
			ld		hl, fcb
			ld		de, buffer
			ld		bc, 0x4000
			call	sub_fwrite
			ld		hl, fcb
			call	sub_fclose
			call	put_crlf

			; 漢字BASIC
			ld		de, s_kanjibasic
			ld		c, _STROUT
			call	bdos
			ld		a, 0x8B				; SLOT#3-2
			ld		hl, 0x4000
			ld		de, buffer
			ld		bc, 0x8000
			call	rom_dump

			ld		hl, s_kanjidriver_rom_file_name
			ld		de, fcb
			call	sub_fcreate
			or		a, a
			call	nz, put_error
			ld		hl, fcb
			ld		de, buffer
			ld		bc, 0x8000
			call	sub_fwrite
			ld		hl, fcb
			call	sub_fclose
			call	put_crlf

			ld		de, s_kanjifoont
			ld		c, _STROUT
			call	bdos

			; 漢字ROM JIS1/JIS2
			ld		hl, s_kanjifont_rom_file_name
			ld		de, fcb
			call	sub_fcreate
			or		a, a
			call	nz, put_error

			ld		hl, 0
			ld		b, 4
	loop_file_write_jis1:
			ld		de, buffer
			push	bc
			ld		bc, 1024
	loop_block_transfer_jis1:
			push	bc
			push	hl
			ld		a, l
			and		a, 0x3F
			add		hl, hl
			add		hl, hl
			out		[0xD8], a
			ld		a, h
			and		a, 0x3F
			out		[0xD9], a
			ld		b, 32
	loop_font_jis1:
			in		a, [0xD9]
			ld		[de], a
			inc		de
			djnz	loop_font_jis1
			pop		hl
			inc		hl
			pop		bc
			dec		bc
			ld		a, c
			or		a, b
			jr		nz, loop_block_transfer_jis1

			push	hl
			ld		hl, fcb
			ld		de, buffer
			ld		bc, 0x8000
			call	sub_fwrite

			ld		c, _CONOUT
			ld		e, '*'
			call	bdos

			pop		hl
			pop		bc
			djnz	loop_file_write_jis1

			ld		hl, 0
			ld		b, 4
	loop_file_write_jis2:
			ld		de, buffer
			push	bc
			ld		bc, 1024
	loop_block_transfer_jis2:
			push	bc
			push	hl
			ld		a, l
			and		a, 0x3F
			add		hl, hl
			add		hl, hl
			out		[0xDA], a
			ld		a, h
			and		a, 0x3F
			out		[0xDB], a
			ld		b, 32
	loop_font_jis2:
			in		a, [0xDB]
			ld		[de], a
			inc		de
			djnz	loop_font_jis2
			pop		hl
			inc		hl
			pop		bc
			dec		bc
			ld		a, c
			or		a, b
			jr		nz, loop_block_transfer_jis2

			push	hl
			ld		hl, fcb
			ld		de, buffer
			ld		bc, 0x8000
			call	sub_fwrite

			ld		c, _CONOUT
			ld		e, '*'
			call	bdos

			pop		hl
			pop		bc
			djnz	loop_file_write_jis2

			ld		hl, fcb
			call	sub_fclose
			call	put_crlf

			; MSX-DOS1
			ld		de, s_dos1
			ld		c, _STROUT
			call	bdos
			ld		a, 0x8F				; SLOT#3-3
			ld		hl, 0x4000
			ld		de, buffer
			ld		bc, 0x8000
			call	rom_dump

			ld		hl, s_dos1_rom_file_name
			ld		de, fcb
			call	sub_fcreate
			or		a, a
			call	nz, put_error
			ld		hl, fcb
			ld		de, buffer
			ld		bc, 0x4000
			call	sub_fwrite
			ld		hl, fcb
			call	sub_fclose
			call	put_crlf

			; MSX-DOS2
			ld		de, s_dos2
			ld		c, _STROUT
			call	bdos

			ld		hl, s_dos2_rom_file_name
			ld		de, fcb
			call	sub_fcreate
			or		a, a
			call	nz, put_error

			ld		a, 0
			call	dos2_dump
			ld		a, 1
			call	dos2_dump
			ld		a, 2
			call	dos2_dump
			ld		a, 3
			call	dos2_dump

			ld		hl, fcb
			call	sub_fclose
			call	put_crlf

			ld		de, s_completed
			ld		c, _STROUT
			call	bdos
			ret

dos2_dump:
			push	af
			ld		a, 0x87				; SLOT#3-1
			ld		h, 0x40
			call	enaslt
			pop		af

			ld		[0x6000], a			; MegaROMバンク切り替え
			ld		de, 0x8000
			ld		hl, 0x4000
			ld		bc, 0x4000
			ldir

			xor		a, a
			ld		[0x6000], a			; MegaROMバンク切り替え

			ld		a, [0xFCC1]			; SLOT#0-0
			ld		h, 0x40
			call	enaslt

			ld		hl, fcb
			ld		de, 0x8000
			ld		bc, 0x4000
			call	sub_fwrite
			ret

err_bad_file_name::
put_error:
			ld		de, s_fopen_error
			ld		c, _STROUT
			call	bdos
			ret
put_crlf:
			ld		de, s_crlf
			ld		c, _STROUT
			call	bdos
			ret

s_title:
			db		"MSX0 BIOS Dumper", 13, 10
			db		"Programmed by t.hara", 13, 10
s_crlf:
			db		13, 10, '$'
s_main_rom:
			db		"Main ROM  :$"
s_sub_rom:
			db		"Sub ROM   :$"
s_xbasic:
			db		"X-BASIC   :$"
s_iotbasic:
			db		"IoTBASIC  :$"
s_kanjibasic:
			db		"KanjiBASIC:$"
s_kanjifoont:
			db		"KanjiROM  :$"
s_dos1:
			db		"MSXDOS1   :$"
s_dos2:
			db		"MSXDOS2   :$"
s_fopen_error:
			db		"fopen error.$"
s_completed:
			db		"Completed.$"
s_main_rom_file_name:
			db		12, "MSX0BIOS.ROM"
s_sub_rom_file_name:
			db		11, "MSX0EXT.ROM"
s_xbasic_rom_file_name:
			db		12, "MSX0XBAS.ROM"
s_iotbasic_rom_file_name:
			db		11, "MSX0IOT.ROM"
s_kanjidriver_rom_file_name:
			db		11, "MSX0KDR.ROM"
s_kanjifont_rom_file_name:
			db		11, "MSX0KFN.ROM"
s_dos1_rom_file_name:
			db		11, "MSX0DOS.ROM"
s_dos2_rom_file_name:
			db		11, "MSX0DOS2.ROM"
fcb:
			space	37
			endscope

; =============================================================================
;	A: 対象となるスロット
;	HL: ダンプ対象となるアドレス
;	DE: 格納先アドレス
;	BC: サイズ
			scope	rom_dump
rom_dump::
			push	af
	loop:
			pop		af
			push	af
			push	hl
			push	de
			push	bc
			call	rdslt
			pop		bc
			pop		de
			pop		hl
			ld		[de], a
			inc		de
			inc		hl
			dec		bc
			ld		a, b
			and		a, 0x1F
			or		a, c
			jr		nz, skip1
			; * を表示
			push	hl
			push	de
			push	bc
			ld		c, _CONOUT
			ld		e, '*'
			call	bdos
			pop		bc
			pop		de
			pop		hl
	skip1:
			ld		a, c
			or		a, b
			jr		nz, loop
			pop		af
			ret
			endscope

; =============================================================================
;	新しいFCBを生成する
;	input:
;		HL ... ファイル名
;		DE ... FCB用のメモリのアドレス (37bytes)
;	output:
;		none
;	break:
;		all
;	comment:
;		ワイルドカード '*' は、'?' に展開する
; =============================================================================
			scope	sub_setup_fcb
sub_setup_fcb::
			; 中身をクリアする
			push	hl
			push	de
			ld		l, e
			ld		h, d
			inc		de
			ld		bc, 36
			ld		[hl], 0
			ldir
			; カレントドライブ取得
			ld		c, _CURDRV
			call	bdos
			inc		a
			pop		de
			pop		hl
			ld		[de], a
			; ドライブ名の存在チェック
			ld		a, [hl]
			inc		hl
			ld		c, a				; C = 長さ
			cp		a, 3
			jr		c, copy_file_name
			inc		hl
			ld		a, [hl]				; 2nd char
			dec		hl
			cp		a, ':'
			jr		nz, copy_file_name
			ld		a, [hl]				; 1st char
			or		a, a
			jp		m, copy_file_name
			and		a, 0b1101_1111
			cp		a, 'H'+1
			jr		nc, copy_file_name
			sub		a, 'A'
			jr		c, copy_file_name
			inc		a
			ld		[de], a				; Driver Number 1:A, 2:B, ... 8:H
			inc		hl
			inc		hl
			dec		c
			dec		c
			; ファイル名(Max 8文字) のコピー
		copy_file_name:
			inc		de					; DE: ファイル名
			ld		b, 8				; B: ファイル名の最大サイズ
			inc		c					; つじつま合わせ
		copy_file_name_loop:
			dec		c
			jr		z, fill_name_padding	; ファイル名のコピーは終わった
			ld		a, [hl]
			cp		a, '.'
			jr		z, copy_ext_name_skip_dot
			cp		a, '*'
			jr		z, copy_ext_name_skip_wildcard
			call	check_error_char
			jp		c, err_bad_file_name
			ld		[de], a
			inc		hl
			inc		de
			djnz	copy_file_name_loop
			; ピッタリ8文字の名前が指定されている場合に . があるかチェック
			ld		a, [hl]
			cp		a, '.'
			jr		nz, copy_ext_name
			inc		hl
			jr		copy_ext_name
			; ファイル名の残りをワイルドカードで埋める
		copy_ext_name_skip_wildcard:
			ld		a, '?'
			inc		hl
			jr		copy_ext_name_skip_dot_loop
			; ファイル名の残りの隙間をスキップ
		fill_name_padding:
			dec		hl					; '.' を読み飛ばす処理のつじつま合わせ
			inc		c					; '.' を読み飛ばす処理のつじつま合わせ
		copy_ext_name_skip_dot:
			ld		a, ' '
		copy_ext_name_skip_dot_loop:
			ld		[de], a				; 隙間は ' ' で埋める
			inc		de					; 拡張子の位置まで移動
			djnz	copy_ext_name_skip_dot_loop
			inc		hl					; '.' を読み飛ばす
			dec		c					; '.' を読み飛ばす
			; 拡張子(Max 3文字) のコピー
		copy_ext_name:
			ld		b, 3
			inc		c
			dec		c
			jr		z, fill_ext_padding
		copy_ext_name_loop:
			ld		a, [hl]
			cp		a, '*'
			jr		z, copy_ext_name_padding_wildcard
			call	check_error_char
			jp		c, err_bad_file_name
			ld		[de], a
			inc		hl
			inc		de
			dec		c
			jr		z, copy_ext_name_padding
			djnz	copy_ext_name_loop
			jr		copy_name_finish
			; * を ? に置換
		copy_ext_name_padding_wildcard:
			ld		a, '?'
			jr		copy_ext_name_padding_loop
			; 拡張子が 3文字未満ならパディング
		copy_ext_name_padding:
			dec		b
			jr		z, copy_name_finish
		fill_ext_padding:
			ld		a, ' '
		copy_ext_name_padding_loop:
			ld		[de], a
			inc		de
			djnz	copy_ext_name_padding_loop
			; ファイル名以外のフィールドを初期化する
		copy_name_finish:
			ret

	check_error_char:
			push	hl
			push	bc
			ld		hl, error_char
			ld		b, a
	check_error_char_loop:
			ld		a, [hl]
			or		a, a
			jr		z, check_error_exit		; 不正な記号には一致しなかった
			cp		a, b
			scf
			jr		z, check_error_exit		; 不正な記号に一致した
			inc		hl
			jr		check_error_char_loop
	check_error_exit:
			ld		a, b
			pop		bc
			pop		hl
			ret		c						; エラーなら抜ける
	toupper:
			cp		a, 'a'
			jr		c, toupper_exit
			cp		a, 'z'+1
			jr		nc, toupper_exit
			and		a, 0b1101_1111			; a → A ... z → Z
	toupper_exit:
			or		a, a					; Cf = 0
			ret
	error_char:
			db		":\"\\^|<>,./ ", 0
			endscope

; =============================================================================
;	ファイルを作る
;	input:
;		HL ... ファイル名
;		DE ... FCB用のメモリのアドレス (37bytes)
;	output:
;		A .... 00h: 成功, FFh: 失敗
;	break:
;		all
;	comment:
;		存在しなければ生成し、存在していれば作り直す
; =============================================================================
			scope	sub_fcreate
sub_fcreate::
			push	de
			call	sub_setup_fcb
			pop		de
			push	de
			ld		c, _FMAKE
			call	bdos
			pop		hl
			ld		de, 14
			add		hl, de
			push	af
			ld		a, 1
			ld		[hl], a
			dec		a
			inc		hl
			ld		[hl], a
			ld		de, 33-15
			add		hl, de
			ld		[hl], a
			inc		hl
			ld		[hl], a
			inc		hl
			ld		[hl], a
			inc		hl
			ld		[hl], a
			pop		af
			ret
			endscope

; =============================================================================
;	ファイルを閉じる
;	input:
;		HL ... 開いたFCB
;	output:
;		A .... 00h: 成功, FFh: 失敗
;	break:
;		all
;	comment:
;		none
; =============================================================================
			scope	sub_fclose
sub_fclose::
			ld		c, _FCLOSE
			ex		de, hl
			jp		bdos
			endscope

; =============================================================================
;	ファイルへ書き込む
;	input:
;		HL ... 開いたFCB
;		DE ... 書き出す内容が格納されているアドレス
;		BC ... 書き出すサイズ
;	output:
;		A .... 00h: 成功, 01h: 失敗
;	break:
;		all
;	comment:
;		none
; =============================================================================
			scope	sub_fwrite
sub_fwrite::
			push	bc					; サイズ
			push	hl					; FCB
			; 転送元アドレスの指定
			ld		c, _SETDTA
			call	bdos
			; 読み出し
			pop		de					; FCB
			pop		hl					; サイズ = レコード数 (1レコード 1byte設定)
			ld		c, _WRBLK
			jp		bdos
			endscope

buffer::
