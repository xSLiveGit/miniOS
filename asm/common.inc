%ifndef __COMMON_INC__
%define __COMMON_INC__

%define ORIGIN_MBR 7C00h
%define ORIGIN_SSL 7E00h
%define ORIGIN_KERNEL 8000h
%define ORIGIN_BASE_KERNEL    110000h
%define break xchg bx, bx

%define END_SECTION_NAME_LENGTH 13
%define END_SECTION_NAME 0x4d, 0x41, 0x52, 0x49, 0x45, 0x53, 0x20, 0x53, 0x45, 0x52, 0x47, 0x49, 0x55

%endif