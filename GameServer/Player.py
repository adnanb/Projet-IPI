class Player:
	def __init__(self, name, connection):
		self.connection = connection
		self.game = None
		self.name = name