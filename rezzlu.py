from words import WordManager
from gui import main_loop

TEST=[['G', 'E', 'H', 'C'],
      ['R', 'S', 'G', 'Y'],
      ['C', 'E', 'B', 'U'],
      ['D', 'Y', 'G', 'A']]

# TEST=[['A', 'A', 'A', 'A'],
#       ['A', 'A', 'A', 'A'],
#       ['A', 'A', 'A', 'A'],
#       ['A', 'A', 'A', 'A']]

if __name__=='__main__':
    wm = WordManager()
    main_loop(wm)
