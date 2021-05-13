SoundCartridge (Snatcherの方)を Slot#1 に装着して、
MSX-DOS から SCCTEST を実行すると、
　　Success
と表示されます。

Page1 を Slot#1 に切り替えます。
Page2 を Slot#1 に切り替えます。
次に BFFEh に 3Fh を書き込んでから、
4000h の値を読み出して反転して書き込んで、
すぐに読み出して、書いた値と同じであるか確認します。
次に、1秒後に同じアドレスを読み出してから、書いた値と同じであるか確認します。

書き込めた場合は、Success.
書き込めなかった場合は、Error.

と表示します。
