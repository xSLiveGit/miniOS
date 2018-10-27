import os
import tempfile

PATH_ASM_DIR = os.path.join(os.getcwd(), 'asm')
PATH_BINARY_FILE = os.path.join(os.getcwd(), 'bin', 'os.bin')
PATH_KERNER = os.path.join(os.getcwd(), 'kernel')
PATH_CFG = os.path.join(os.getcwd(), 'cfg', 'os.bxrc')
FLOPPY_SIZE = 1474560

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
    print('CDM IS: [' + cmd + ']')
    os.system(cmd)

    with open(str(outputFile), 'rb') as inputStream:
        BinaryStream.write(inputStream.read())
    os.unlink(outputFile)


def BuildBinFromAsm(AsmDirectory, BinaryFilePath):
    with open(BinaryFilePath, 'wb') as binaryStream:
        for root, _, files in os.walk(AsmDirectory):
            for file in files:
                AddAsmInBinaryFile(file, BinaryFilePath, root, binaryStream)

def Run():
    cmd = 'bochsdbg.exe -q -f "' + PATH_CFG + '"'
    print(cmd)
    os.system(cmd)

if __name__ == "__main__":
    BuildBinFromAsm(PATH_ASM_DIR, PATH_BINARY_FILE)
    AddPadding(PATH_BINARY_FILE, FLOPPY_SIZE)
    Run()
    