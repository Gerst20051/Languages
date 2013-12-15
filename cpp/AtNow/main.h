#define MAX_PARAMS 10
#define MAX_PARAM_SIZE 1024
#define WIDECHAR_BUFF_SIZE 1024
#define ERROR_BUFFER_SIZE 1024
#define ATNOW_INFO	"AtNow v1.1\r\n" \
					"Copyright (c) 2002 - 2004 Nir Sofer\r\n" \
					"Web site: http://nirsoft.mirrorz.com\r\n\r\n" \
					"Syntax: \r\n" \
					"AtNow [\\\\ComputerName] [delay] [/interactive] \"Command\" [parameters]\r\n\r\n" \
					"Examples: \r\n"\
					"AtNow \\\\COMP1 \"c:\\Program Files\\MySoftware\\ms.exe\" /c /m \r\n" \
					"AtNow /interactive \"c:\\winnt\\system32\\calc.exe\"\r\n\r\n" \
					"For extended information, type AtNow /?\r\n\r\n"