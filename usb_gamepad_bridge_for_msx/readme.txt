USB Gamepad bridge for MSX
-------------------------------------------------------------------------------

1. �Ȃɂ���H
	Raspberry Pi Pico ���g���� USB�Q�[���p�b�h�� MSX �̃W���C�X�e�B�b�N�|�[�g
	�ɐڑ����Ďg����悤�ɂ��邽�߂́A�uRaspberry Pi Pico��œ��삷��v���O�����v
	�ł��B
	�I�}�P�ŁAMegaDrive�����Ή�������Ă���܂����AMegaDrive�����L���Ă��Ȃ�����
	������͓��얢�m�F�ł��B

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
	add_subdirectory(usb_gamepad_bridge_for_msx)

	�����ꏊ�Ƀf�B���N�g�� usb_gamepad_bridge_for_msx ���쐬���A
	�����ɂ���ꎮ�iusb_gamepad_bridge_for_msx.c ���j���A���̃f�B���N�g���փR�s�[
	���܂��B

	pico-sdk/lib �̒��ɁA�����ɒu���Ă��� TinyUSB/pico-sdk/lib �̒��g���㏑���R�s�[���܂��B
	����ɂ��ATinyUSB �� gamepad�T�|�[�g���ǉ�����܂��B
	�i���̑��̃��C�u�������o�[�W�����A�b�v���Č݊����������Ă��܂����ꍇ�ɁA
	����ɋ@�\���Ȃ��Ȃ邩������܂���B�j

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
