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
	//		2: X8-X4 �� ROW�A�h���X���w�肳���AY0��DIR(HB-F500)
	//
	#define MSX_KEYMATRIX_ROW_TYPE 1

	//	MSX_KEYMATRIX_INV
	//		0: Y11-Y0 �͐��_���ł���
	//		1: Y11-Y0 �͕��_���ł���
	//
	#define MSX_KEYMATRIX_INV 1

	��PPI(i8255)�� RegisterC (Y3-Y0) �����̂܂ܐڑ�����ꍇ�́AMSX_KEYMATRIX_INV �� 0 �ɂ��Ă��������B

	//	MSX_KEYMATRIX_ROW_PULL_UP
	//		0: Y0-Y11 �̓��̓s���� ���� PULL UP/DOWN �͎g��Ȃ�
	//		1: Y0-Y11 �̓��̓s���� ���� PULL UP ���g��
	//		2: Y0-Y11 �̓��̓s���� ���� PULL DOWN ���g��
	//
	#define MSX_KEYMATRIX_ROW_PULL_UP 1

	//	MSX_X_HIZ_SEL
	//		0: X�o�͂� H or L �ł���
	//		1: X�o�͂� HiZ or L �ł���
	//
	#define MSX_X_HIZ_SEL 1

5. ���ӓ_
	Raspberry Pi Pico �� I/O�d���� 3.3V �ł��B
	����� MSX �� 5V �ł��B
	Pico �� MSX �̐ڑ��́A�����Ă�������OK���Ǝv���܂����A�K�v�ɉ�����
	���x���ϊ�IC�����o�R���Đڑ����ĉ������B
	MSX �� Pico �̐ڑ��́A�K�v�ɉ����� 10k�����x�̒�R�������Őڑ����邩�A
	���x���ϊ�IC�����o�R���Đڑ����ĉ������B

	Y�M�����f�R�[�h����Ă���^�C�v�̏ꍇ�AL �� HiZ ��2�l���̂�P�[�X��������
	�v���܂��B���̏ꍇ�A�{�̂� X�M�����󂯂镔���� PullUp ����Ă���͂��ł��B
	���̂悤�ȍ\���̖{�̂ŁAPico ���� X�M���� H ���o���Ă��܂��ƁAY�M���ɂ��
	�I������Ă���s�ɏ�������{�̑��̃L�[�������ꂽ�ꍇ�ɋ�������������NG�ł��B
	���̂悤�ȍ\���̖{�̂ŗ��p����ꍇ�́AMSX_X_HIZ_SEL �� 1 �ɂ��Ă��������B
	X �� L �� HiZ �ɂȂ�悤�ɐݒ肳��܂��B

6. ����
	MSX�̃n�[�h�I�ɂ́APPI(i8255) �� Register C �� bit3�`bit0 �Ɂu�s�v��
	�w�肷��ƁAPPI �̏o�̓|�[�g���炻�̒l�����̂܂܏o�Ă��܂��B
	PPI�Ƃ��ẮA�������܂ꂽ�l�������o�͂��邾���̔ėpI/O�ł����A
	���� bit3�`bit0 �� YA�`YD �ȂǂƌĂсA������f�R�[�h���� Y0�`Y10��
	�M�������o���܂��B
	Panasonic��MSX2+�ȍ~�ł́A�������[�v���\�t�g�p��[���s][���]�L�[��
	���ڂ��Ă��āA���̃L�[�������s�� 12 (Y11) �ƂȂ��Ă��܂��B���̂��߁A
	����ȊO�̋@��ł́AY11 �͎g���܂���B

	MSX_KEYMATRIX_ROW_TYPE �́A�f�R�[�h���ꂽ Y0�`Y10(Y11) �� Pico �Ɍq��
	�ꍇ�� 1 ���w�肵�܂��B�f�R�[�h�O�� YA�`YD �� Pico �Ɍq���ꍇ�ɂ́A
	0 �܂��� 2 ���w�肷�邱�ƂɂȂ�܂��B

	��MSX_KEYMATRIX_ROW_TYPE 0
	Panasonic FS-A1F ���� S1985 ���� YA�`YD �M�������o���� Pico �Ɍq����
	�ꍇ�́A���̃^�C�v��I�����܂��B
	YA�`YD ���f�R�[�h���� IC ���O�t������Ă���A���̏o�͂� Y0�`Y10 �ɂȂ���
	����Ǝv���܂����A��������g���ꍇ�� MSX_KEYMATRIX_ROW_TYPE 1 �ɂȂ�܂��B
	�ڑ�����ꏊ�ɂ���āA�^�C�v���قȂ�̂ł����Ӊ������B

	��MSX_KEYMATRIX_ROW_TYPE 1
	Panasonic FS-A1GT ���̃L�[�{�[�h�[�q�ɂ̓f�R�[�h���ꂽ Y0�`Y11 ��
	���Ă��邽�߁A�����Ɍq����ꍇ�� 1 ���w�肵�܂��B
	�L�[�{�[�h���̃t�B������̔z���̓s�����Ǝv���܂����A�����M��������
	���Ă���̂ŁA�������q����K�v������Ǝv���܂��B

	A1GT�̏ꍇ�A�L�[�{�[�h�[�q�i�}�U�[�{�[�h����2����ŕt���Ă��鍕��
	�R�l�N�^�B�L�[�{�[�h���j�b�g�̃t�B�����P�[�u�����ڑ������ꏊ�B�j
	�́ACN11�ECN12 �ƂȂ��Ă���A���L�̂悤�ȃA�T�C���ɂȂ��Ă��܂��B

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

		��Y61,Y62,Y63 �� Y6 �ƌq����悤�ł��B

	�ȉ����m�F�̐���ł����A�����炭 Y �͑I������Ă���s�ɑΉ�����M������
	L �ɗ����āA����ȊO�� HiZ �ɂȂ��Ă���Ǝv���܂��B
	�i�L�[�{�[�h���̓t�B������ŃX�C�b�`�����t���Ă��炸�AY�M���� MSX�G���W��
	DAT9769C �ɂ��̂܂܌q�����Ă��邽�߂ł��B�j
	���̂��߁AMSX_KEYMATRIX_ROW_PULL_UP �� 1 �ɂ��Ă����K�v������Ǝv���܂��B

	��MSX_KEYMATRIX_ROW_TYPE 2
	SONY �� HB-F500�n�́A�L�[�{�[�h�����^�ł����A�L�[�{�[�h�R�l�N�^�ɂ�
	YA�`YD ���o�Ă��Ă���悤�ł��B�������AX8�`X4 �Ƌ��ʂ̔z���ɂȂ���
	����ADIR�M���� YA�`YD ��MSX���o���Ă���^�C�~���O�Ȃ̂��AX8�`X4 ��
	�ǂݎ���Ă���^�C�~���O�Ȃ̂���I������悤�ł��BX��Y��bit�̕��т�
	�t�]���Ă���̂ŁAPico���Ńr�b�g���є��]���������{����K�v������܂��B
	���� HB-F500�n�̏ꍇ�ɂ́A2 ���w�肷�邱�ƂɂȂ�܂��B
	Pico���̃|�[�g�́AX0�`X7, Y0 ���g���܂��BPico X0�`X7 �ɁAMSX X1�`X8 
	��ڑ��BPico X4�`X8 �� MSX YD�`YA ��ڑ��BPico Y0 �� MSX DIR ��ڑ�
	�ƂȂ�܂��BY1�`Y11 �͎g���܂���B


	MSX�� BIOS �́A59.94Hz (NTSC��)�œ����Ă��銄�荞�݂̃^�C�~���O��
	2��̂���1�񂾂��A�L�[�X�L�������[�`��������悤�ɂȂ��Ă���A
	���̒��őS�s�X�L�������Ă��܂��B
	�]���āAPPI �� YA�`YD ��A���̃f�R�[�h���ʂł��� Y0�`Y10(Y11) ��
	�� 1/30�b �Ɉ�x�A0�`10(11) �ɘA���I�ɕω�����^�C�~���O������悤��
	�Ȃ��Ă��܂��B
	MSX �́APPI Register C �� bit3�`bit0 �� YA�`YD (�s) ���w�肵�āA
	PPI Register B ���� X7�`X0 ��ǂݎ��܂��B
	1/30�b�� 1����s�����R�[�h�͉��L�̂悤�ɂȂ��Ă���Ǝv���܂��B

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

	OUT �� Y ���o�Ă�����ɁAIN �œǂݏo���܂ł̊Ԃ� X�M�����w�肳�ꂽY��
	��Â����l�Ɋm�肷��K�v������̂ŁAPico �͂��̃^�C�~���O�͖Z��������
	�K�v������܂��B
	�������Ԃɍ���Ȃ��ƓK�؂ȃL�[�����𓾂��܂���B

7. ���ӓ_ ����2
	2021�N8��16�����݁A�o����Ă��� Raspberry Pi Pico �ɂ͎኱�̕s�������A
	���̉e���ł��܂��F���ł��Ȃ��L�[�{�[�h��A�Q��ڂ̐ڑ��ŔF�����Ȃ��Ȃ�L�[�{�[�h
	�����݂���悤�ł��B
	���ہA���̏��L���閳���L�[�{�[�h�͔F�����܂��񂵁A�F������L�[�{�[�h�ł�����������
	�J��Ԃ��Ă���� TinyUSB ���p�j�b�N���N�����Ē�~����̂��m�F���Ă��܂��B

	FS-A1ST/A1GT ���܂� MSX-ENGINE2�n�̏ꍇ�ARaspberry Pi Pico �ɓ����� PullUp��R�ł�
	�Ή�������Ȃ����Ƃ�����悤�ł��B
	�����Ŕz�z���Ă���̂̓\�t�g�E�F�A�݂̂Ȃ̂ŁA�d�C�I�ȕ����Ɋւ��Ă͏\���ɍl���̂���
	�����p�������B

-------------------------------------------------------------------------------
2021�N5��12�� ver1.0 HRA!
	���ŁB

2021�N6��12�� ver1.1 HRA!
	TinyUSB �� API�ύX�ɍ��킹�ďC���B

2021�N8��16�� HRA!
	���ӏ��� 7 ��ǉ�
