;
; Pretty straight-forward, mostly unoptimized, translation from C
; to POASM X86 assembly language.
;

; MD5C.C - RSA Data Security, Inc., MD5 message-digest algorithm

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

INCLUDE "md5.inc"

; Constants for MD5_transform routine
S11 EQU 7
S12 EQU 12
S13 EQU 17
S14 EQU 22
S21 EQU 5
S22 EQU 9
S23 EQU 14
S24 EQU 20
S31 EQU 4
S32 EQU 11
S33 EQU 16
S34 EQU 23
S41 EQU 6
S42 EQU 10
S43 EQU 15
S44 EQU 21

; Prototypes.
MD5_transform PROTO :PTR MD5_state, :PTR BYTE
encode PROTO :PTR BYTE, :PTR DWORD, :DWORD
decode PROTO :PTR DWORD, :PTR BYTE, :DWORD

; General "C" scaling of array index.
INDEX MACRO e:REQ, t:REQ
    EXITM ((e) * SIZEOF t)
ENDM

; Special case for indexing a DWORD array.
INDEX_DWORD MACRO e:REQ
    EXITM (INDEX(e, DWORD))
ENDM

; F, G, H and I are basic MD5 functions.

; #define F(x,y,z)  (((x) & (y)) | ((~x) & (z)))
F MACRO x:REQ, y:REQ, z:REQ
    mov eax, x
    mov edx, eax
    and eax, y
    not edx
    and edx, z
    or  eax, edx
ENDM

; #define G(x,y,z)  (((x) & (z)) | ((y) & (~z)))
G MACRO x:REQ, y:REQ, z:REQ
    mov eax, z
    mov edx, eax
    and eax, x
    not edx
    and edx, y
    or  eax, edx
ENDM

; #define H(x,y,z)  ((x) ^ (y) ^ (z))
H MACRO x:REQ, y:REQ, z:REQ
    mov eax, z
    xor eax, y
    xor eax, x
ENDM

; #define I(x,y,z)  ((y) ^ ((x) | (~z)))
I MACRO x:REQ, y:REQ, z:REQ
    mov eax, z
    not eax
    or  eax, x
    xor eax, y
ENDM

; FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
; Rotation is separate from addition to prevent recomputation.
;
; #define FF(a,b,c,d,x,s,ac) { \
    ; (a) += F ((b), (c), (d)) + (x) + (ulong_t)(ac); \
    ; (a) = ROTATE_LEFT((a), (s)); \
    ; (a) += (b); \
    ; }
FF MACRO a:REQ, b:REQ, c:REQ, d:REQ, x:REQ, s:REQ, ac:REQ
    F b,c,d
    add eax, a
    add eax, x
    add eax, ac
    rol eax, s
    add eax, b
    mov [a], eax
ENDM

; #define GG(a,b,c,d,x,s,ac) { \
    ; (a) += G ((b), (c), (d)) + (x) + (ulong_t)(ac); \
    ; (a) = ROTATE_LEFT((a), (s)); \
    ; (a) += (b); \
    ; }
GG MACRO a:REQ, b:REQ, c:REQ, d:REQ, x:REQ, s:REQ, ac:REQ
    G b,c,d
    add eax, a
    add eax, x
    add eax, ac
    rol eax, s
    add eax, b
    mov [a], eax
ENDM

; #define HH(a,b,c,d,x,s,ac) { \
    ; (a) += H ((b), (c), (d)) + (x) + (ulong_t)(ac); \
    ; (a) = ROTATE_LEFT((a), (s)); \
    ; (a) += (b); \
    ; }
HH MACRO a:REQ, b:REQ, c:REQ, d:REQ, x:REQ, s:REQ, ac:REQ
    H b,c,d
    add eax, a
    add eax, x
    add eax, ac
    rol eax, s
    add eax, b
    mov [a], eax
ENDM

; #define II(a,b,c,d,x,s,ac) { \
    ; (a) += I ((b), (c), (d)) + (x) + (ulong_t)(ac); \
    ; (a) = ROTATE_LEFT((a), (s)); \
    ; (a) += (b); \
    ; }
II MACRO a:REQ, b:REQ, c:REQ, d:REQ, x:REQ, s:REQ, ac:REQ
    I b,c,d
    add eax, a
    add eax, x
    add eax, ac
    rol eax, s
    add eax, b
    mov [a], eax
ENDM

