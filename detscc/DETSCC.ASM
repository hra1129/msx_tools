; ==============================================================================
;	SCC Detect Routine
;
;  Copyright (C) 2021-2022 Takayuki Hara (HRA!)
;  All rights reserved.
; ------------------------------------------------------------------------------
;   Date        Author  Ver  Description
;   2021/01/11  t.hara  1.0  1st release
;   2022/11/21  t.hara  1.1  OCM-PLDの内蔵MegaSCCを検出できないバグを修正
;                            Snathcher Sound Cartridge を "SCC or ESE-SCC" と誤認するバグを修正
; ==============================================================================

		include		"MSXBIOS.ASM"
		include		"MSXDOS1.ASM"

		org			0x100
; ==============================================================================
;	エントリーポイント
; ==============================================================================
		scope		entry_point
entry::
		ld			sp, [TPA_BOTTOM]

		ld			de, exec_message
		ld			c, D1F_STROUT
		call		bdos

		call		detect_scc
		call		dump_scc
		call		check_wave45

		; page2 (8000h-BFFFh) を RAMスロットに戻す
		ld			a, [ramad2]
		ld			h, 0x80
		call		enaslt
		ei

		ld			c, D1F_TERM0
		jp			bdos

exec_message:
		ds			"DETSCC v1.1  Copyright(C)2021-2022 HRA!\r\n"
		ds			"=========================\n\r$"
		endscope

; ==============================================================================
;	SCC検出ルーチン
;	input)
;		None
;	output)
;		scc_slot_info にスロット情報が詰められる
; ==============================================================================
scc_bank2					:= 0x9000
scc_bank3					:= 0xB000
scc_reg_a					:= 0x7FFE
scc_reg_b					:= 0xBFFE

scc_reg_b_ram_write_en		:= 0b0001_0000
scc_reg_b_new_mode			:= 0b0010_0000
scc_bank2_wave_bank			:= 0x3F
scc_bank3_wave_bank			:= 0x80

		scope		detect_scc
detect_scc::
		; slot_info を FF で埋める
		ld			hl, slot_info
		ld			de, slot_info + 1
		ld			bc, 2 * 16 - 1
		ld			a, 0xFF
		ld			[hl], a
		ldir
		ld			de, slot_info
		; スロットの拡張状況を調べる
		ld			hl, exptbl
		xor			a, a				; SLOT#0 から開始
base_slot_loop:
		ld			b, a
		ld			a, [hl]
		and			a, 0x80				; A = 00h: 拡張スロット無し, 80h: 拡張スロットあり
		or			a, b
		ld			[current_slot], a
		push		hl
		push		de
check_slot:
		; page2 (8000h-BFFFh) を 着目スロットに切り替える
		ld			h, 0x80
		call		enaslt
		; scc_bank2 (の可能性のあるアドレス) の内容を保存してから
		; レジスタ出現バンクに設定する値を書き込む
		ld			a, [scc_bank2]
		ld			[backup_scc_bank2], a
		ld			a, scc_bank2_wave_bank
		ld			[scc_bank2], a
		ld			a, [scc_bank3]
		ld			[backup_scc_bank3], a
		ld			a, scc_bank3_wave_bank
		ld			[scc_bank3], a
		; scc_reg_b (の可能性のあるアドレス) の内容を保存してから
		; 互換モードに設定する値を書き込む (SCC-I で DRAMモードだとバンクレジスタもDRAMになってるため次のチェックで RAM と誤判定する)
		ld			a, [scc_reg_b]
		ld			[backup_scc_reg_b], a
		xor			a, a
		ld			[scc_reg_b], a
		; バンクレジスタは Write only
		ld			hl, scc_bank2
		call		check_access
		jp			z, is_not_scc		; RAM だったら is_not_scc へ
		ld			hl, scc_bank3
		call		check_access
		jp			z, is_not_scc		; RAM だったら is_not_scc へ
		; バンクレジスタを再設定
		ld			a, scc_bank2_wave_bank
		ld			[scc_bank2], a
		ld			a, scc_bank3_wave_bank
		ld			[scc_bank3], a
		; 98A0h は SCC+ なら Ch.E の波形(ReadOnly), SCC なら無効。ここが書けないことを確認する
		; OCM-PLD 3.9.1-2nd以前では書き込めてしまうので、このチェックはスキップ。
