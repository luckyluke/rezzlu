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
