import os
import tempfile

PATH_ASM_DIR = os.path.join(os.getcwd(), 'asm')
PATH_BINARY_FILE = os.path.join(os.getcwd(), 'bin', 'os.bin')
PATH_KERNEL = os.path.join(os.getcwd(), 'kernel')
PATH_CFG = os.path.join(os.getcwd(), 'cfg', 'os.bxrc')
FLOPPY_SIZE = 1474560
DISK_SIZE = 33554432  # 32 mb is enough...

def AddPadding(Path, TotalSize):


    actualSize = os.path.getsize(Path)
    if(actualSize >= TotalSize):
        return

    padding = '\0' * (TotalSize - actualSize)

    with open(Path, 'ab') as f:
        f.write(str(padding).encode())

def CreateFile(FilePath):
    if os.path.isfile(FilePath):
        os.unlink(FilePath)
    with open(FilePath, 'w+'):
        pass

def AddAsmInBinaryFile(AsmFile, BinaryFilePath, Root, BinaryStream):
    if not AsmFile.endswith('.asm'):
        return
    
    path =  os.path.join(Root, AsmFile)
    outputFile = path + '.obj'
    CreateFile(outputFile)
    cmd = 'nasm -o "%s" -fbin "%s"'%(outputFile, path)
    # print('CDM IS: [' + cmd + ']')
    os.system(cmd)

    with open(str(outputFile), 'rb') as inputStream:
        BinaryStream.write(inputStream.read())
    os.unlink(outputFile)

def LoadKernel(KernelPathDirectory, BinaryFilePath):
    binaryDirPath   = os.path.join(KernelPathDirectory, 'bin')
    headersDirPath  = os.path.join(KernelPathDirectory, 'headers')
    sourcesDirPath  = os.path.join(KernelPathDirectory, 'sources')
    asmIncludesPath = os.path.join(KernelPathDirectory, 'asm', 'include')  + '\\'
    obj             = os.path.join(binaryDirPath, "kernel.bin")
    lnk = ''

    print('[DBG] binaryDirPath: {%s}, headerDirPath:{%s}, asmIncludesPath:{%s}'%(binaryDirPath, headersDirPath, asmIncludesPath))

    for root, _, files in os.walk(KernelPathDirectory):
        for f in files:
            src     = os.path.join(root, f)
            output  = os.path.join(binaryDirPath, f + '.obj')
            asmo    = os.path.join(binaryDirPath, f + '.asmo')
            # print('src: {%s} output: {%s} asmo: {%s}'%(src, output, asmo))

            if f.endswith('.asm'):
                cmd = 'nasm -f elf64 -O0 -o "' + output + '" "' + src + '" -I "' + asmIncludesPath + '"'
                os.system(cmd)

            elif f.endswith('.c'):
                # print('inainte de gcc')
                cmd = 'cc1 -mabi=ms -std=c99 -ffreestanding -O0 "' + src + '" -o "' + asmo + '" -Wall -Werror -Wfatal-errors -masm=intel -I "' + headersDirPath + '"'
                # print('cmd:%s'%(cmd))
                os.system(cmd)
                cmd = 'as --64 "' + asmo + '" -o "' + output + '" -msyntax=intel';            
                # print('cmd:%s pentru as'%(cmd))
                os.system(cmd)
            else:
                continue
            lnk += ' "' + output + '" '
    print('LNK: ' + str(lnk))
    cmd = 'ld -O0 -Ttext 0x110000 -Tdata 0x150000 -Tbss 0x200000 --oformat binary -o "' + obj + '" ' + lnk + ' -m elf_x86_64'
    # cmd = 'ld -O0 -Ttext 0x110000 -Tdata 0x150000 -Tbss 0x175000 --oformat binary -o "' + obj + '" ' + lnk + ' -m elf_x86_64 -nostdlib'
    # print('[INFO] ld cmd: %s'%(cmd))
    os.system(cmd)
    # os.system('ld -O0 -Tdata 0x125000 -Tbss 0x150000 --oformat binary -o "' + obj + '" "' + lnk + '" -m elf_x86_64')

    print('Binary fila path: %s'%(BinaryFilePath))
    with open(BinaryFilePath, 'ab') as o:
        with open(obj, 'rb') as p:
            o.write(p.read())

def BuildBinFromAsm(AsmDirectory, BinaryFilePath):
    with open(BinaryFilePath, 'wb') as binaryStream:
        for root, _, files in os.walk(AsmDirectory):
            for file in files:
                AddAsmInBinaryFile(file, BinaryFilePath, root, binaryStream)

def Run():
    cmd = 'bochsdbg.exe -q -f "' + PATH_CFG + '"'
    # print(cmd)
    os.system(cmd)

if __name__ == "__main__":
    BuildBinFromAsm(PATH_ASM_DIR, PATH_BINARY_FILE)
    LoadKernel(PATH_KERNEL, PATH_BINARY_FILE)
    AddPadding(PATH_BINARY_FILE, DISK_SIZE)
    Run()
    