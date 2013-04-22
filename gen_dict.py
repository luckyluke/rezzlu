import sys
from solver import OptDict

if __name__=='__main__':
    try:
        lang = sys.argv[1]
    except IndexError:
        print >>sys.stderr, "uso:", sys.argv[0], "lang"
        sys.exit(1)

    od = OptDict()
    print >>sys.stderr, "Leggo dict", lang
    od.read('dicts/'+lang+'.list')
    od.opt = True
    print >>sys.stderr, "Ottimizzo dict"
    od.build()
    od.build_stats()
    print >>sys.stderr, "Scrivo dict ottimizzato", lang
    od.write('dicts/'+lang+'.dict')
