import solver


class GameManager(object):
    cfg = None

    def do_config(self, cfg):
        self.cfg = cfg
        self.load_dict(cfg.lang)

    def get_timer(self):
        if self.cfg.rows*self.cfg.columns == 16:
            return 120
        else:
            return 180

    def get_schema(self):
        self.wdict.build_stats()
        schema = solver.gen_game(self.wdict.stats,
                                 self.cfg.rows, self.cfg.columns)
        return schema

    def solve_schema(self, schema, progress_cb=None):
        self.wdict.optimize(progress_cb)
        sol = solver.solve(schema, self.wdict, progress_cb)
        self.wdict.deoptimize()
        return sol

    def load_dict(self, lang):
        self.wdict = solver.OptDict()
        self.wdict.read('dicts/'+lang+'.dict')
        self.wdict.lang = lang

    def get_wm(self, progress_cb=None):
        wm = WordManager()
        wm.schema = self.get_schema()
        wm.bonus = {'dw':(1, 0),
                    'dc':(3, 2),
                    'tw':(1, 1),
                    'tc':(2, 1)}
        wm.rows = self.cfg.rows
        wm.cols = self.cfg.columns
        wm.cvalues = self.get_values()
        if self.cfg.solve_all:
            wm.sol = self.solve_schema(wm.schema, progress_cb)
        else:
            wm.wdict = self.wdict
        return wm

    def get_values(self):
        vals = {}
        minv = 1/max(self.wdict.stats.values())
        for k, v in self.wdict.stats.iteritems():
            vals[k] = int(1/(v*minv))
        return vals

class WordManager(object):
    """ logica di gestione del gioco """
    def __init__(self):
        self._tmpw = []
        self.schema = []
        self.bonus = {}
        self.rows = 0
        self.cols = 0
        self.sol = []
        self.wdict = None
        self.cvalues = {}
        self.found = []
        self.score = 0

    def put_char(self, char):
        """ Ritorna la parola selezionata"""
        if self._tmpw:
            # tornato indietro di una
            if len(self._tmpw) > 1 and char is self._tmpw[-2]:
                self._tmpw = self._tmpw[:-1]
                return self._tmpw

            # lettera gia selezionata
            elif char in self._tmpw:
                return self._tmpw

            # lettera troppo distante dall'ultima
            lastc = self._tmpw[-1]
            if abs(lastc.row - char.row) > 1 or abs(lastc.col - char.col) > 1:
                return self._tmpw

        self._tmpw.append(char)
        return self._tmpw

    def stop_word(self):
        word = self._tmpw
        self._tmpw = []
        return word

    def check_word(self, word, path):
        word = word.lower()
        if (word not in self.found) and\
                ((self.sol and word in [w for w, path in self.sol]) or \
                     self.wdict.find(word) == 0):
            print "Trovato", word, "!!!!!!"
            self.found.append(word)
            self.score += self.calc_score(word, path)
            return True
        else:
            return False

    def calc_score(self, word, path):
        mul = 1
        summ = 1
        if self.bonus['dw'] in path:
            mul *= 2
        if self.bonus['tw'] in path:
            mul *= 3
        if self.bonus['dc'] in path:
            dci = path.index(self.bonus['dc'])
            dcc = path.count(self.bonus['dc'])
            summ += self.cvalues[word[dci]]*dcc
        if self.bonus['tc'] in path:
            tci = path.index(self.bonus['tc'])
            tcc = path.count(self.bonus['tc'])
            summ += 2*self.cvalues[word[tci]]*tcc
            
        return mul*(sum([self.cvalues[c] for c in word]) + summ)
