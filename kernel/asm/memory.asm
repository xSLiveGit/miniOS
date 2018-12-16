%define PDT             103000h
; At 104000h there are page tables for identity map (first 2 mb)
%define PT              106000h 

; We allocate at 1000000h (16 mb)
; We will allocate a maximum of 64 pages of 4KB each
%define BASE_ADDRESS    1000000h
%define PAGE_SIZE       4096
%define MAX_PAGES       64
