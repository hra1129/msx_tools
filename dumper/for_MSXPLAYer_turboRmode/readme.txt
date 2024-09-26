MSXPLAYer の turboRモード から BIOS等の ROMイメージをダンプするツールです。
ダンプして得られた ROMイメージは、その MSXPLAYer の所有者のみ使用することが出来ます。
MSXPLAYer のメディア等 を手放した時点で使用することは出来なくなります。
ダンプした BIOS を、第三者へ配布・コピーすることは禁じられています。
取り扱いにご注意下さい。


OpenMSX にインストールすることで、OpenMSX で MSX-BASIC を使うことが出来
るようになります。やり方を下記にまとめます。

OpenMSX のインストールフォルダの

share/machines に MSXturboR.xml
share/extensions に MegaFlashROM_SCC+_SD.xml

を入れて下さい。

下記のサイトにある MegaFlashROM SCC+SD の ROMイメージをダウンロードして下さい。
https://www.konamiman.com/msx/msx-e.html#nextor

	Normal version, intended to be burned with the OPFXSD tool.
		For MegaFlashROM with 1 SD card slot ←これをクリックしてダウンロード。

ダウンロードした zip を解凍して出てくる下記のファイルを、share/extensions にコピーして下さい。
		Nextor-2.1.0.MegaFlashSDSCC.1-slot.ROM
		Nextor-2.1.0.MegaFlashSDSCC.1-slot.Recovery.ROM

openMSX catapult から、File→Test MSX Hardware をクリックしてしばらく待つと、MSX type に MSXturboR_MSXPLAYER が
現れます。Extensions に MegaFlashROM SCC+_SD も現れます。
これらを選択した上で、Hard Disk のところで DSK イメージを選択すれば、ディスクイメージにアクセス出来ます。

MSXDOS2.SYS/COMMAND2.COM を入れておけば、DOS2(のような NextorDOS)が起動します。
