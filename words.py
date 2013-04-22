import solver


class WordManager(object):
    """ logica di gestione del gioco """
    # opzioni? 
    schema_rows = 4
    schema_cols = 4
    def __init__(self):
        self._tmpw = []
        #self.schema = schema
        #self.load_dict('ita')

    def do_config(self, cfg):
        self.load_dict(cfg.lang)

    def get_schema(self, rows, cols):
        #d = solver.load_dict('dicts/ita.dict', False)
        #stats = self.wdict.build_stats()
        self.wdict.build_stats()
        schema = solver.gen_game(self.wdict.stats, rows, cols)
        return schema

    def solve_schema(self, schemas):
        self.wdict.optimize()
        sols = []
        for sch in schemas:
            sol = solver.solve(sch, self.wdict)
            sol.append(sol)
        self.wdict.deoptimize()
        return sols

    def load_dict(self, lang):
        self.wdict = solver.OptDict()
        self.wdict.read('dicts/'+lang+'.dict')
        self.wdict.lang = lang

    def put_char(self, char):
        """ Ritorna la parola selezionata"""
        if self._tmpw:
            # tornato indietro di una
            if len(self._tmpw) > 1 and char is self._tmpw[-2]:
                self._tmpw = self._tmpw[:-1]
                print [c.char for c in self._tmpw]
                return self._tmpw

            # lettera gia selezionata
            elif char in self._tmpw:
                return self._tmpw

            # lettera troppo distante dall'ultima
            lastc = self._tmpw[-1]
            if abs(lastc.row - char.row) > 1 or abs(lastc.col - char.col) > 1:
                return self._tmpw

        self._tmpw.append(char)
        print [c.char for c in self._tmpw]
        return self._tmpw

    def stop_word(self):
        word = self._tmpw
        self._tmpw = []
        return word

    def check_word(self, word):
        word = word.lower()
        #if word == 'bu':
        #if word in self.wdict:
        if self.wdict.find(word) == 0:
            print "Trovato", word, "!!!!!!"
            return True
        else:
            return False