.data

PADDING BYTE 80H, 63 DUP(0)

.code

; MD5 initialization. Begins an MD5 operation, writing a new context.
MD5_init PROC CDECL context : PTR MD5_CTX

    mov eax, context
    mov [eax].MD5_CTX.count.LSB, 0
    mov [eax].MD5_CTX.count.MSB, 0

    ; Load magic initialization constants
    mov [eax].MD5_CTX.state.A, 067452301H
    mov [eax].MD5_CTX.state.B, 0efcdab89H
    mov [eax].MD5_CTX.state.C, 098badcfeH
    mov [eax].MD5_CTX.state.D, 010325476H

MD5_init ENDP

; MD5 block update operation. Continues an MD5 message-digest
; operation, processing another message block, and updating the
; context.
MD5_update PROC CDECL USES ebx,edi,esi context : PTR MD5_CTX, input : PTR BYTE, inputLen : DWORD

    LOCAL index : DWORD
    LOCAL partLen : DWORD

    mov edx, context

    ; Compute number of bytes mod 64
    mov eax,[edx].MD5_CTX.count.LSB
    shr eax, 3
    and eax, 3FH
    mov index, eax

    ; Update number of bits
    mov eax, inputLen
    mov ecx, eax
    shl eax, 3
    shr ecx, 29
    add [edx].MD5_CTX.count.LSB, eax
    adc [edx].MD5_CTX.count.MSB, ecx

    mov eax, 64
    sub eax, index
    mov partLen, eax

    ; Transform as many times as possible
    mov eax, inputLen
    mov ecx, partLen
    .if eax >= ecx
        lea edi, [edx].MD5_CTX.buffer
        add edi, index
        mov esi, input
        rep movsb
        push edx
        INVOKE MD5_transform, ADDR [edx].MD5_CTX.state, ADDR [edx].MD5_CTX.buffer
        pop edx

        mov ebx, partLen
@@:     lea eax, [ebx + 63]
        .if eax < inputLen
            push edx
            mov ecx, input
            add ecx, ebx
            INVOKE MD5_transform, ADDR [edx].MD5_CTX.state, ecx
            pop edx
            add ebx, 64
            jmp @B
        .endif
        mov index, 0
    .else
        xor ebx, ebx
    .endif

    ; Buffer remaining input
    lea edi, [edx].MD5_CTX.buffer
    add edi, index
    mov esi, input
    add esi, ebx
    mov ecx, inputLen
    sub ecx, ebx
    rep movsb

MD5_update ENDP

; MD5 finalization. Ends an MD5 message-digest operation, writing the
;the message digest and zeroizing the context.
MD5_final PROC CDECL USES edi digest : PTR BYTE, context : PTR MD5_CTX

    LOCAL bits[8] : BYTE
    LOCAL padLen : DWORD

    mov edi, context

    ; Save number of bits
    INVOKE encode, ADDR bits, ADDR [edi].MD5_CTX.count, 8

    ; Pad out to 56 mod 64
    mov eax,[edi].MD5_CTX.count.LSB
    shr eax, 3
    and eax, 3FH
    .if eax < 56
        neg eax
        add eax, 56
    .else
        neg eax
        add eax, 120
    .endif
    mov padLen, eax
    INVOKE MD5_update, edi, ADDR PADDING, padLen

    ; Append length (before padding)
    INVOKE MD5_update, edi, ADDR bits, 8

    ; Store state in digest
    INVOKE encode, digest, ADDR [edi].MD5_CTX.state, 16

    ; Zero sensitive information
    xor eax, eax
    mov ecx, SIZEOF MD5_CTX
    rep stosb

MD5_final ENDP

