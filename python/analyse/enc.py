import io
import sys

def FileConvert(szOldefile, szNewfile):
    odf = open(szOldefile, "rb")
    with open(szNewfile, "wb") as ndf:
        while(odf.readable()):
            c = odf.read(1) ^ 0xa9
            ndf.write(c)
    odf.close()


if __name__ == "__main__":
    szOldefile = "E:/work/learn/python/analyse/config.en"
    szNewfile = "E:/work/learn/python/analyse/config.ini"
    FileConvert(szOldefile,szNewfile)
    pass