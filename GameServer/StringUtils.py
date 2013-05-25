#!/usr/bin/env python3

class StringBuilder:
	def __init__(self):
		self.data = ""

	def add(self, string):
		size = len(string)
		if self.data != "":
			temp = self.data.split(":", 1)[1]
		else:
			temp = ""
		temp += str(size)+":"+string+","
		self.data = str(len(temp))+":"+temp

def StringExtract(string):
	out = []
	while string != "":
		temp = string.split(":", 1)
		l = int(temp[0])
		out.append(temp[1][:l])
		string = temp[1][l+1:]
	return out