; MD5 basic transformation. Transforms state based on block
MD5_transform PROC CDECL USES edi state : PTR MD5_state, block : PTR BYTE

    LOCAL a : DWORD
    LOCAL b : DWORD
    LOCAL c : DWORD
    LOCAL d : DWORD
    LOCAL x[16] : DWORD

    mov edx, state
    mov eax, [edx].MD5_state.A
    mov a, eax
    mov eax, [edx].MD5_state.B
    mov b, eax
    mov eax, [edx].MD5_state.C
    mov c, eax
    mov eax, [edx].MD5_state.D
    mov d, eax

    INVOKE decode, ADDR x, block, 64

    ; Round 1
    FF a, b, c, d, x[INDEX_DWORD( 0)], S11, 0d76aa478H  ; 1
    FF d, a, b, c, x[INDEX_DWORD( 1)], S12, 0e8c7b756H  ; 2
    FF c, d, a, b, x[INDEX_DWORD( 2)], S13, 0242070dbH  ; 3
    FF b, c, d, a, x[INDEX_DWORD( 3)], S14, 0c1bdceeeH  ; 4
    FF a, b, c, d, x[INDEX_DWORD( 4)], S11, 0f57c0fafH  ; 5
    FF d, a, b, c, x[INDEX_DWORD( 5)], S12, 04787c62aH  ; 6
    FF c, d, a, b, x[INDEX_DWORD( 6)], S13, 0a8304613H  ; 7
    FF b, c, d, a, x[INDEX_DWORD( 7)], S14, 0fd469501H  ; 8
    FF a, b, c, d, x[INDEX_DWORD( 8)], S11, 0698098d8H  ; 9
    FF d, a, b, c, x[INDEX_DWORD( 9)], S12, 08b44f7afH  ; 10
    FF c, d, a, b, x[INDEX_DWORD(10)], S13, 0ffff5bb1H  ; 11
    FF b, c, d, a, x[INDEX_DWORD(11)], S14, 0895cd7beH  ; 12
    FF a, b, c, d, x[INDEX_DWORD(12)], S11, 06b901122H  ; 13
    FF d, a, b, c, x[INDEX_DWORD(13)], S12, 0fd987193H  ; 14
    FF c, d, a, b, x[INDEX_DWORD(14)], S13, 0a679438eH  ; 15
    FF b, c, d, a, x[INDEX_DWORD(15)], S14, 049b40821H  ; 16

    ; Round 2
    GG a, b, c, d, x[INDEX_DWORD( 1)], S21, 0f61e2562H  ; 17
    GG d, a, b, c, x[INDEX_DWORD( 6)], S22, 0c040b340H  ; 18
    GG c, d, a, b, x[INDEX_DWORD(11)], S23, 0265e5a51H  ; 19
    GG b, c, d, a, x[INDEX_DWORD( 0)], S24, 0e9b6c7aaH  ; 20
    GG a, b, c, d, x[INDEX_DWORD( 5)], S21, 0d62f105dH  ; 21
    GG d, a, b, c, x[INDEX_DWORD(10)], S22, 002441453H  ; 22
    GG c, d, a, b, x[INDEX_DWORD(15)], S23, 0d8a1e681H  ; 23
    GG b, c, d, a, x[INDEX_DWORD( 4)], S24, 0e7d3fbc8H  ; 24
    GG a, b, c, d, x[INDEX_DWORD( 9)], S21, 021e1cde6H  ; 25
    GG d, a, b, c, x[INDEX_DWORD(14)], S22, 0c33707d6H  ; 26
    GG c, d, a, b, x[INDEX_DWORD( 3)], S23, 0f4d50d87H  ; 27
    GG b, c, d, a, x[INDEX_DWORD( 8)], S24, 0455a14edH  ; 28
    GG a, b, c, d, x[INDEX_DWORD(13)], S21, 0a9e3e905H  ; 29
    GG d, a, b, c, x[INDEX_DWORD( 2)], S22, 0fcefa3f8H  ; 30
    GG c, d, a, b, x[INDEX_DWORD( 7)], S23, 0676f02d9H  ; 31
    GG b, c, d, a, x[INDEX_DWORD(12)], S24, 08d2a4c8aH  ; 32

    ; Round 3
    HH a, b, c, d, x[INDEX_DWORD( 5)], S31, 0fffa3942H  ; 33
    HH d, a, b, c, x[INDEX_DWORD( 8)], S32, 08771f681H  ; 34
    HH c, d, a, b, x[INDEX_DWORD(11)], S33, 06d9d6122H  ; 35
    HH b, c, d, a, x[INDEX_DWORD(14)], S34, 0fde5380cH  ; 36
    HH a, b, c, d, x[INDEX_DWORD( 1)], S31, 0a4beea44H  ; 37
    HH d, a, b, c, x[INDEX_DWORD( 4)], S32, 04bdecfa9H  ; 38
    HH c, d, a, b, x[INDEX_DWORD( 7)], S33, 0f6bb4b60H  ; 39
    HH b, c, d, a, x[INDEX_DWORD(10)], S34, 0bebfbc70H  ; 40
    HH a, b, c, d, x[INDEX_DWORD(13)], S31, 0289b7ec6H  ; 41
    HH d, a, b, c, x[INDEX_DWORD( 0)], S32, 0eaa127faH  ; 42
    HH c, d, a, b, x[INDEX_DWORD( 3)], S33, 0d4ef3085H  ; 43
    HH b, c, d, a, x[INDEX_DWORD( 6)], S34, 004881d05H  ; 44
    HH a, b, c, d, x[INDEX_DWORD( 9)], S31, 0d9d4d039H  ; 45
    HH d, a, b, c, x[INDEX_DWORD(12)], S32, 0e6db99e5H  ; 46
    HH c, d, a, b, x[INDEX_DWORD(15)], S33, 01fa27cf8H  ; 47
    HH b, c, d, a, x[INDEX_DWORD( 2)], S34, 0c4ac5665H  ; 48

    ; Round 4
    II a, b, c, d, x[INDEX_DWORD( 0)], S41, 0f4292244H  ; 49
    II d, a, b, c, x[INDEX_DWORD( 7)], S42, 0432aff97H  ; 50
    II c, d, a, b, x[INDEX_DWORD(14)], S43, 0ab9423a7H  ; 51
    II b, c, d, a, x[INDEX_DWORD( 5)], S44, 0fc93a039H  ; 52
    II a, b, c, d, x[INDEX_DWORD(12)], S41, 0655b59c3H  ; 53
    II d, a, b, c, x[INDEX_DWORD( 3)], S42, 08f0ccc92H  ; 54
    II c, d, a, b, x[INDEX_DWORD(10)], S43, 0ffeff47dH  ; 55
    II b, c, d, a, x[INDEX_DWORD( 1)], S44, 085845dd1H  ; 56
    II a, b, c, d, x[INDEX_DWORD( 8)], S41, 06fa87e4fH  ; 57
    II d, a, b, c, x[INDEX_DWORD(15)], S42, 0fe2ce6e0H  ; 58
    II c, d, a, b, x[INDEX_DWORD( 6)], S43, 0a3014314H  ; 59
    II b, c, d, a, x[INDEX_DWORD(13)], S44, 04e0811a1H  ; 60
    II a, b, c, d, x[INDEX_DWORD( 4)], S41, 0f7537e82H  ; 61
    II d, a, b, c, x[INDEX_DWORD(11)], S42, 0bd3af235H  ; 62
    II c, d, a, b, x[INDEX_DWORD( 2)], S43, 02ad7d2bbH  ; 63
    II b, c, d, a, x[INDEX_DWORD( 9)], S44, 0eb86d391H  ; 64

    mov edx, state
    mov eax, a
    add [edx].MD5_state.A, eax
    mov eax, b
    add [edx].MD5_state.B, eax
    mov eax, c
    add [edx].MD5_state.C, eax
    mov eax, d
    add [edx].MD5_state.D, eax

    ; Zero sensitive information
    lea edi, x
    xor eax, eax
    mov ecx, SIZEOF x
    rep stosb