;		ld			hl, 0x9880
;		call		check_access
;		jp			z, is_not_scc		; RAM だったら is_not_scc へ
;		ld			hl, 0x9888
;		call		check_access
;		jp			z, is_not_scc		; RAM だったら is_not_scc へ
;		ld			hl, 0x9890
;		call		check_access
;		jp			z, is_not_scc		; RAM だったら is_not_scc へ
;		ld			hl, 0x9898
;		call		check_access
;		jp			z, is_not_scc		; RAM だったら is_not_scc へ
		; 9800h は SCC/SCC+ なら Ch.A の波形(Read/Write)。ここが書けることを確認する
		ld			hl, 0x9800
		call		check_access
		jp			nz, is_not_scc		; ROM だったら is_not_scc へ
		ld			hl, 0x9820
		call		check_access
		jp			nz, is_not_scc		; ROM だったら is_not_scc へ
		ld			hl, 0x9840
		call		check_access
		jp			nz, is_not_scc		; ROM だったら is_not_scc へ
		ld			hl, 0x9860
		call		check_access
		jp			nz, is_not_scc		; ROM だったら is_not_scc へ
		; この時点で SCC/SCC-I のどちらかであることは確定
		; 固有モードに設定する値を書き込む
		ld			a, scc_reg_b_new_mode
		ld			[scc_reg_b], a
		; B800h は SCC-I なら Ch.A の波形(Read/Write)。ここが書けることを確認する
		ld			hl, 0xB800
		call		check_access
		ld			a, slot_info_scc
		jp			nz, removal					; ROM だったら removal へ
		; B8A0h は SCC-I なら Ch.A の周波数(Write Only)。ここに書いた値を読み出せないことを確認する
		; OCM-PLD 3.9.1-2nd以前では読み出せてしまうので、このチェックはスキップ。
;;		ld			hl, 0xB8A0
;		call		check_access
;		ld			a, slot_info_scc
;		jp			z, removal					; ROM だったら removal へ
		; この時点で SCC-I であることは確定
		; RAMモードに設定する値を書き込む
		ld			a, scc_reg_b_new_mode | scc_reg_b_ram_write_en
		ld			[scc_reg_b], a

		; Bank#0 を bank2 に出現させてRAMか調べる
		xor			a, a
		call		change_bank2
		ld			hl, 0x8000
		call		check_access
		jp			nz, check_sd_snatcher		; ROM だったら SD-Snatcherサウンドカートリッジ か 似非SCC-I のどちらか

		; Bank#8 を bank2 に出現させてRAMか調べる
		ld			a, 8
		call		change_bank2
		ld			hl, 0x8000
		call		check_access
		ld			a, slot_info_scci_sna
		jp			nz, removal					; ROM だったら Snatcherサウンドカートリッジに確定

		ld			a, slot_info_scci_exp		; 128KB化サウンドカートリッジに確定
		jp			removal

check_sd_snatcher:
		; Bank#8 を bank2 に出現させてRAMか調べる
		ld			a, 8
		call		change_bank2
		ld			hl, 0x8000
		call		check_access
		ld			a, slot_info_scci_ese
		jp			nz, removal					; ROM だったら 似非SCC-I に確定

		ld			a, slot_info_scci_sds		; SD-Snatcherサウンドカートリッジに確定
		jp			removal

removal:
		push		af
		xor			a, a
		ld			[scc_reg_b], a				; 互換モードに戻す
		ld			a, 2
		ld			[scc_bank2], a				; Bank2 は Bank#2 に戻す
		inc			a
		ld			[scc_bank3], a				; Bank3 は Bank#3 に戻す
		pop			af
		jp			check_next_slot

is_not_scc:
		; レジスタのつもりで書き替えた内容を書き戻す
		ld			a, [backup_scc_reg_b]
		ld			[scc_reg_b], a
		ld			a, [backup_scc_bank3]
		ld			[scc_bank3], a
		ld			a, [backup_scc_bank2]
		ld			[scc_bank2], a
		ld			a, slot_info_unknown

check_next_slot:
		pop			hl						; HL = slot_info[ current_slot ]
		ld			b, a
		ld			a, [current_slot]
		ld			[hl], a
		inc			hl
		ld			[hl], b
		inc			hl

		or			a, a					; Is this expanded slot?
		jp			p, slot_is_not_expanded

		add			a, 0x04		; 次の拡張スロット番号にインクリメントする
		bit			4, a		; SLOT#X-4 になってしまったら、基本スロットのインクリメントへ。
		jp			nz, next_basic_slot
		ld			[current_slot], a
		push		hl
		jp			check_slot

