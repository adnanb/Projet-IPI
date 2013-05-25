
class Game:
    def __init__(self, name, nb_players, size, nb_alignments):
    	self.name = name
    	self.nb_players = nb_players
    	self.size = size
    	self.nb_alignements = nb_alignments
    	self.size = size
    	self.grid = []
    	self.players = []
    	self.waiting = True
    	self.players = []
    	self.admin = None
