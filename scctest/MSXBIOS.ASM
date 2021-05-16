; ==============================================================================
;	MSX BIOS
;
;  Copyright (C) 2020 Takayuki Hara (HRA!)
;  All rights reserved.
;                           http://hraroom.s602.xrea.com/msx/software/mgsp.html
;
;  �{�\�t�g�E�F�A����і{�\�t�g�E�F�A�Ɋ�Â��č쐬���ꂽ�h�����́A�ȉ��̏�����
;  �������ꍇ�Ɍ���A�ĔЕz����юg�p��������܂��B
;
;  1.�\�[�X�R�[�h�`���ōĔЕz����ꍇ�A��L�̒��쌠�\���A�{�����ꗗ�A����щ��L
;    �Ɛӏ��������̂܂܂̌`�ŕێ����邱�ƁB
;  2.�o�C�i���`���ōĔЕz����ꍇ�A�Еz���ɕt���̃h�L�������g���̎����ɁA��L��
;    ���쌠�\���A�{�����ꗗ�A����щ��L�Ɛӏ������܂߂邱�ƁB
;  3.���ʂɂ�鎖�O�̋��Ȃ��ɁA�{�\�t�g�E�F�A��̔��A����я��ƓI�Ȑ��i�⊈��
;    �Ɏg�p���Ȃ����ƁB
;
;  �{�\�t�g�E�F�A�́A���쌠�҂ɂ���āu����̂܂܁v�񋟂���Ă��܂��B���쌠�҂́A
;  ����ړI�ւ̓K�����̕ۏ؁A���i���̕ۏ؁A�܂�����Ɍ��肳��Ȃ��A�����Ȃ閾��
;  �I�������͈ÖقȕۏؐӔC�������܂���B���쌠�҂́A���R�̂�������킸�A���Q
;  �����̌�����������킸�A���ӔC�̍������_��ł��邩���i�ӔC�ł��邩�i�ߎ�
;  ���̑��́j�s�@�s�ׂł��邩���킸�A���ɂ��̂悤�ȑ��Q����������\����m��
;  ����Ă����Ƃ��Ă��A�{�\�t�g�E�F�A�̎g�p�ɂ���Ĕ��������i��֕i�܂��͑�p�T
;  �[�r�X�̒��B�A�g�p�̑r���A�f�[�^�̑r���A���v�̑r���A�Ɩ��̒��f���܂߁A�܂���
;  ��Ɍ��肳��Ȃ��j���ڑ��Q�A�Ԑڑ��Q�A�����I�ȑ��Q�A���ʑ��Q�A�����I���Q�A��
;  ���͌��ʑ��Q�ɂ��āA��ؐӔC�𕉂�Ȃ����̂Ƃ��܂��B
;
;  Note that above Japanese version license is the formal document.
;  The following translation is only for reference.
;
;  Redistribution and use of this software or any derivative works,
;  are permitted provided that the following conditions are met:
;
;  1. Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;  2. Redistributions in binary form must reproduce the above
;     copyright notice, this list of conditions and the following
;     disclaimer in the documentation and/or other materials
;     provided with the distribution.
;  3. Redistributions may not be sold, nor may they be used in a
;     commercial product or activity without specific prior written
;     permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
;  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
;  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
;  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
;  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
;  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
;  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
;  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;  POSSIBILITY OF SUCH DAMAGE.
; ------------------------------------------------------------------------------
;	Date		Author	Ver		Description
;	2020/9/22	t.hara	1.0		1st release
; ==============================================================================

chkram		:=			0x0000
synchr		:=			0x0008
rdslt		:=			0x000C
chrgtr		:=			0x0010
wrslt		:=			0x0014
outdo		:=			0x0018
calslt		:=			0x001c
dcompr		:=			0x0020
enaslt		:=			0x0024
getypr		:=			0x0028
callf		:=			0x0030
keyint		:=			0x0038
initio		:=			0x003b
inifnk		:=			0x003e
disscr		:=			0x0041
enascr		:=			0x0044
wrtvdp		:=			0x0047
rdvrm		:=			0x004a
wrtvrm		:=			0x004d
setrd		:=			0x0050
setwrt		:=			0x0053
filvrm		:=			0x0056
ldirmv		:=			0x0059
ldirvm		:=			0x005c
chgmod		:=			0x005f
chgclr		:=			0x0062
nmi			:=			0x0066
clrspr		:=			0x0069
initxt		:=			0x006c
init32		:=			0x006f
inigrp		:=			0x0072
inimlt		:=			0x0075
settxt		:=			0x0078
sett32		:=			0x007b
setgrp		:=			0x007e
setmlt		:=			0x0081
calpat		:=			0x0084
calatr		:=			0x0087
gspsiz		:=			0x008a
grpprt		:=			0x008d
gicini		:=			0x0090
wrtpsg		:=			0x0093
rdpsg		:=			0x0096
strtms		:=			0x0099
chsns		:=			0x009c
chget		:=			0x009f
chput		:=			0x00a2
lptout		:=			0x00a5
lptstt		:=			0x00a8
cnvchr		:=			0x00ab
pinlin		:=			0x00ae
inlin		:=			0x00b1
qinlin		:=			0x00b4
breakx		:=			0x00b7
beep		:=			0x00c0
cls			:=			0x00c3
posit		:=			0x00c6
fnksb		:=			0x00c9
erafnk		:=			0x00cc
dspfnk		:=			0x00cf
totext		:=			0x00d2
gtstck		:=			0x00d5
gttrig		:=			0x00d8
gtpad		:=			0x00db		; note: MSXturboR�ł̓��C�g�y��(A=8�`11)�͏��0���Ԃ�
gtpdl		:=			0x00de		; note: MSXturboR�ł͏��0���Ԃ�
tapion		:=			0x00e1		; note: MSXturboR�ł͏��Cy=1(�G���[)���Ԃ�
tapin		:=			0x00e4		; note: MSXturboR�ł͏��Cy=1(�G���[)���Ԃ�
tapiof		:=			0x00e7		; note: MSXturboR�ł͏��Cy=1(�G���[)���Ԃ�
tapoon		:=			0x00ea		; note: MSXturboR�ł͏��Cy=1(�G���[)���Ԃ�
tapout		:=			0x00ed		; note: MSXturboR�ł͏��Cy=1(�G���[)���Ԃ�
tapoof		:=			0x00f0		; note: MSXturboR�ł͏��Cy=1(�G���[)���Ԃ�
stmotr		:=			0x00f3		; note: MSXturboR�ł͉��������ɕԂ�
chgcap		:=			0x0132
chgsnd		:=			0x0135
rslreg		:=			0x0138
wslreg		:=			0x013b
rdvdp		:=			0x013e
snsmat		:=			0x0141
isflio		:=			0x014a
outdlp		:=			0x014d
kilbuf		:=			0x0156
calbas		:=			0x0159

; require MSX2
subrom		:=			0x015c
extrom		:=			0x015f
eol			:=			0x0168
bigfil		:=			0x016b
nsetrd		:=			0x016e
nstwrt		:=			0x0171
nrdvrm		:=			0x0174
nwrvrm		:=			0x0177
rdres		:=			0x017A
wrres		:=			0x017D

; require MSXturboR
chgcpu		:=			0x0180
getcpu		:=			0x0183
pcmply		:=			0x0186
pcmrec		:=			0x0189

; SUB-ROM (require MSX2)
sub_gtpprt	:=			0x0089
sub_nvbxln	:=			0x00c9
sub_nvbxfl	:=			0x00cd
sub_chgmod	:=			0x00d1
sub_initxt	:=			0x00d5
sub_init32	:=			0x00d9
sub_inigrp	:=			0x00dd
sub_inimlt	:=			0x00e1
sub_settxt	:=			0x00e5
sub_sett32	:=			0x00e9
sub_setgrp	:=			0x00ed
sub_setmlt	:=			0x00f1
sub_clrspr	:=			0x00f5
sub_calpat	:=			0x00f9
sub_calatr	:=			0x00fd
sub_gspsiz	:=			0x0101
sub_getpat	:=			0x0105
sub_wrtvrm	:=			0x0109
sub_rdvrm	:=			0x010d
sub_chgclr	:=			0x0111
sub_clssub	:=			0x0115
sub_dspfnk	:=			0x011d
sub_wrtvdp	:=			0x012d
sub_vdpsta	:=			0x0131
sub_setpag	:=			0x013d
sub_iniplt	:=			0x0141
sub_rstplt	:=			0x0145
sub_getplt	:=			0x0149
sub_setplt	:=			0x014d
sub_beep	:=			0x017d
sub_prompt	:=			0x0181
sub_newpad	:=			0x01ad		; note: MSXturboR�ł̓��C�g�y��(A=8�`11)�͏��0���Ԃ�
sub_chgmdp	:=			0x01b5
sub_knjprt	:=			0x01bd
sub_redclk	:=			0x01f5
sub_wrtclk	:=			0x01f9

h_prompt	:=			0xF24F		; hook: Disk change prompt (Require DISK DRIVE)

diskve		:=			0xF323		; 2bytes (Require DISK DRIVE)
breakv		:=			0xF325		; 2bytes (Require DISK DRIVE)
ramad0		:=			0xF341		; 1byte, Page0 RAM Slot (Require DISK DRIVE)
ramad1		:=			0xF342		; 1byte, Page1 RAM Slot (Require DISK DRIVE)
ramad2		:=			0xF343		; 1byte, Page2 RAM Slot (Require DISK DRIVE)
ramad3		:=			0xF344		; 1byte, Page3 RAM Slot (Require DISK DRIVE)
masters		:=			0xF348		; 1byte, MasterCartridgeSlot (Require DISK DRIVE)

rdprim		:=			0xF380		; 5bytes, ��{�X���b�g����̓ǂݍ���
wrprim		:=			0xF385		; 7bytes, ��{�X���b�g�ւ̏�������
clprim		:=			0xF38C		; 14bytes, ��{�X���b�g�R�[��

cliksw		:=			0xF3DB		; 1byte, �L�[�N���b�N�X�C�b�` (0=OFF, others=ON)
csry		:=			0xF3DC		; 1byte, �J�[�\����Y���W
csrx		:=			0xF3DD		; 1byte, �J�[�\����X���W
cnsdfg		:=			0xF3DE		; 1byte, �t�@���N�V�����L�[�\���X�C�b�` (0=ON, others=OFF)

rg0sav		:=			0xF3DF		; 1byte, VDP R#0 �ɏ������񂾒l
rg1sav		:=			0xF3E0		; 1byte, VDP R#1 �ɏ������񂾒l
rg2sav		:=			0xF3E1		; 1byte, VDP R#2 �ɏ������񂾒l
rg3sav		:=			0xF3E2		; 1byte, VDP R#3 �ɏ������񂾒l
rg4sav		:=			0xF3E3		; 1byte, VDP R#4 �ɏ������񂾒l
rg5sav		:=			0xF3E4		; 1byte, VDP R#5 �ɏ������񂾒l
rg6sav		:=			0xF3E5		; 1byte, VDP R#6 �ɏ������񂾒l
rg7sav		:=			0xF3E6		; 1byte, VDP R#7 �ɏ������񂾒l
statfl		:=			0xF3E7		; 1byte, VDP S#0 ����ǂݍ��񂾒l

fnkstr		:=			0xF87F		; 16byte * 10, �t�@���N�V�����L�[�ɑΉ����镶����

exbrsa		:=			0xFAF8		; 1byte, SUB-ROM �̃X���b�g�ԍ�

hokvld		:=			0xFB20

oldkey		:=			0xFBDA		; 11bytes, �L�[�}�g���N�X�̏��(��)
newkey		:=			0xFBE5		; 11bytes, �L�[�}�g���N�X�̏��(�V)
keybuf		:=			0xFBF0		; 40bytes, �L�[�o�b�t�@
linwrk		:=			0xFC18		; 40bytes, �X�N���[���n���h�����g���ꎞ�ۊǏꏊ
patwrk		:=			0xFC40		; 8bytes, �p�^�[���R���o�[�^���g���ꎞ�ۊǏꏊ
bottom		:=			0xFC48		; 2bytes, ��������RAM�̐擪�Ԓn�BMSX2�ł͒ʏ� 8000h
himem		:=			0xFC4A		; 2bytes, ���p�\�ȃ������̏�ʔԒn�BCLEAR����<���������>�ɂ��ݒ肳���
trptbl		:=			0xFC4C		; 78bytes, ���荞�ݏ����Ŏg���g���b�v�e�[�u���B��̃e�[�u���� 3bytes �� 1�o�C�g�ڂ� ON/OFF/STOP�̏�ԁB�c�肪�����e�L�X�g�A�h���X

scrmod		:=			0xFCAF		; 1byte, ���݂̃X�N���[�����[�h�ԍ�
oldscr		:=			0xFCB0		; 1byte, �X�N���[�����[�h�ۑ��G���A

exptbl		:=			0xFCC1		; 4bytes, �e�X���b�g�̊g���̗L��
slttbl		:=			0xFCC5		; 4bytes, �e�X���b�g�̊g���X���b�g���W�X�^�̌��݂̑I����
sltatr		:=			0xFCC9		; 64bytes, �e�X���b�g�p�̑���
sltwrk		:=			0xFD09		; 128bytes, �e�X���b�g�p�̓���̃��[�N�G���A���m��
procnm		:=			0xFD89		; 16bytes, �g���X�e�[�g�����g, �g���f�o�C�X�̖��O������, ASCIIZ
device		:=			0xFD99		; 1byte, �J�[�g���b�W�p�̑��u����

h_timi		:=			0xFD9F		; 5bytes, �����������荞�ݔ������̃t�b�N

extbio		:=			0xFFCA

rg8sav		:=			0xFFE7
rg9sav		:=			0xFFE8