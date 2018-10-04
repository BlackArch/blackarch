import os, requests
from threading import Thread

class MyThread(Thread):
    def __init__(self, package_name):
        Thread.__init__(self)
        self.name = package_name
    def run(self):
        def check(self):
            temp_c = requests.get('https://www.archlinux.org/packages/community/x86_64/'+self.name+'/')
            if temp_c.status_code == 200:
                print('We can remove: '+self.name)
        check(self)

to_check = []

for root, dirs, files in os.walk('.'):
    for dir in dirs:
        to_check.append(dir)

while len(to_check) > 15:
    c0 = MyThread(to_check.pop(0))
    c1 = MyThread(to_check.pop(0))
    c2 = MyThread(to_check.pop(0))
    c3 = MyThread(to_check.pop(0))
    c4 = MyThread(to_check.pop(0))
    c5 = MyThread(to_check.pop(0))
    c6 = MyThread(to_check.pop(0))
    c7 = MyThread(to_check.pop(0))
    c8 = MyThread(to_check.pop(0))
    c9 = MyThread(to_check.pop(0))
    c10 = MyThread(to_check.pop(0))
    c11 = MyThread(to_check.pop(0))
    c12 = MyThread(to_check.pop(0))
    c13 = MyThread(to_check.pop(0))
    c14 = MyThread(to_check.pop(0))
    c15 = MyThread(to_check.pop(0))

    c0.start()
    c1.start()
    c2.start()
    c3.start()
    c4.start()
    c5.start()
    c6.start()
    c7.start()
    c8.start()
    c9.start()
    c10.start()
    c11.start()
    c12.start()
    c13.start()
    c14.start()
    c15.start()

    c0.join()
    c1.join()
    c2.join()
    c3.join()
    c4.join()
    c5.join()
    c6.join()
    c7.join()
    c8.join()
    c9.join()
    c10.join()
    c11.join()
    c12.join()
    c13.join()
    c14.join()
    c15.join()

while len(to_check) > 0:

    c0 = MyThread(to_check.pop(0))

    c0.start()

    c0.join()

print('Done!')
