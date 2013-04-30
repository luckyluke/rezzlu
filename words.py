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

    def check_word(self, word):
        word = word.lower()
        if (word not in self.found) and\
                ((self.sol and word in [w for w, path in self.sol]) or \
                     self.wdict.find(word) == 0):
            print "Trovato", word, "!!!!!!"
            self.found.append(word)
            self.score += self.calc_score(word)
            return True
        else:
            return False

    def calc_score(self, word):
        return sum([self.cvalues[c] for c in word])
