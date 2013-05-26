
class Game:
    def __init__(self, name, nb_players, size, nb_alignments):
    	self.name = name
    	self.nb_players = nb_players
    	self.size = size
    	self.nb_alignments = nb_alignments
    	self.size = size
    	self.grid = []
    	self.players = []
    	self.waiting = True
    	self.admin = None
