if exist encode_test.exe (
	del encode_test.exe
)
cl main.c /oencode_test.exe
if exist encode_test.exe (
	encode_test.exe
)
pause
