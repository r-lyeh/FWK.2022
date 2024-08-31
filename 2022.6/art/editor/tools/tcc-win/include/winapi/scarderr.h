/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within this package.
 */
#ifndef SCARD_S_SUCCESS
#define SCARD_S_SUCCESS NO_ERROR

#define FACILITY_SYSTEM 0x0
#define FACILITY_SCARD 0x10

#define STATUS_SEVERITY_WARNING 0x2
#define STATUS_SEVERITY_INFORMATIONAL 0x1
#define STATUS_SEVERITY_ERROR 0x3

#define SCARD_F_INTERNAL_ERROR ((DWORD)0x80100001L)
#define SCARD_E_CANCELLED ((DWORD)0x80100002L)
#define SCARD_E_INVALID_HANDLE ((DWORD)0x80100003L)
#define SCARD_E_INVALID_PARAMETER ((DWORD)0x80100004L)
#define SCARD_E_INVALID_TARGET ((DWORD)0x80100005L)
#define SCARD_E_NO_MEMORY ((DWORD)0x80100006L)
#define SCARD_F_WAITED_TOO_LONG ((DWORD)0x80100007L)
#define SCARD_E_INSUFFICIENT_BUFFER ((DWORD)0x80100008L)
#define SCARD_E_UNKNOWN_READER ((DWORD)0x80100009L)
#define SCARD_E_TIMEOUT ((DWORD)0x8010000AL)
#define SCARD_E_SHARING_VIOLATION ((DWORD)0x8010000BL)
#define SCARD_E_NO_SMARTCARD ((DWORD)0x8010000CL)
#define SCARD_E_UNKNOWN_CARD ((DWORD)0x8010000DL)
#define SCARD_E_CANT_DISPOSE ((DWORD)0x8010000EL)
#define SCARD_E_PROTO_MISMATCH ((DWORD)0x8010000FL)
#define SCARD_E_NOT_READY ((DWORD)0x80100010L)
#define SCARD_E_INVALID_VALUE ((DWORD)0x80100011L)
#define SCARD_E_SYSTEM_CANCELLED ((DWORD)0x80100012L)
#define SCARD_F_COMM_ERROR ((DWORD)0x80100013L)
#define SCARD_F_UNKNOWN_ERROR ((DWORD)0x80100014L)
#define SCARD_E_INVALID_ATR ((DWORD)0x80100015L)
#define SCARD_E_NOT_TRANSACTED ((DWORD)0x80100016L)
#define SCARD_E_READER_UNAVAILABLE ((DWORD)0x80100017L)
#define SCARD_P_SHUTDOWN ((DWORD)0x80100018L)
#define SCARD_E_PCI_TOO_SMALL ((DWORD)0x80100019L)
#define SCARD_E_READER_UNSUPPORTED ((DWORD)0x8010001AL)
#define SCARD_E_DUPLICATE_READER ((DWORD)0x8010001BL)
#define SCARD_E_CARD_UNSUPPORTED ((DWORD)0x8010001CL)
#define SCARD_E_NO_SERVICE ((DWORD)0x8010001DL)
#define SCARD_E_SERVICE_STOPPED ((DWORD)0x8010001EL)
#define SCARD_E_UNEXPECTED ((DWORD)0x8010001FL)
#define SCARD_E_ICC_INSTALLATION ((DWORD)0x80100020L)
#define SCARD_E_ICC_CREATEORDER ((DWORD)0x80100021L)
#define SCARD_E_UNSUPPORTED_FEATURE ((DWORD)0x80100022L)
#define SCARD_E_DIR_NOT_FOUND ((DWORD)0x80100023L)
#define SCARD_E_FILE_NOT_FOUND ((DWORD)0x80100024L)
#define SCARD_E_NO_DIR ((DWORD)0x80100025L)
#define SCARD_E_NO_FILE ((DWORD)0x80100026L)
#define SCARD_E_NO_ACCESS ((DWORD)0x80100027L)
#define SCARD_E_WRITE_TOO_MANY ((DWORD)0x80100028L)
#define SCARD_E_BAD_SEEK ((DWORD)0x80100029L)
#define SCARD_E_INVALID_CHV ((DWORD)0x8010002AL)
#define SCARD_E_UNKNOWN_RES_MNG ((DWORD)0x8010002BL)
#define SCARD_E_NO_SUCH_CERTIFICATE ((DWORD)0x8010002CL)
#define SCARD_E_CERTIFICATE_UNAVAILABLE ((DWORD)0x8010002DL)
#define SCARD_E_NO_READERS_AVAILABLE ((DWORD)0x8010002EL)
#define SCARD_E_COMM_DATA_LOST ((DWORD)0x8010002FL)
#define SCARD_E_NO_KEY_CONTAINER ((DWORD)0x80100030L)
#define SCARD_E_SERVER_TOO_BUSY ((DWORD)0x80100031L)
#define SCARD_W_UNSUPPORTED_CARD ((DWORD)0x80100065L)
#define SCARD_W_UNRESPONSIVE_CARD ((DWORD)0x80100066L)
#define SCARD_W_UNPOWERED_CARD ((DWORD)0x80100067L)
#define SCARD_W_RESET_CARD ((DWORD)0x80100068L)
#define SCARD_W_REMOVED_CARD ((DWORD)0x80100069L)
#define SCARD_W_SECURITY_VIOLATION ((DWORD)0x8010006AL)
#define SCARD_W_WRONG_CHV ((DWORD)0x8010006BL)
#define SCARD_W_CHV_BLOCKED ((DWORD)0x8010006CL)
#define SCARD_W_EOF ((DWORD)0x8010006DL)
#define SCARD_W_CANCELLED_BY_USER ((DWORD)0x8010006EL)
#define SCARD_W_CARD_NOT_AUTHENTICATED ((DWORD)0x8010006FL)

#endif
