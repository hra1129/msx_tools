USB Gamepad bridge for MSX
-------------------------------------------------------------------------------

1. なにこれ？
	Raspberry Pi Pico を使って USBゲームパッドを MSX のジョイスティックポート
	に接続して使えるようにするための、「Raspberry Pi Pico上で動作するプログラム」
	です。
	オマケで、MegaDrive向け対応も入れてありますが、MegaDriveを所有していないため
	こちらは動作未確認です。

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
