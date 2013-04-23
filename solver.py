# liste di parole italiane:
# https://dl.dropbox.com/u/5813814/280000_parole_italiane.zip
# https://dl.dropbox.com/u/5813814/60000_parole_italiane.zip
# http://coding.napolux.com/post/42114980272/ruzzle-lista-parole-italiane

import sys
import string
import random
import cPickle

def vicini(game, row, col, usati):
    vic = []
    for r in [row-1, row, row+1]:
        for c in [col-1, col, col+1]:
            # fuori dallo schema
            if r < 0 or r > len(game)-1 or c < 0 or c > len(game[0])-1:
                continue
            # stesso di partenza
            if r == row and c == col:
                continue
            if (r, c) in usati:
                continue
            vic.append((r, c))
    return vic

class OptDict(object):
    # struttura ad albero a dizionari
    # termine della parla segnalato con un {None: None} nel diz child
    def __init__(self):
        self.stats = {}
        #self._opt = opt
        self._d = {}
        self._rawd = []
        #if fname:
        #    self.read(fname)
        self.find = self.find_noopt

    def read(self, fname):
        with open(fname) as fh:
            if fname.endswith('.dict'):
                self._rawd = map(str.strip, fh.readlines())
                #self._d = dict.fromkeys(map(lambda w: w.replace('\n', '').
                #                            replace('\r', ''), fh),
                #                        {None:None})
                #self._rawd = self._d.keys()

                #self.build_stats()
                #self.build_opt(fh)

            #elif fname.endswith('.dict'):
            #    self._d, self.stats = cPickle.load(fh)

            else:
                print "OptDict:", fname, "non letto"

    # def write(self, fname):
    #     with open(fname, 'w') as fh:
    #         if fname.endswith('.list'):
    #             fh.write('\r\n'.join(self._rawd))

    #         #elif fname.endswith('.dict'):
    #         #    cPickle.dump((self._d, self.stats), fh)

    #         else:
    #             print "OptDict:", fname, "non scritto"

    # def build(self):
    #     if self._opt:
    #         self.build_opt(self._rawd)
    #     else:
    #         self.build_noopt(self._rawd)

    # def find(self, word):
    #     if self._opt:
    #         return self.find_opt(word)
    #     else:
    #         return self.find_noopt(word)

    def optimize(self, *a, **kw):
        self.build_opt(*a, **kw)
        self.find = self.find_opt

    def deoptimize(self):
        self._d = {}
        self.find = self.find_noopt

    def build_stats(self):
        if not self.stats:
            d = ''.join(self._rawd)
            totc = float(len(d))
            stats = {}
            for c in string.ascii_lowercase:
                stats[c] = d.count(c) / totc
            self.stats = stats

    def find_noopt(self, word):
        wl = filter(lambda w: w.startswith(word), self._rawd)
        if len(wl) == 0:
            return -1
        elif word in wl:
            return 0
        else:
            return 1

    # def build_opt(self, fh=None):
    #     for w in fh:
    def build_opt(self, progress_cb=None):
        if progress_cb:
            wid = 0

        for w in self._rawd:
            wl = list(w)
            wlen = len(w)
            tmpd = self._d
            for i, c in enumerate(wl):
                #if len(tmpd.keys()) == 0 or not tmpd.has_key(c):
                if not tmpd.has_key(c):
                    # parola nuova
                    tmpd[c] = {}

                if i == wlen - 1:
                    # teminatore di parola
                    tmpd[c][None] = None
                else:
                    # parola ok fin qua, scorro avanti
                    tmpd = tmpd[c]

            if progress_cb:
                wid += 1
                if wid % 1000 == 1:
                    progress_cb()

    def find_opt(self, word):
        ret = -1
        wl = list(word)
        tmpd = self._d
        wlen = len(word)
        # scorro l'albero lettera per lettera finche' ci sono
        for i, c in enumerate(wl):
            if tmpd.has_key(c):
                tmpd = tmpd[c]
            else:
                break
        else:
            if tmpd.has_key(None):
                # parola trovata
                ret = 0
            elif tmpd.keys():
                # trovate piu di una parola
                ret = 1
        return ret

    # def semi_optimize(self):
    #     self.build_semiopt()
    #     self.find = self.find_semiopt
    #     #print 'semiopt:',self._d

    # def build_semiopt(self):
    #     #nd = {None: None}
    #     #self._d = dict([(w,nd) for w in self._rawd])

    #     #for w in self._rawd:
    #     #    self._d[w] = {None:None}

    #     #self._d = dict.fromkeys(self._rawd, {None:None})
    #     pass

    #     #### ok, velocizzazione quasi nulla, 2-3 %
    #     # for w in self._rawd:
    #     #     wl = list(w)
    #     #     wlen = len(w)
    #     #     tmpd = self._d
    #     #     for i, c in enumerate(wl[:2]):
    #     #         #if len(tmpd.keys()) == 0 or not tmpd.has_key(c):
    #     #         if not tmpd.has_key(c):
    #     #             # parola nuova
    #     #             tmpd[c] = {}
    #     #
    #     #         if i == wlen - 1:
    #     #             # teminatore di parola
    #     #             tmpd[c][None] = None
    #     #         elif i == 1:
    #     #             # parola non terminata ma ultima iterazione
    #     #             tmpd[c][w[2:]] = {None:None}
    #     #         else:
    #     #             # parola ok fin qua, scorro avanti
    #     #             tmpd = tmpd[c]

    # def find_semiopt(self, word):
    #     ret = -1
    #     wl = list(word)
    #     tmpd = self._d
    #     wlen = len(word)
    #     # scorro l'albero lettera per lettera finche' ci sono
    #     for i, c in enumerate(wl):
    #         if tmpd.has_key(c):
    #             tmpd = tmpd[c]
    #             continue
    #         # ottimizzo il prossimo step e continuo la ricerca
    #         for k in tmpd.keys():
    #             if k is None:
    #                 continue
    #             if k.startswith(c):
    #                 self._build_subdicts(tmpd)
    #                 tmpd = tmpd[c]
    #                 break
    #         else:
    #             break
    #     else:
    #         if tmpd.has_key(None):
    #             # parola trovata
    #             ret = 0
    #         elif tmpd.keys():
    #             # trovate piu di una parola
    #             ret = 1
    #     return ret

    # def _build_subdicts(self, tmpd, keys=None):
    #     """
    #     Espande l'albero, creando i nodi superiori ad 1 carattere
    #     trasforma da {'haha':{None:None}, 'he':{None:None}, 'baba':{None:None}}
    #     a {'h':{'aha':{None:None}, 'e':{None:None}}, 'b':{'aba':{None:None}}}
    #     """
    #     if not keys:
    #         keys = tmpd.keys()
    #     for w in keys:
    #         if w is None:
    #             # parola finita, niente da espandere
    #             continue
    #         if len(w) == 1:
    #             # mantengo chiavi ad 1 carattere, sono il punto di arrivo
    #             continue
    #         del tmpd[w]
    #         c = w[0]
    #         if not tmpd.has_key(c):
    #             tmpd[c] = {}
    #         #print c, tmpd[c], w[1:]
    #         tmpd[c][w[1:]] = {None:None}
                

