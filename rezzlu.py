from words import SchemaManager
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
    sm = SchemaManager()
    main_loop(sm)
