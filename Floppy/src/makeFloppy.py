import os, sys, time

params = len(sys.argv)
if (params < 2):
    print ("Usage: \n %s file1.asm [file2.asm] [... fileN.asm]\n"%(sys.argv[0]))
    exit(1)

#we have cmd line, open the result file
out = open("floppy.img", "wb", )

#we have the result file, iterate files and compile them and them to the floppy
for i in range(1, params):
    print ("Compiling %s"%(sys.argv[i]))
    os.system("nasm -f bin %s -o tmp.bin"%(sys.argv[i]))
    out.write(open("tmp.bin","rb").read())

#cleanup    
# os.remove("tmp.bin")


out.write(open("kernel32.bin","rb").read())
out.close()
# add padding to the end of floppy
time.sleep(1)
flpSize = 1474560  
size = os.path.getsize("floppy.img")
out = open("floppy.img", "ab")

print ("Adding padding from %d to %d ...\n" % (size, flpSize))

# x = "\n"
# finalTex = ''.join([x for i in range(0, (flpSize - size))])
	
# out.write(finalTex.encode("utf-8").replace("\n", os.linesep))

# out.close()