USB Keybard bridge for MSX
-------------------------------------------------------------------------------

1. なにこれ？
	Raspberry Pi Pico を使って USBキーボードのキー状態を MSX のキーマトリクス信号
	に変換するための、「Raspberry Pi Pico上で動作するプログラム」です。

2. 免責事項
	Pico単体で簡単なテストは行っていますが、これを実際に MSX に繋げて、
	MSXからの Y信号指定からの X信号応答が十分間に合う速度なのかどうかは未確認です。
	ご利用の際には、各自の責任でご利用下さい。
	これを利用することによって、何らかの不具合が発生したとしても、
	HRA! (t.hara) は一切責任を負いませんのでご了承下さい。

3. ビルド
	Raspberry Pi の Sampleプログラム(下記)をビルドできる状態まで環境を整えて下さい。
	https://github.com/raspberrypi/pico-examples

	ローカルに clone した pico-examples の中の CMakeLists.txt に下記を追記します。
	add_subdirectory(usb_keyboard_bridge_for_msx)

	同じ場所にディレクトリ usb_keyboard_bridge_for_msx を作成し、
	ここにある一式（usb_keyboard_bridge_for_msx.c 等）を、そのディレクトリへコピー
	します。

	ディレクトリ build に入り、make または nmake で rebuild_cache します。
	> nmake rebuild_cache

	あとは、普通にビルドします。
	> nmake usb_keyboard_bridge_for_msx

	build/usb_keyboard_bridge_for_msx に usb_keyboard_bridge_for_msx.uf2 が
	出来上がるので、Pico のボタンを押しながら USBケーブルを接続して、
	出てくるストレージに usb_keyboard_bridge_for_msx.uf2 をコピーすると、
	ストレージが自動的に切断されます。これで書き込み完了。

	Pico の USB端子に OTGケーブルを接続し、そこに USBキーボードを接続します。
	usb_keyboard_bridge_for_msx.pdf に示したピンアサインになっているので、
	必要なところに接続してご利用下さい。

4. カスタマイズ
	ソースファイル usb_keyboard_bridge_for_msx.c の上の方に設定が書いてあります。
	必要に応じて書き替えてご利用下さい。

	// --------------------------------------------------------------------
	//	MSX_KEYMATRIX_ROW_TYPE
	//		0: Y3-Y0  に ROWアドレスが指定される
	//		1: Y11-Y0 に ROWアドレスをデコードした結果が指定される (ONE HOT)
	//		2: X8-X4 に ROWアドレスが指定される、Y0にDIR(HB-F500)
	//
	#define MSX_KEYMATRIX_ROW_TYPE 1

	//	MSX_KEYMATRIX_INV
	//		0: Y11-Y0 は正論理である
	//		1: Y11-Y0 は負論理である
	//
	#define MSX_KEYMATRIX_INV 1

	★PPI(i8255)の RegisterC (Y3-Y0) をそのまま接続する場合は、MSX_KEYMATRIX_INV は 0 にしてください。

	//	MSX_KEYMATRIX_ROW_PULL_UP
	//		0: Y0-Y11 の入力ピンに 内蔵 PULL UP/DOWN は使わない
	//		1: Y0-Y11 の入力ピンに 内蔵 PULL UP を使う
	//		2: Y0-Y11 の入力ピンに 内蔵 PULL DOWN を使う
	//
	#define MSX_KEYMATRIX_ROW_PULL_UP 1

	//	MSX_X_HIZ_SEL
	//		0: X出力は H or L である
	//		1: X出力は HiZ or L である
	//
	#define MSX_X_HIZ_SEL 1

5. 注意点
	Raspberry Pi Pico の I/O電圧は 3.3V です。
	一方で MSX は 5V です。
	Pico → MSX の接続は、たいてい直結でOKだと思いますが、必要に応じて
	レベル変換IC等を経由して接続して下さい。
	MSX → Pico の接続は、必要に応じて 10kΩ程度の抵抗器を挟んで接続するか、
	レベル変換IC等を経由して接続して下さい。

	Y信号がデコードされているタイプの場合、L か HiZ の2値を採るケースが多いと
	思います。この場合、本体の X信号を受ける部分で PullUp されているはずです。
	そのような構成の本体で、Pico から X信号に H を出してしまうと、Y信号により
	選択されている行に所属する本体側のキーが押された場合に競合が発生してNGです。
	そのような構成の本体で利用する場合は、MSX_X_HIZ_SEL を 1 にしてください。
	X が L か HiZ になるように設定されます。