MD5_transform ENDP

; Encodes input (ulong_t) into output (unsigned char). Assumes len is a multiple of 4
encode PROC CDECL PRIVATE USES ebx output : PTR BYTE, input : PTR DWORD, len : DWORD

    mov ecx, len
    mov edx, output
    mov ebx, input
    .while ecx != 0
        mov eax, [ebx]
        mov [edx], eax
        add ebx, 4
        add edx, 4
        sub ecx, 4
    .endw

encode ENDP

; Decodes input (unsigned char) into output (ulong_t). Assumes len is a multiple of 4
decode PROC CDECL PRIVATE USES ebx output : PTR DWORD, input : PTR BYTE, len : DWORD

    mov ecx, len
    mov edx, output
    mov ebx, input
    .while ecx != 0
        mov eax, [ebx]
        mov [edx], eax
        add ebx, 4
        add edx, 4
        sub ecx, 4
    .endw

decode ENDP

; For testing the X86 assembly version...
asm_md5_test PROC CDECL digest : PTR BYTE, data : PTR BYTE, len : DWORD

    LOCAL context : MD5_CTX

    INVOKE MD5_init, ADDR context
    INVOKE MD5_update, ADDR context, data, len
    INVOKE MD5_final, digest, ADDR context

asm_md5_test ENDP

END
