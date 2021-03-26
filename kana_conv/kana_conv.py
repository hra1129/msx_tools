#!/usr/bin/env python3
# -*- coding=utf-8 -*-

import sys
import re

replace_list = [
	( '♠', r'\\x80' ), ( '♥', r'\\x81' ), ( '♣', r'\\x82' ), ( '◆', r'\\x83' ), ( '○', r'\\x84' ), ( '●', r'\\x85' ), ( 'を', r'\\x86' ), 
	( 'ぁ', r'\\x87' ), ( 'ぃ', r'\\x88' ), ( 'ぅ', r'\\x89' ), ( 'ぇ', r'\\x8A' ), ( 'ぉ', r'\\x8B' ), 
	( 'ゃ', r'\\x8C' ), ( 'ゅ', r'\\x8D' ), ( 'ょ', r'\\x8E' ), ( 'っ', r'\\x8F' ), 
	( 'あ', r'\\x91' ), ( 'い', r'\\x92' ), ( 'う', r'\\x93' ), ( 'え', r'\\x94' ), ( 'お', r'\\x95' ), 
	( 'か', r'\\x96' ), ( 'き', r'\\x97' ), ( 'く', r'\\x98' ), ( 'け', r'\\x99' ), ( 'こ', r'\\x9A' ), 
	( 'さ', r'\\x9B' ), ( 'し', r'\\x9C' ), ( 'す', r'\\x9D' ), ( 'せ', r'\\x9E' ), ( 'そ', r'\\x9F' ), 
	( '。', r'\\xA1' ), ( '「', r'\\xA2' ), ( '」', r'\\xA3' ), ( '、', r'\\xA4' ), ( '・', r'\\xA5' ), ( 'ヲ', r'\\xA6' ), 
	( 'ァ', r'\\xA7' ), ( 'ィ', r'\\xA8' ), ( 'ゥ', r'\\xA9' ), ( 'ェ', r'\\xAA' ), ( 'ォ', r'\\xAB' ), 
	( 'ャ', r'\\xAC' ), ( 'ュ', r'\\xAD' ), ( 'ョ', r'\\xAE' ), ( 'ッ', r'\\xAF' ), ( 'ー', r'\\xB0' ), 
	( 'ア', r'\\xB1' ), ( 'イ', r'\\xB2' ), ( 'ウ', r'\\xB3' ), ( 'エ', r'\\xB4' ), ( 'オ', r'\\xB5' ), 
	( 'カ', r'\\xB6' ), ( 'キ', r'\\xB7' ), ( 'ク', r'\\xB8' ), ( 'ケ', r'\\xB9' ), ( 'コ', r'\\xBA' ), 
	( 'サ', r'\\xBB' ), ( 'シ', r'\\xBC' ), ( 'ス', r'\\xBD' ), ( 'セ', r'\\xBE' ), ( 'ソ', r'\\xBF' ), 
	( 'タ', r'\\xC0' ), ( 'チ', r'\\xC1' ), ( 'ツ', r'\\xC2' ), ( 'テ', r'\\xC3' ), ( 'ト', r'\\xC4' ), 
	( 'ナ', r'\\xC5' ), ( 'ニ', r'\\xC6' ), ( 'ヌ', r'\\xC7' ), ( 'ネ', r'\\xC8' ), ( 'ノ', r'\\xC9' ), 
	( 'ハ', r'\\xCA' ), ( 'ヒ', r'\\xCB' ), ( 'フ', r'\\xCC' ), ( 'ヘ', r'\\xCD' ), ( 'ホ', r'\\xCE' ), 
	( 'マ', r'\\xCF' ), ( 'ミ', r'\\xD0' ), ( 'ム', r'\\xD1' ), ( 'メ', r'\\xD2' ), ( 'モ', r'\\xD3' ), 
	( 'ヤ', r'\\xD4' ), ( 'ユ', r'\\xD5' ), ( 'ヨ', r'\\xD6' ), 
	( 'ラ', r'\\xD7' ), ( 'リ', r'\\xD8' ), ( 'ル', r'\\xD9' ), ( 'レ', r'\\xDA' ), ( 'ロ', r'\\xDB' ), 
	( 'ワ', r'\\xDC' ), ( 'ン', r'\\xDD' ), ( '゛', r'\\xDE' ), ( '゜', r'\\xDF' ), 
	( 'た', r'\\xE0' ), ( 'ち', r'\\xE1' ), ( 'つ', r'\\xE2' ), ( 'て', r'\\xE3' ), ( 'と', r'\\xE4' ), 
	( 'な', r'\\xE5' ), ( 'に', r'\\xE6' ), ( 'ぬ', r'\\xE7' ), ( 'ね', r'\\xE8' ), ( 'の', r'\\xE9' ), 
	( 'は', r'\\xEA' ), ( 'ひ', r'\\xEB' ), ( 'ふ', r'\\xEC' ), ( 'へ', r'\\xED' ), ( 'ほ', r'\\xEE' ), 
	( 'ま', r'\\xEF' ), ( 'み', r'\\xF0' ), ( 'む', r'\\xF1' ), ( 'め', r'\\xF2' ), ( 'も', r'\\xF3' ), 
	( 'や', r'\\xF4' ), ( 'ゆ', r'\\xF5' ), ( 'よ', r'\\xF6' ), 
	( 'ら', r'\\xF7' ), ( 'り', r'\\xF8' ), ( 'る', r'\\xF9' ), ( 'れ', r'\\xFA' ), ( 'ろ', r'\\xFB' ), 
	( 'わ', r'\\xFC' ), ( 'ん', r'\\xFD' ), ( '■', r'\\xFF' ), 
	( '月', r'\\x1\\x41' ), ( '火', r'\\x1\\x42' ), ( '水', r'\\x1\\x43' ), ( '木', r'\\x1\\x44' ), 
	( '金', r'\\x1\\x45' ), ( '土', r'\\x1\\x46' ), ( '日', r'\\x1\\x47' ), ( '年', r'\\x1\\x48' ), 
	( '円', r'\\x1\\x49' ), ( '時', r'\\x1\\x4A' ), ( '分', r'\\x1\\x4B' ), ( '秒', r'\\x1\\x4C' ), 
	( '百', r'\\x1\\x4D' ), ( '千', r'\\x1\\x4E' ), ( '万', r'\\x1\\x4F' ), ( 'π', r'\\x1\\x50' ), 
	( '┴', r'\\x1\\x51' ), ( '┬', r'\\x1\\x52' ), ( '┤', r'\\x1\\x53' ), ( '├', r'\\x1\\x54' ), 
	( '┼', r'\\x1\\x55' ), ( '│', r'\\x1\\x56' ), ( '─', r'\\x1\\x57' ), ( '┌', r'\\x1\\x58' ), 
	( '┐', r'\\x1\\x59' ), ( '└', r'\\x1\\x5A' ), ( '┘', r'\\x1\\x5B' ), ( '×', r'\\x1\\x5C' ), 
	( '大', r'\\x1\\x5D' ), ( '中', r'\\x1\\x5E' ), ( '小', r'\\x1\\x5F' ),
	( 'が', r'\\x96\\xDE' ), ( 'ぎ', r'\\x97\\xDE' ), ( 'ぐ', r'\\x98\\xDE' ), ( 'げ', r'\\x99\\xDE' ), ( 'ご', r'\\x9A\\xDE' ), 
	( 'ざ', r'\\x9B\\xDE' ), ( 'じ', r'\\x9C\\xDE' ), ( 'ず', r'\\x9D\\xDE' ), ( 'ぜ', r'\\x9E\\xDE' ), ( 'ぞ', r'\\x9F\\xDE' ), 
	( 'だ', r'\\xE0\\xDE' ), ( 'ぢ', r'\\xE1\\xDE' ), ( 'づ', r'\\xE2\\xDE' ), ( 'で', r'\\xE3\\xDE' ), ( 'ど', r'\\xE4\\xDE' ), 
	( 'ば', r'\\xEA\\xDE' ), ( 'び', r'\\xEB\\xDE' ), ( 'ぶ', r'\\xEC\\xDE' ), ( 'べ', r'\\xED\\xDE' ), ( 'ぼ', r'\\xEE\\xDE' ), 
	( 'ガ', r'\\xB6\\xDE' ), ( 'ギ', r'\\xB7\\xDE' ), ( 'グ', r'\\xB8\\xDE' ), ( 'ゲ', r'\\xB9\\xDE' ), ( 'ゴ', r'\\xBA\\xDE' ), 
	( 'ザ', r'\\xBB\\xDE' ), ( 'ジ', r'\\xBC\\xDE' ), ( 'ズ', r'\\xBD\\xDE' ), ( 'ゼ', r'\\xBE\\xDE' ), ( 'ゾ', r'\\xBF\\xDE' ), 
	( 'ダ', r'\\xC0\\xDE' ), ( 'ヂ', r'\\xC1\\xDE' ), ( 'ヅ', r'\\xC2\\xDE' ), ( 'デ', r'\\xC3\\xDE' ), ( 'ド', r'\\xC4\\xDE' ), 
	( 'バ', r'\\xCA\\xDE' ), ( 'ビ', r'\\xCB\\xDE' ), ( 'ブ', r'\\xCC\\xDE' ), ( 'ベ', r'\\xCD\\xDE' ), ( 'ボ', r'\\xCE\\xDE' ), 
	( 'ぱ', r'\\xEA\\xDF' ), ( 'ぴ', r'\\xEB\\xDF' ), ( 'ぷ', r'\\xEC\\xDF' ), ( 'ぺ', r'\\xED\\xDF' ), ( 'ぽ', r'\\xEE\\xDF' ), 
	( 'パ', r'\\xCA\\xDF' ), ( 'ピ', r'\\xCB\\xDF' ), ( 'プ', r'\\xCC\\xDF' ), ( 'ペ', r'\\xCD\\xDF' ), ( 'ポ', r'\\xCE\\xDF' ), 
]

def text_replacer( s_source_name, s_destination_name ):
	with open( s_source_name, 'r', encoding="shift_jis" ) as f:
		file_image = f.readlines()

	for replace_touple in replace_list:
		result = []
		for line in file_image:
			line = re.sub( replace_touple[0], replace_touple[1], line )
			result.append( line )
		file_image = result

	with open( s_destination_name, 'w', encoding="shift_jis" ) as f:
		f.writelines( file_image )

if __name__ == '__main__':
	print( "SJIS code to MSX character converter" )
	print( "===============================================" )
	print( "Mar./27th/2021  HRA!" )
	if( len( sys.argv ) < 3 ):
		print( "Usage> %s <input.txt> <output.txt>" % sys.argv[0] )
		exit()
	text_replacer( sys.argv[1], sys.argv[2] )
