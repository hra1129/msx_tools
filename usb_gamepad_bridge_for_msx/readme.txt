USB Gamepad bridge for MSX
-------------------------------------------------------------------------------

1. なにこれ？
	Raspberry Pi Pico を使って USBゲームパッド/USBマウスを MSX のジョイスティック
	ポートに接続して使えるようにするための、「Raspberry Pi Pico上で動作する
	プログラム」です。
	オマケで、MegaDrive向け対応も入れてありますが、MegaDriveを所有していないため
	こちらは動作未確認です。MegaDrive向けは、define設定を変えてコンパイルし直す
	必要があります。また、MegaDrive対応はゲームパッドのみです。

2. 免責事項
	実際に1つ作成して、MSX実機 (FS-A1GT) と、FPGA互換機 (8bits4ever SX-2 試作機)
	にて Joymega非対応ゲームを問題なく操作でき、joymega動作確認ツールで方向キーと
	8ボタン認識するのを確認済みですが、以下の2つのゲームパッドしか確認していません。

	ゲームパッド
		BUFFALO BSGP815GY : スーファミタイプ。連射機能付き。利用できました。
		ELECOM JC-U1608TBK: スーファミタイプ。連射機能付き。利用できました。

	マウス
		HP USB光学マウス (型番不明, HP製パソコンの付属品)
		ELECOM M-BL27UB

	また、下記は認識されないことを確認済みです。

	ゲームパッド
		SEGA メガドラミニ付属コントローラー: 利用できませんでした。

	また、下記は認識されるものの、Pico の USBドライバが PANIC をおこして停止
	（ハングアップ）することを確認済みです。

	マウス
		Easterntimes Tech 2.4G Wireless Optical Mouse D-09

	他のゲームパッド・ジョイスティック・マウスが使えるかどうかは不明です。
	また、配線ミスによっては Rasberry Pi Pico や、MSX/MegaDrive本体を壊す恐れが
	あります。ご利用の際には、各自の責任でご利用下さい。
	MegaDrive に至っては、一切動作確認をしていないため、配線図の方も正しいか不明
	です。
	これを利用することによって、何らかの不具合が発生したとしても、
	HRA! (t.hara) は一切責任を負いませんのでご了承下さい。

	マウスを繋げた場合、中ボタン（ホイールボタン）を押すと動作モードが切り替わります。

		初期状態：ノーマル動作
		1回押す ：垂直方向だけ半分の速度 (SCREEN7用)
		2回押す ：水平・垂直ともに半分の速度（速すぎるマウス用）
		3回押す ：水平は半分、垂直は 1/4 の速度（速すぎるマウスの SCREEN7用）
		4回押す ：初期状態へ戻る

	LEDの点滅パターンで動作モードを示します。
		1回ゆっくり点滅       ： 初期状態、ゲームパッドモード。
		2回点滅               ： マウス（初期状態）モード。
		3回点滅               ： マウス（1回押す）モード。
		4回点滅               ： マウス（2回押す）モード。
		1回長く、その後2回点滅： マウス（3回押す）モード。

	ゲームパッドやマウスを抜き差ししていると、PicoのUSBドライバが PANIC になり
	ハングアップする場合があります。ハングアップすると、LED は消灯します。
	その場合、MSXから一度外して挿し直してください。

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

------------------------------------------------------------------------------
2021年5月8日 ver1.0 HRA!
	TinyUSB が GAMEPAD/JOYSTICK にまだ対応していないため、TinyUSB自体を修正して
	強引に対応。手持ちのゲームパッド (JOYSTICKとして認識) のみ動作確認。

2021年6月11日 ver1.1 HRA!
	TinyUSB が GAMEPAD対応したことを機に、独自修正を排除。
	TinyUSB の API が大幅に変更になっていたため、それに合わせて修正。
	相変わらず JOYSTICK の方の対応はまだなので、my_hid_joystick_report_t を定義
	して対応。ver1.0 と同様に動作することを確認。
	GAMEPADとして認識するコントローラーにも対応しているつもりだが、そのような
	ゲームパッドを所有していないため、動作未確認。

2021年7月25日 ver2.0 HRA!
	TinyUSB のゲームパッドのボタン定義名が変更になっていたのに対応。
	マウス対応を追加。
