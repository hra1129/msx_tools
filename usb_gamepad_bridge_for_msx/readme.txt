USB Gamepad bridge for MSX
-------------------------------------------------------------------------------

1. �Ȃɂ���H
	Raspberry Pi Pico ���g���� USB�Q�[���p�b�h/USB�}�E�X�� MSX �̃W���C�X�e�B�b�N
	�|�[�g�ɐڑ����Ďg����悤�ɂ��邽�߂́A�uRaspberry Pi Pico��œ��삷��
	�v���O�����v�ł��B
	�I�}�P�ŁAMegaDrive�����Ή�������Ă���܂����AMegaDrive�����L���Ă��Ȃ�����
	������͓��얢�m�F�ł��BMegaDrive�����́Adefine�ݒ��ς��ăR���p�C��������
	�K�v������܂��B�܂��AMegaDrive�Ή��̓Q�[���p�b�h�݂̂ł��B

2. �Ɛӎ���
	���ۂ�1�쐬���āAMSX���@ (FS-A1GT) �ƁAFPGA�݊��@ (8bits4ever SX-2 ����@)
	�ɂ� Joymega��Ή��Q�[������Ȃ�����ł��Ajoymega����m�F�c�[���ŕ����L�[��
	8�{�^���F������̂��m�F�ς݂ł����A�ȉ���2�̃Q�[���p�b�h�����m�F���Ă��܂���B

	�Q�[���p�b�h
		BUFFALO BSGP815GY : �X�[�t�@�~�^�C�v�B�A�ˋ@�\�t���B���p�ł��܂����B
		ELECOM JC-U1608TBK: �X�[�t�@�~�^�C�v�B�A�ˋ@�\�t���B���p�ł��܂����B

	�}�E�X
		HP USB���w�}�E�X (�^�ԕs��, HP���p�\�R���̕t���i)
		ELECOM M-BL27UB

	�܂��A���L�͔F������Ȃ����Ƃ��m�F�ς݂ł��B

	�Q�[���p�b�h
		SEGA ���K�h���~�j�t���R���g���[���[: ���p�ł��܂���ł����B

	�܂��A���L�͔F���������̂́APico �� USB�h���C�o�� PANIC ���������Ē�~
	�i�n���O�A�b�v�j���邱�Ƃ��m�F�ς݂ł��B

	�}�E�X
		Easterntimes Tech 2.4G Wireless Optical Mouse D-09

	���̃Q�[���p�b�h�E�W���C�X�e�B�b�N�E�}�E�X���g���邩�ǂ����͕s���ł��B
	�܂��A�z���~�X�ɂ���Ă� Rasberry Pi Pico ��AMSX/MegaDrive�{�̂��󂷋��ꂪ
	����܂��B�����p�̍ۂɂ́A�e���̐ӔC�ł����p�������B
	MegaDrive �Ɏ����ẮA��ؓ���m�F�����Ă��Ȃ����߁A�z���}�̕������������s��
	�ł��B
	����𗘗p���邱�Ƃɂ���āA���炩�̕s������������Ƃ��Ă��A
	HRA! (t.hara) �͈�ؐӔC�𕉂��܂���̂ł������������B

	�}�E�X���q�����ꍇ�A���{�^���i�z�C�[���{�^���j�������Ɠ��샂�[�h���؂�ւ��܂��B

		������ԁF�m�[�}������
		1�񉟂� �F�����������������̑��x (SCREEN7�p)
		2�񉟂� �F�����E�����Ƃ��ɔ����̑��x�i��������}�E�X�p�j
		3�񉟂� �F�����͔����A������ 1/4 �̑��x�i��������}�E�X�� SCREEN7�p�j
		4�񉟂� �F������Ԃ֖߂�

	LED�̓_�Ńp�^�[���œ��샂�[�h�������܂��B
		1��������_��       �F ������ԁA�Q�[���p�b�h���[�h�B
		2��_��               �F �}�E�X�i������ԁj���[�h�B
		3��_��               �F �}�E�X�i1�񉟂��j���[�h�B
		4��_��               �F �}�E�X�i2�񉟂��j���[�h�B
		1�񒷂��A���̌�2��_�ŁF �}�E�X�i3�񉟂��j���[�h�B

	�Q�[���p�b�h��}�E�X�𔲂��������Ă���ƁAPico��USB�h���C�o�� PANIC �ɂȂ�
	�n���O�A�b�v����ꍇ������܂��B�n���O�A�b�v����ƁALED �͏������܂��B
	���̏ꍇ�AMSX�����x�O���đ}�������Ă��������B

3. �r���h
	�{�^���̔z�u��ς�����AMegaDrive�����ɕύX�����肷��ꍇ�́A�R���p�C��������
	�K�v������܂��B

	Raspberry Pi �� Sample�v���O����(���L)���r���h�ł����Ԃ܂Ŋ��𐮂��ĉ������B
	https://github.com/raspberrypi/pico-examples

	���[�J���� clone ���� pico-examples �̒��� CMakeLists.txt �ɉ��L��ǋL���܂��B
	add_subdirectory(usb_gamepad_bridge_for_msx)

	�����ꏊ�Ƀf�B���N�g�� usb_gamepad_bridge_for_msx ���쐬���A
	�����ɂ���ꎮ�iusb_gamepad_bridge_for_msx.c ���j���A���̃f�B���N�g���փR�s�[
	���܂��B

	�f�B���N�g�� build �ɓ���Amake �܂��� nmake �� rebuild_cache ���܂��B
	> nmake rebuild_cache

	���Ƃ́A���ʂɃr���h���܂��B
	> nmake usb_gamepad_bridge_for_msx

	build/usb_gamepad_bridge_for_msx �� usb_gamepad_bridge_for_msx.uf2 ��
	�o���オ��̂ŁAPico �̃{�^���������Ȃ��� USB�P�[�u����ڑ����āA
	�o�Ă���X�g���[�W�� usb_gamepad_bridge_for_msx.uf2 ���R�s�[����ƁA
	�X�g���[�W�������I�ɐؒf����܂��B����ŏ������݊����B

	Pico �� USB�[�q�� OTG�P�[�u����ڑ����A������ USB�Q�[���p�b�h��ڑ����܂��B
	usb_gamepad_bridge_for_msx.pdf �Ɏ������s���A�T�C���ɂȂ��Ă���̂ŁA
	�K�v�ȂƂ���ɐڑ����Ă����p�������B

4. �J�X�^�}�C�Y
	�\�[�X�t�@�C�� usb_gamepad_bridge_for_msx.c �̏�̕��ɐݒ肪�����Ă���܂��B
	�K�v�ɉ����ď����ւ��Ă����p�������B

5. ���ӓ_
	Raspberry Pi Pico �� I/O�d���� 3.3V �ł��B
	����� MSX �� 5V �ł��B
	MSX �� Pico �̐ڑ��́A10k�����x�̒�R�������Őڑ����ĉ������B
	Pico �� MSX �̐ڑ��́A������OK�ł��B

------------------------------------------------------------------------------
2021�N5��8�� ver1.0 HRA!
	TinyUSB �� GAMEPAD/JOYSTICK �ɂ܂��Ή����Ă��Ȃ����߁ATinyUSB���̂��C������
	�����ɑΉ��B�莝���̃Q�[���p�b�h (JOYSTICK�Ƃ��ĔF��) �̂ݓ���m�F�B

2021�N6��11�� ver1.1 HRA!
	TinyUSB �� GAMEPAD�Ή��������Ƃ��@�ɁA�Ǝ��C����r���B
	TinyUSB �� API ���啝�ɕύX�ɂȂ��Ă������߁A����ɍ��킹�ďC���B
	���ς�炸 JOYSTICK �̕��̑Ή��͂܂��Ȃ̂ŁAmy_hid_joystick_report_t ���`
	���đΉ��Bver1.0 �Ɠ��l�ɓ��삷�邱�Ƃ��m�F�B
	GAMEPAD�Ƃ��ĔF������R���g���[���[�ɂ��Ή����Ă�����肾���A���̂悤��
	�Q�[���p�b�h�����L���Ă��Ȃ����߁A���얢�m�F�B

2021�N7��25�� ver2.0 HRA!
	TinyUSB �̃Q�[���p�b�h�̃{�^����`�����ύX�ɂȂ��Ă����̂ɑΉ��B
	�}�E�X�Ή���ǉ��B
