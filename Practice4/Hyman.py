import threading

states = [False, False]
turn = 0
THREADS = 2
MAX_COUNT = 10000000
counter = 0

def entry_critical_section(i):
    global states
    global turn
    states[i] = True
    while turn != i:
        while states[(i + 1)%2]:
            pass
        turn = i

def critical_section(i):
    global counter
    counter += 1
    print("Counter value: {} id: {}" .format(counter, i))

def exit_critical_section(i):
    global states
    states[i] = False

def thread(i):
    for j in range(MAX_COUNT//THREADS):
        entry_critical_section(i)
        critical_section(i)
        exit_critical_section(i)

def main():
    threads = []
    for i in range(THREADS):
        #create new thead
        t = threading.Thread(target=thread, args=(i,))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()
    print("Counter value: {} Expected: {}\n" .format(counter, MAX_COUNT))

if __name__ == "__main__":
    main()
