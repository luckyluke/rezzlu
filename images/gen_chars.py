import sys
import string
#from xml.etree.ElementTree import ElementTree


def gen_files(fname):
    with open(fname) as fh:
        forig = fh.read()

    chars = string.ascii_uppercase.replace('A', '')
    print chars
    print fname
    for c in chars:
        fname_new = fname.replace('A', c)
        print fname_new
        fnew = forig.replace(fname, fname_new)
        fnew = fnew.replace('>A<', '>'+c+'<')
        with open(fname_new, 'w') as fh:
            fh.write(fnew)

gen_files('cell_A.svg')
gen_files('cell_A_active.svg')
