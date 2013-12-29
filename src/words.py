#
# Copyright (C) 2013 Luca Dariz
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import rezzlu_game

class SchemaManager(object):
    cfg = None

    def do_config(self, cfg):
        self.cfg = rezzlu_game.game_config_t()
        self.cfg.rows = cfg.rows
        self.cfg.cols = cfg.columns
        self.cfg.solve_all = int(cfg.solve_all)
        self.load_dict(cfg.lang)

    def get_timer(self):
        if self.cfg.rows*self.cfg.cols == 16:
            return 120
        else:
            return 180

    def get_schema(self):
        #self.wdict.build_stats() # TODO
        g = rezzlu_game.game_alloc(self.cfg)
        rezzlu_game.game_gen(g)
        return g

    def solve_schema(self, schema, progress_cb=None):
        sol = rezzlu_game.solution_t()
        rezzlu_game.solve_game(schema, sol)
        return sol

    def load_dict(self, lang):
        self.wdict = rezzlu_game.load_dict('../dicts/'+lang+'.dict\0')
        self.cfg.dict = self.wdict

    def get_gm(self, progress_cb=None):
        gm = GameManager()
        gm.schema = self.get_schema()
        gm.bonus = {'dw':(1, 0),
                    'dc':(3, 2),
                    'tw':(1, 1),
                    'tc':(2, 1)}
        gm.rows = self.cfg.rows
        gm.cols = self.cfg.cols
        gm.cvalues = self.get_values()
        if self.cfg.solve_all:
            gm.sol = self.solve_schema(gm.schema, progress_cb)
        else:
            gm.wdict = self.wdict
        return gm

    def get_values(self):
        vals = {} # TODO
        # minv = 1/max(self.wdict.stats)
        # for k, v in self.wdict.stats.iteritems():
        #     vals[k] = int(1/(v*minv))
        return vals

class GameManager(object):
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
        self.gstat = rezzlu_game.game_status_t()

    def get_cell_char(self, row, col):
        return rezzlu_game.char_get(self.schema.ch, row, col)        

    def put_char(self, char):
        """ Ritorna la parola selezionata"""
        # if self._tmpw:
        #     # tornato indietro di una
        #     if len(self._tmpw) > 1 and char is self._tmpw[-2]:
        #         self._tmpw = self._tmpw[:-1]
        #         return self._tmpw

        #     # lettera gia selezionata
        #     elif char in self._tmpw:
        #         return self._tmpw

        #     # lettera troppo distante dall'ultima
        #     lastc = self._tmpw[-1]
        #     if abs(lastc.row - char.row) > 1 or abs(lastc.col - char.col) > 1:
        #         return self._tmpw

        # self._tmpw.append(char)
        return rezzlu_game.game_put_char(self.gstat, 'c')
        # return self._tmpw

    def stop_word(self):
        # word = self._tmpw
        # self._tmpw = []
        # return word
        return rezzlu_game.game_stop_word(self.gstat)

    def check_word(self, word, path):
        # word = word.lower()
        # if (word not in self.found):
        #     if (self.sol and word in [w for w, path in self.sol]) or\
        #              (self.wdict and self.wdict.find(word) == 0):
        #         print "Trovato", word, "!!!!!!"
        #         self.found.append(word)
        #         self.score += self.calc_score(word, path)
        #         return True
        # else:
        #     return False
        return rezzlu_game.game_check_word(self.gstat, word)

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