6. メモ
	MSXのハード的には、PPI(i8255) の Register C の bit3～bit0 に「行」を
	指定すると、PPI の出力ポートからその値がそのまま出てきます。
	PPIとしては、書き込まれた値をただ出力するだけの汎用I/Oですが、
	この bit3～bit0 を YA～YD などと呼び、これをデコードして Y0～Y10の
	信号を作り出します。
	PanasonicのMSX2+以降では、内蔵ワープロソフト用に[実行][取消]キーを
	搭載していて、このキーを示す行が 12 (Y11) となっています。そのため、
	それ以外の機種では、Y11 は使いません。

	MSX_KEYMATRIX_ROW_TYPE は、デコードされた Y0～Y10(Y11) を Pico に繋ぐ
	場合に 1 を指定します。デコード前の YA～YD を Pico に繋ぐ場合には、
	0 または 2 を指定することになります。

	■MSX_KEYMATRIX_ROW_TYPE 0
	Panasonic FS-A1F 等で S1985 から YA～YD 信号を取り出して Pico に繋げる
	場合は、このタイプを選択します。
	YA～YD をデコードする IC が外付けされており、その出力が Y0～Y10 になって
	いると思いますが、こちらを使う場合は MSX_KEYMATRIX_ROW_TYPE 1 になります。
	接続する場所によって、タイプが異なるのでご注意下さい。

	■MSX_KEYMATRIX_ROW_TYPE 1
	Panasonic FS-A1GT 等のキーボード端子にはデコードされた Y0～Y11 が
	来ているため、そこに繋げる場合は 1 を指定します。
	キーボード側のフィルム基板の配線の都合だと思いますが、同じ信号が複数
	来ているので、それらを繋げる必要もあると思います。

	A1GTの場合、キーボード端子（マザーボード側に2つ並んで付いている黒い
	コネクタ。キーボードユニットのフィルムケーブルが接続される場所。）
	は、CN11・CN12 となっており、下記のようなアサインになっています。

	CN11
		pin1	X6
		pin2	X4
		pin3	X8
		pin4	X0
		pin5	X3
		pin6	X7
		pin7	X1
		pin8	X5
		pin9	X4
		pin10	X7
		pin11	X6
		pin12	X5
		pin13	X0
		pin14	X4
		pin15	X1
		pin16	X2

	CN12
		pin17	Y6
		pin18	Y62
		pin19	Y63
		pin20	Y8
		pin21	PAUSE
		pin22	Y11
		pin23	Y61
		pin24	Y2
		pin25	Y5
		pin26	Y3
		pin27	Y4
		pin28	Y1
		pin29	Y0
		pin30	Y7
		pin31	Y10
		pin32	Y9

		※Y61,Y62,Y63 は Y6 と繋げるようです。

	以下未確認の推定ですが、おそらく Y は選択されている行に対応する信号線が
	L に落ちて、それ以外は HiZ になっていると思います。
	（キーボード側はフィルム基板でスイッチしか付いておらず、Y信号が MSXエンジン
	DAT9769C にそのまま繋がっているためです。）
	そのため、MSX_KEYMATRIX_ROW_PULL_UP は 1 にしておく必要があると思います。

	■MSX_KEYMATRIX_ROW_TYPE 2
	SONY の HB-F500系は、キーボード分離型ですが、キーボードコネクタには
	YA～YD が出てきているようです。しかも、X8～X4 と共通の配線になって
	おり、DIR信号で YA～YD をMSXが出しているタイミングなのか、X8～X4 を
	読み取っているタイミングなのかを選択するようです。XとYでbitの並びが
	逆転しているので、Pico側でビット並び反転処理を実施する必要があります。
	この HB-F500系の場合には、2 を指定することになります。
	Pico側のポートは、X0～X7, Y0 を使います。Pico X0～X7 に、MSX X1～X8 
	を接続。Pico X4～X8 に MSX YD～YA を接続。Pico Y0 に MSX DIR を接続
	となります。Y1～Y11 は使いません。


	MSXの BIOS は、59.94Hz (NTSC時)で入ってくる割り込みのタイミングの
	2回のうち1回だけ、キースキャンルーチンが走るようになっており、
	この中で全行スキャンしています。
	従って、PPI の YA～YD や、そのデコード結果である Y0～Y10(Y11) は
	約 1/30秒 に一度、0～10(11) に連続的に変化するタイミングが来るように
	なっています。
	MSX は、PPI Register C の bit3～bit0 に YA～YD (行) を指定して、
	PPI Register B から X7～X0 を読み取ります。
	1/30秒に 1回実行されるコードは下記のようになっていると思います。

	LD    C, 0A9h
	XOR   A
	LD    B, 11
	LD    HL, 0FBE5h
LOOP:
	OUT   (0AAh), A
	IN    D, (C)
	LD    (HL), D
	INC   HL
	DJNZ  LOOP

	OUT で Y が出てきた後に、IN で読み出すまでの間に X信号を指定されたYに
	基づいた値に確定する必要があるので、Pico はこのタイミングは忙しく動く
	必要があります。
	ここが間に合わないと適切なキー反応を得られません。

7. 注意点 その2
	2021年8月16日現在、出回っている Raspberry Pi Pico には若干の不具合があり、
	この影響でうまく認識できないキーボードや、２回目の接続で認識しなくなるキーボード
	が存在するようです。
	実際、私の所有する無線キーボードは認識しませんし、認識するキーボードでも抜き差しを
	繰り返していると TinyUSB がパニックを起こして停止するのを確認しています。

	FS-A1ST/A1GT を含む MSX-ENGINE2系の場合、Raspberry Pi Pico に内蔵の PullUp抵抗では
	対応しきれないことがあるようです。
	ここで配布しているのはソフトウェアのみなので、電気的な部分に関しては十分に考慮のうえ
	ご利用下さい。

-------------------------------------------------------------------------------
2021年5月12日 ver1.0 HRA!
	初版。

2021年6月12日 ver1.1 HRA!
	TinyUSB の API変更に合わせて修正。

2021年8月16日 HRA!
	注意書き 7 を追加
