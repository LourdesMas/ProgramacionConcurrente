# -*- coding: utf-8 -*-


#! /usr/bin/env python

import threading
import time


THREADS = 2
MAX_COUNT = 10000

counter = 0




def thread(id):
    global counter
    global states   #array to represent wether an P wants access to sc
    global turn     
    states = [False,False]

    print("Thread {}".format(threading.current_thread().name))
    

    for i in range(MAX_COUNT//THREADS):
        #preparation of values
        states[id] = True
        otherProcess=1
        if id == 0:
            turn = 1
            otherProcess = 1
        elif id == 1:
            turn = 0
            otherProcess = 0
        #Processing access to sc, if the other proccess has turn and wants to acces, wait
        while states[otherProcess] == True and turn == otherProcess:
            pass
        
        #sc
        counter += 1
        #end sc
        states[id] = False

def main():
    threads = []

    for i in range(THREADS):
        # Create new threads
        t = threading.Thread(target=thread, args= (i,))
        threads.append(t)
        

        t.start() # start the thread	
        

    # Wait for all threads to complete
    for t in threads:
        t.join()

    print("Counter value: {} Expected: {}\n".format(counter, MAX_COUNT))

if __name__ == "__main__":
    main()