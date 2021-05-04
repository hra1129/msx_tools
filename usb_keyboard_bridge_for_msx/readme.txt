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
	//
	#define MSX_KEYMATRIX_ROW_TYPE 1

	//	MSX_KEYMATRIX_INV
	//		0: Y11-Y0 は正論理である
	//		1: Y11-Y0 は負論理である
	//
	#define MSX_KEYMATRIX_INV 1

	//	MSX_KEYMATRIX_ROW_PULL_UP
	//		0: Y0-Y11 の入力ピンに 内蔵 PULL UP/DOWN は使わない
	//		1: Y0-Y11 の入力ピンに 内蔵 PULL UP を使う
	//		2: Y0-Y11 の入力ピンに 内蔵 PULL DOWN を使う
	//
	#define MSX_KEYMATRIX_ROW_PULL_UP 1

5. 注意点
	Raspberry Pi Pico の I/O電圧は 3.3V です。
	一方で MSX は 5V です。
	MSX → Pico の接続は、10kΩ程度の抵抗器を挟んで接続して下さい。
	Pico → MSX の接続は、直結でOKです。