slot_is_not_expanded:
		ld			de, 6
		add			hl, de
next_basic_slot:
		ex			de, hl
		pop			hl
		ld			a, l
		cp			a, 0xC4
		ret			z

		; EXPTBL のアドレスの値を SLOT#X の X に変換する
		inc			hl
		and			a, 0x03
		jp			base_slot_loop

; RAMモード時に Bank2 を Aレジスタで指定の Bank# に切り替える
change_bank2:
		push		af
		xor			a, a
		ld			[scc_reg_b], a				; 一度、互換モードに戻してバンクレジスタを出現させる
		pop			af
		ld			[scc_bank2], a				; バンクレジスタに指定の値を書き込む
		ld			a, scc_reg_b_ram_write_en
		ld			[scc_reg_b], a				; RAM書き込みモードに戻す
		ret

; [HL] が ROMなら NZ, RAMなら Z になる。AFレジスタが破壊される。
check_access:
		ld			a, [hl]
		cpl
		ld			[hl], a
		cp			a, [hl]
		cpl
		ld			[hl], a
		ret

current_slot:
		db			0
backup_scc_reg_b:
		db			0
backup_scc_bank2:
		db			0
backup_scc_bank3:
		db			0
slot_info::
		space		2 * 16, 0xFF	; 2byte * 16slot : slot_info[0] = スロット番号, slot_info[1] = slot_info_***

slot_info_inactive_slot	:= 0xFF		; 存在しないスロット
slot_info_unknown		:= 0x00		; 無関係なスロット
slot_info_scc			:= 0x01		; SCC または 似非SCC
slot_info_scci_sna		:= 0x02		; SCC-I, Snacher (64KB RAM)
slot_info_scci_sds		:= 0x03		; SCC-I, SD-Snacher (64KB RAM)
slot_info_scci_exp		:= 0x04		; SCC-I, Snacher + SD-Snacher (128KB RAM)
slot_info_scci_ese		:= 0x05		; 似非SCC-I
		endscope

; ==============================================================================
;	SLOT番号を表示
;	input)
;		A ... 表示するスロット番号
;	break)
;		Bレジスタは保存される
; ==============================================================================
		scope		print_slot_num
print_slot_num::
		; スロット番号を文字列にセットする
		ld			c, a						; Cにスロット番号をバックアップ
		and			a, 0x03						; Aに基本スロット番号
		add			a, '0'
		ld			[s_slot_num_value], a

		; 基本スロット番号を表示する
		push		bc
		ld			de, s_slot_num
		ld			c, D1F_STROUT
		call		bdos
		pop			bc

		; 拡張スロットか？
		ld			a, c
		or			a, a
		jp			m, is_expanded_slot

		; 拡張されていないので桁揃えだけ表示
		push		bc
		ld			de, s_indent
		ld			c, D1F_STROUT
		call		bdos
		pop			bc
		jp			skip1

is_expanded_slot:
		push		bc
		; 拡張スロット番号を文字列にセットする
		rrca
		rrca
		and			a, 0x03
		add			a, '0'
		ld			[s_expslot_num_value], a

		ld			de, s_expslot_num
		ld			c, D1F_STROUT
		call		bdos
		pop			bc

skip1:
		ret

s_slot_num::
		ds			"SLOT#"
s_slot_num_value::
		ds			"0$"

s_indent::
		ds			"   : $"

s_expslot_num::
		ds			"-"
s_expslot_num_value::
		ds			"0 : $"
		endscope

; ==============================================================================
;	SCC検出結果の表示ルーチン
;	input)
;		scc_slot_info にスロット情報が詰められていること
; ==============================================================================
		scope		dump_scc
dump_scc::
		ld			b, 16
		ld			hl, slot_info
loop:
		push		bc
		ld			a, [hl]			; スロット番号
		inc			hl
		ld			b, [hl]			; 種類
		inc			hl
		push		hl

		; スロットの存在をチェック
		inc			a
		jp			z, invalid_slot
		dec			a

		; スロット番号を表示
		call		print_slot_num

		; テーブルから種別を示す文字列を取得
		ld			a, b
		add			a, a
		ld			l, a
		ld			h, 0
		ld			de, s_type_table
		add			hl, de
		ld			e, [hl]
		inc			hl
		ld			d, [hl]

		; 文字列を表示する
		ld			c, D1F_STROUT
		call		bdos

