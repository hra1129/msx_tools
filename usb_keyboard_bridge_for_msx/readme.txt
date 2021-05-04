USB Keybard bridge for MSX
-------------------------------------------------------------------------------

1. �Ȃɂ���H
	Raspberry Pi Pico ���g���� USB�L�[�{�[�h�̃L�[��Ԃ� MSX �̃L�[�}�g���N�X�M��
	�ɕϊ����邽�߂́A�uRaspberry Pi Pico��œ��삷��v���O�����v�ł��B

2. �Ɛӎ���
	Pico�P�̂ŊȒP�ȃe�X�g�͍s���Ă��܂����A��������ۂ� MSX �Ɍq���āA
	MSX����� Y�M���w�肩��� X�M���������\���Ԃɍ������x�Ȃ̂��ǂ����͖��m�F�ł��B
	�����p�̍ۂɂ́A�e���̐ӔC�ł����p�������B
	����𗘗p���邱�Ƃɂ���āA���炩�̕s������������Ƃ��Ă��A
	HRA! (t.hara) �͈�ؐӔC�𕉂��܂���̂ł������������B

3. �r���h
	Raspberry Pi �� Sample�v���O����(���L)���r���h�ł����Ԃ܂Ŋ��𐮂��ĉ������B
	https://github.com/raspberrypi/pico-examples

	���[�J���� clone ���� pico-examples �̒��� CMakeLists.txt �ɉ��L��ǋL���܂��B
	add_subdirectory(usb_keyboard_bridge_for_msx)

	�����ꏊ�Ƀf�B���N�g�� usb_keyboard_bridge_for_msx ���쐬���A
	�����ɂ���ꎮ�iusb_keyboard_bridge_for_msx.c ���j���A���̃f�B���N�g���փR�s�[
	���܂��B

	�f�B���N�g�� build �ɓ���Amake �܂��� nmake �� rebuild_cache ���܂��B
	> nmake rebuild_cache

	���Ƃ́A���ʂɃr���h���܂��B
	> nmake usb_keyboard_bridge_for_msx

	build/usb_keyboard_bridge_for_msx �� usb_keyboard_bridge_for_msx.uf2 ��
	�o���オ��̂ŁAPico �̃{�^���������Ȃ��� USB�P�[�u����ڑ����āA
	�o�Ă���X�g���[�W�� usb_keyboard_bridge_for_msx.uf2 ���R�s�[����ƁA
	�X�g���[�W�������I�ɐؒf����܂��B����ŏ������݊����B

	Pico �� USB�[�q�� OTG�P�[�u����ڑ����A������ USB�L�[�{�[�h��ڑ����܂��B
	usb_keyboard_bridge_for_msx.pdf �Ɏ������s���A�T�C���ɂȂ��Ă���̂ŁA
	�K�v�ȂƂ���ɐڑ����Ă����p�������B

4. �J�X�^�}�C�Y
	�\�[�X�t�@�C�� usb_keyboard_bridge_for_msx.c �̏�̕��ɐݒ肪�����Ă���܂��B
	�K�v�ɉ����ď����ւ��Ă����p�������B

	// --------------------------------------------------------------------
	//	MSX_KEYMATRIX_ROW_TYPE
	//		0: Y3-Y0  �� ROW�A�h���X���w�肳���
	//		1: Y11-Y0 �� ROW�A�h���X���f�R�[�h�������ʂ��w�肳��� (ONE HOT)
	//
	#define MSX_KEYMATRIX_ROW_TYPE 1

	//	MSX_KEYMATRIX_INV
	//		0: Y11-Y0 �͐��_���ł���
	//		1: Y11-Y0 �͕��_���ł���
	//
	#define MSX_KEYMATRIX_INV 1

	//	MSX_KEYMATRIX_ROW_PULL_UP
	//		0: Y0-Y11 �̓��̓s���� ���� PULL UP/DOWN �͎g��Ȃ�
	//		1: Y0-Y11 �̓��̓s���� ���� PULL UP ���g��
	//		2: Y0-Y11 �̓��̓s���� ���� PULL DOWN ���g��
	//
	#define MSX_KEYMATRIX_ROW_PULL_UP 1

5. ���ӓ_
	Raspberry Pi Pico �� I/O�d���� 3.3V �ł��B
	����� MSX �� 5V �ł��B
	MSX �� Pico �̐ڑ��́A10k�����x�̒�R�������Őڑ����ĉ������B
	Pico �� MSX �̐ڑ��́A������OK�ł��B
