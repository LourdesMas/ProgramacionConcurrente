#! /usr/bin/env python

import threading

THREADS = 5
counter = 0

def thread():
	global counter

	print("Thread {}".format(threading.current_thread().name))
	for i in range (500000):
		counter +=1

def main():
	threads = []

	for i in range(THREADS):
		t = threading.Thread(target=thread)
		threads.append(t)
		t.start()

	for t in threads:
		t.join()

	print ("Counter value: {}".format(counter))

if __name__ == "__main__":
	main()