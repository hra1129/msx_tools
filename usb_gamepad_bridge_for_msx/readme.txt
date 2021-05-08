USB Gamepad bridge for MSX
-------------------------------------------------------------------------------

1. なにこれ？
	Raspberry Pi Pico を使って USBゲームパッドを MSX のジョイスティックポート
	に接続して使えるようにするための、「Raspberry Pi Pico上で動作するプログラム」
	です。
	オマケで、MegaDrive向け対応も入れてありますが、MegaDriveを所有していないため
	こちらは動作未確認です。MegaDrive向けは、define設定を変えてコンパイルし直す
	必要があります。

2. 免責事項
	実際に1つ作成して、MSX実機 (FS-A1GT) と、FPGA互換機 (8bits4ever SX-2 試作機)
	にて Joymega非対応ゲームを問題なく操作でき、joymega動作確認ツールで方向キーと
	8ボタン認識するのを確認済みですが、以下の2つのゲームパッドしか確認していません。

		BUFFALO BSGP815GY : スーファミタイプ。連射機能付き。利用できました。
		ELECOM JC-U1608TBK: スーファミタイプ。利用できました。

	また、下記は認識されないことを確認済みです。

		SEGA メガドラミニ付属コントローラー: 利用できませんでした。

	他のゲームパッドやジョイスティックなど、使えるかどうかは不明です。
	また、配線ミスによっては Rasberry Pi Pico や、MSX/MegaDrive本体を壊す恐れが
	あります。ご利用の際には、各自の責任でご利用下さい。
	MegaDrive に至っては、一切動作確認をしていないため、配線図の方も正しいか不明
	です。
	これを利用することによって、何らかの不具合が発生したとしても、
	HRA! (t.hara) は一切責任を負いませんのでご了承下さい。

3. ビルド
	ボタンの配置を変えたり、MegaDrive向けに変更したりする場合は、コンパイルし直す
	必要があります。

	Raspberry Pi の Sampleプログラム(下記)をビルドできる状態まで環境を整えて下さい。
	https://github.com/raspberrypi/pico-examples

	ローカルに clone した pico-examples の中の CMakeLists.txt に下記を追記します。
	add_subdirectory(usb_gamepad_bridge_for_msx)

	同じ場所にディレクトリ usb_gamepad_bridge_for_msx を作成し、
	ここにある一式（usb_gamepad_bridge_for_msx.c 等）を、そのディレクトリへコピー
	します。

	pico-sdk/lib の中に、ここに置いてある TinyUSB/pico-sdk/lib の中身を上書きコピーします。
	これにより、TinyUSB に gamepadサポートが追加されます。
	（その他のライブラリがバージョンアップして互換性が失われてしまった場合に、
	正常に機能しなくなるかもしれません。）

	ディレクトリ build に入り、make または nmake で rebuild_cache します。
	> nmake rebuild_cache

	あとは、普通にビルドします。
	> nmake usb_gamepad_bridge_for_msx

	build/usb_gamepad_bridge_for_msx に usb_gamepad_bridge_for_msx.uf2 が
	出来上がるので、Pico のボタンを押しながら USBケーブルを接続して、
	出てくるストレージに usb_gamepad_bridge_for_msx.uf2 をコピーすると、
	ストレージが自動的に切断されます。これで書き込み完了。

	Pico の USB端子に OTGケーブルを接続し、そこに USBゲームパッドを接続します。
	usb_gamepad_bridge_for_msx.pdf に示したピンアサインになっているので、
	必要なところに接続してご利用下さい。

4. カスタマイズ
	ソースファイル usb_gamepad_bridge_for_msx.c の上の方に設定が書いてあります。
	必要に応じて書き替えてご利用下さい。

5. 注意点
	Raspberry Pi Pico の I/O電圧は 3.3V です。
	一方で MSX は 5V です。
	MSX → Pico の接続は、10kΩ程度の抵抗器を挟んで接続して下さい。
	Pico → MSX の接続は、直結でOKです。
