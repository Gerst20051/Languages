; MD5.INC - header file for MD5.ASM

; Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
; rights reserved.

; License to copy and use this software is granted provided that it
; is identified as the "RSA Data Security, Inc. MD5 Message-Digest
; Algorithm" in all material mentioning or referencing this software
; or this function.

; License is also granted to make and use derivative works provided
; that such works are identified as "derived from the RSA Data
; Security, Inc. MD5 Message-Digest Algorithm" in all material
; mentioning or referencing the derived work.

; RSA Data Security, Inc. makes no representations concerning either
; the merchantability of this software or the suitability of this
; software for any particular purpose. It is provided "as is"
; without express or implied warranty of any kind.

; These notices must be retained in any copies of any part of this
; documentation and/or software.

; MD5 state
MD5_state STRUCT
    A DWORD ?
    B DWORD ?
    C DWORD ?
    D DWORD ?
MD5_state ENDS

; MD5 count
MD5_count STRUCT
    LSB DWORD ?
    MSB DWORD ?
MD5_count ENDS

; MD5 context
MD5_CTX STRUCT
    state MD5_state {?}     ; state (ABCD)
    count MD5_count {?}     ; number of bits, modulo 2^64 (lsb first)
    buffer BYTE 64 DUP(?)   ; input buffer
MD5_CTX ENDS

; void __cdecl MD5_init(MD5_CTX *);
MD5_init PROTO CDECL :PTR MD5_CTX
; void __cdecl MD5_update(MD5_CTX *, unsigned char *, unsigned int);
MD5_update PROTO CDECL :PTR MD5_CTX, :PTR BYTE, :DWORD
; void __cdecl MD5_final(unsigned char [16], MD5_CTX *);
MD5_final PROTO CDECL :PTR BYTE, :PTR MD5_CTX
