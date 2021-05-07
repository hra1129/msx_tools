if exist makeup_encode_table.exe (
	del makeup_encode_table.exe
)
cl makeup_encode_table.c /makeup_encode_table.exe
if exist makeup_encode_table.exe (
	makeup_encode_table.exe > encode.h
)
pause
