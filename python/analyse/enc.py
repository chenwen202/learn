import io
import sys

def FileConvert(szOldefile, szNewfile):
    odf = open(szOldefile, "rb")
    with open(szNewfile, "w") as ndf:
        c = odf.read(1)        
        ndf.write(chr(ord(c)))
        while True:
            c = odf.read(1)
            if not c: break
            c = ord(c) ^ 0xa9
            ndf.write(chr(c))
    odf.close()

def FileEncoding(szSrcFile,szEncFile):
    sf = open(szSrcFile, "rb")
    with open(szEncFile, "wb") as edf:
        c = sf.read(1)
        edf.write(c)
        while True:
            c = sf.read(1)
            if not c: break
            c = ord(c) ^ 0xa9
            edf.write(bytes([c]))
    sf.close()


if __name__ == "__main__":
    szOldefile = "E:/work/learn/python/analyse/config.en"
    szNewfile = "E:/work/learn/python/analyse/config.ini"
    szEncfile = "E:/work/learn/python/analyse/config1.en"
    szNewfile2 = "E:/work/learn/python/analyse/config1.ini"
    #FileConvert(szOldefile,szNewfile)
    FileEncoding(szNewfile, szEncfile)
    FileConvert(szEncfile,szNewfile2)
    pass