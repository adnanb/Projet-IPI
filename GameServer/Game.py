
class Game:
    def __init__(self, name, nb_players, size, nb_alignments_max):
    	self.name = name
    	self.nb_players = nb_players
    	self.size = size
    	self.nb_alignments_max = nb_alignments_max
    	self.size = size
    	self.grid = []
    	self.players = []
    	self.waiting = True
    	self.admin = None
        self.nb_alignments = 0

    def vertical_check(x, y, player):
        j1 = y
        j2 = y

        while j1 > 0 && (self.grid[x+self.size*(j1-1)].state == 1):
            j1--

        while j2 < (self.size-1) && (self.grid[x+self.size*(j2+1)].state == 1):
            j2++


        if (j2-j1+1) >= 5:
            for j in range (j1, j1+5):
                square = self.grid[x+self.size*j]
                square.state = 2
                square.player = player

    def horizontal_check(x, y, player):
        i1 = x
        i2 = x

        while i1 > 0 && (self.grid[(i1-1)+self.size*y].state == 1):
            i1--

        while i2 < (self.size-1) && (self.grid[(i2+1)+self.size*y].state == 1):
            i2++


        if (i2-i1+1) >= 5:
            for i in range (i1, i1+5):
                square = self.grid[i+self.size*y]
                square.state = 2
                square.player = player


    def diag_right_check(x, y, player):
        i1, j1 = x, y
        i2, j2 = x, y

        while i1 < (self.size-1) && j1 > 0 && (self.grid[(i1+1)+self.size*(j1-1)].state == 1):
            i1++
            j1--

        while i2 > 0 && j2 < (self.size-1) && (self.grid[(i2-1)+self.size*(j2+1)].state == 1):
            i2--
            j2++


        if (i1-i2+1) >= 5:
            for j in range (j1, j1+5):
                square = self.grid[i1+self.size*j]
                square.state = 2
                square.player = player
                i1--

    def diag_left_check(x, y, player):
        i1, j1 = x, y
        i2, j2 = x, y

        while i1 > 0 && j1 > 0 && (self.grid[(i1-1)+self.size*(j1-1)].state == 1):
            i1--
            j1--

        while i2 < (self.size-1) && j2 > (self.size-1) && (self.grid[(i2+1)+self.size*(j2+1)].state == 1):
            i2++
            j2++


        if (i2-i1+1) >= 5:
            for i in range (i1, i1+5):
                square = self.grid[i+self.size*j1]
                square.state = 2
                square.player = player
                j1++


    def check(x, y, player):
        self.horizontal_check(x, y, player);
        self.vertical_check(x, y, player);
        self.diag_left_check(x, y, player);
        self.diag_right_check(x, y, player);

    def play(x, y, player):
        #On vérifie que x et y sont des cases existantes
        if (x<0 or x>self.size-1) or (y<0 or y>self.size-1):
            return 1

        square = self.grid[x+self.size*y]
        if square.state == 0:
            square.state = 1
            square.player = player

        self.check()

        return 0