not_scc_slot:
invalid_slot:
		pop			hl
		pop			bc
		djnz		loop
		ret

s_type_table:
		dw			s_type_scc0, s_type_scc1, s_type_scc2, s_type_scc3, s_type_scc4, s_type_scc5
s_type_scc0:
		ds			"Unknown\n\r$"
s_type_scc1:
		ds			"SCC or ESE SCC\n\r$"
s_type_scc2:
		ds			"SCC-I (Snatcher Sound Cartridge with 64KB RAM)\n\r$"
s_type_scc3:
		ds			"SCC-I (SD-Snatcher Sound Cartridge with 64KB RAM)\n\r$"
s_type_scc4:
		ds			"SCC-I (Sound Cartridge with 128KB RAM)\n\r$"
s_type_scc5:
		ds			"ESE SCC-I (ROM Cartridge)\n\r$"
		endscope

; ==============================================================================
;	SCC互換モードの Ch.D/Ch.E 波形メモリミラーチェック
; ==============================================================================
		scope		check_wave45
check_wave45::
		ld			c, D1F_STROUT
		ld			de, sep
		call		bdos

		ld			b, 16
		ld			hl, slot_info
loop:
		push		bc
		; スロット番号を取得
		ld			a, [hl]
		inc			hl
		; 種別を取得
		ld			b, [hl]
		inc			hl
		push		hl
		; 存在しないスロットなら飛ばす
		inc			a
		jp			z, skip_process
		dec			a
		; page2 のスロットを切り替える
		push		bc
		push		af
		ld			h, 0x80
		call		enaslt
		pop			af
		pop			bc
		; Unknown なら飛ばす
		ld			c, a
		ld			a, b
		or			a, a
		jp			z, skip_process
		; スロット番号を表示する
		ld			a, c
		call		print_slot_num
		; 改行
		ld			c, D1F_STROUT
		ld			de, crlf
		call		bdos
		; 互換モードにする
		xor			a, a
		ld			[scc_reg_b], a
		; SCCレジスタバンクに切り替える
		ld			a, scc_bank2_wave_bank
		ld			[scc_bank2], a
		; Ch.D 波形メモリにインクリメントデータを書き込む
		xor			a, a
		ld			b, 32
		ld			hl, 0x9860			; Ch.D 波形メモリ先頭アドレス。ここから 32byte
write_ch4_wave:
		ld			[hl], a
		inc			a
		inc			hl
		djnz		write_ch4_wave
		; Ch.D 波形をダンプする
		ld			a, 'D'
		ld			hl, 0x9860
		call		dump_wave
		; Ch.E 波形 (ReadOnly, SCC-I Only)をダンプする
		ld			a, 'E'
		ld			hl, 0x98A0
		call		dump_wave
skip_process:
		pop			hl
		pop			bc
		djnz		loop
		ret

dump_wave:
		push		hl
		; "Ch.*" を表示
		ld			[ch_name], a
		ld			c, D1F_STROUT
		ld			de, ch_message
		call		bdos
		pop			hl

		; 32byte をダンプする
		ld			b, 2
dump_wave_loop1:
		push		bc
		ld			b, 16
dump_wave_loop2:
		push		bc
		; 波形メモリから 1byte 読み出し
		ld			a, [hl]
		inc			hl
		push		hl

		; 16進数ダンプに変換
		ld			hl, hex
		ld			c, a

		rrca
		rrca
		rrca
		rrca
		and			a, 0x0F
		add			a, '0'
		cp			a, '9' + 1
		jp			c, skip1
		add			a, 'A' - ('9' + 1)
skip1:
		ld			[hl], a
		inc			hl

		ld			a, c
		and			a, 0x0F
		add			a, '0'
		cp			a, '9' + 1
		jp			c, skip2
		add			a, 'A' - ('9' + 1)
skip2:
		ld			[hl], a

		; 16進数ダンプを表示
		ld			c, D1F_STROUT
		ld			de, hex
		call		bdos

		pop			hl
		pop			bc
		djnz		dump_wave_loop2

		; 改行
		push		hl
		ld			c, D1F_STROUT
		ld			de, crlf
		call		bdos
		pop			hl

		pop			bc
		djnz		dump_wave_loop1
		ret

sep:
		ds			"-------------------------\n\r$"
ch_message:
		ds			"Ch."
ch_name:
		ds			"D\n\r$"
hex:
		ds			"00,$"
crlf:
		ds			"\n\r$"
		endscope