def solve_char(game, d, starts, srow, scol, usati, path):
    words = []
    for row, col in vicini(game, srow, scol, usati):
        neww = starts+game[row][col]
        tmppath = path+[(row, col)]
        w_in_d = d.find(neww)
        # se non ho piu parole nel dizionario che iniziano con la parola a cui
        # sono arrivato ora mi fermo, non andrei da nessuna parte
        if w_in_d == -1:
            continue
        # la parola attuale e' valida
        elif w_in_d == 0:
            words.append((neww, tmppath))
        newws = solve_char(game, d, neww, row, col, usati+[(row, col)], tmppath)
        words.extend(newws)

    return words

def solve(game, d, progress_cb=None):
    game = load_game(game)
    rows = len(game)
    columns = len(game[0])
    words = []
    for row in range(rows):
        for col in range(columns):
            tmp = game[row][col]
            newws = solve_char(game, d, tmp, row, col,
                               [(row, col)], [(row, col)])
            words.extend(newws)
            if progress_cb:
                progress_cb()
    return words

def load_dict(fname, opt=True):
    od = OptDict()
    od.read(fname)
    od.optimize()
    return od

def load_game(game):
    rows = len(game)
    columns = len(game[0])
    newg = [['']*columns for _ in range(rows)]
    for row in range(rows):
        for col in range(columns):
            newg[row][col] = game[row][col].lower()
    return newg

def gen_game(stats, rows, columns):
    # genero distribuzine cumulativa
    ptmp = 0
    for k, v in stats.copy().iteritems():
        ptmp += v
        stats[k] = ptmp
    # uso prob cumul come chiave
    pstats = dict([(v, k) for k, v in stats.iteritems()])
    pvs = pstats.keys()
    pvs.sort()
    chars = []
    # seleziono i caratteri pesando una distrib uniforme
    for i in range(rows*columns):
        rand = random.random()
        for p in pvs:
            if p >= rand:
                c = pstats[p]
                break
        else:
            print rand
        chars.append(c)
    # creo la griglia con i caratteri
    game = [['']*columns for i in range(rows)]
    for row in range(rows):
        for col in range(columns):
            #print row, col, rows, columns, len(chars), columns*row +col
            game[row][col] = chars[columns*row+col].upper()
    return game

TEST=[['G', 'E', 'H', 'C'],
      ['R', 'S', 'G', 'Y'],
      ['C', 'E', 'B', 'U'],
      ['D', 'Y', 'G', 'A']]

if __name__=='__main__':
    print >>sys.stderr, "Carico dizionario e schema ..."
    d = load_dict('dicts/ita.dict')
    #d.build_stats()
    #g = gen_game(d.stats, 4, 4)
    g = TEST
    print g
    game = load_game(g)
    #d.semi_optimize()
    #d.optimize()

    print >>sys.stderr, "Inizio risoluzione ..."
    sol = solve(game, d)
    wuniq = []
    for w, path in sol:
        if w not in wuniq:
            wuniq.append(w)
    print 'Soluzione', len(sol), 'parole,', len(wuniq), 'uniche:'
    #for w in sol:
    #    print w
    print ' '.join(wuniq)
