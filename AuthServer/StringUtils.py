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
    array = []
    while string != "":
        l, string = string.split(":", 1)
        array.append(string[:int(l)])
        string = string[int(l)+1:]
    return array