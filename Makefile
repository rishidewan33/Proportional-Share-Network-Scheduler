LABDEFS =

C++ = g++
CC = g++
AR = ar
ARFLAGS = ru
RANLIB = ranlib


COMMONSRCS = BufSchedOutputStream.cc ScheduledInputStream.cc InputStream.cc ScheduledOutputStream.cc OutputStream.cc sthread.cc Stats.cc util.cc NWScheduler.cc Flag.cc AlarmThread.h AlarmThread.cc STFQNWScheduler.cc MaxNWScheduler.cc

COMMONOBJS := $(COMMONSRCS:.cc=.o) SocketLibrary/libsock.a

OTHERSRCS = sender.cc receiver.cc unit.cc common.c sendAndRecv.cc

SRCS = $(COMMONSRCS) $(OTHERSRCS)

LIBS = -L./SocketLibrary/ -lpthread -lrt -lsock

CFLAGS = -g -Wall -D_POSIX_THREAD_SEMANTICS -Werror $(LABDEFS)

all: SocketLibrary/libsock.a sender receiver unit sendAndRecv


%.o: %.cc
	$(C++) $(CFLAGS) $< -c -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

#
# Etags is really useful for navigating source code 
# from emacs. Try it out.
#
etags:
	etags *.cc *.c SocketLibrary/*.c

SocketLibrary/libsock.a:
	cd SocketLibrary && make

unit: unit.o common.o $(COMMONOBJS)
	$(CC) -o unit unit.o common.o $(COMMONOBJS) $(LIBS) 

sendAndRecv: sendAndRecv.o common.o $(COMMONOBJS)
	$(CC) -o sendAndRecv sendAndRecv.o common.o $(COMMONOBJS) $(LIBS) 

sender: sender.o common.o $(COMMONOBJS)
	$(CC) -o sender sender.o common.o $(COMMONOBJS) $(LIBS) 

receiver: receiver.o common.o $(COMMONOBJS)
	$(CC) -o receiver receiver.o common.o $(COMMONOBJS) $(LIBS) 

test: test.o
	$(CC) -o test test.o common.o $(COMMONOBJS) $(LIBS)

simpleGraph: sender receiver example.gnuplot
	receiver 5000 & > /dev/null; sleep 1; sender 127.0.0.1 5000 2 | gawk -f diff.awk > tmp.dat; gnuplot example.gnuplot; echo "Done. Graph is in tmp.ps. Note that the receiver is still running. You may want to kill it."




#We changed the numbers of streams in our unit.cc file (line 242) to test which number of streams. 

#We also modified the die time in util.cc (line 34) to be able to check 
#how long a non-zero,non-negative flow would last.

#Plots that tests part 2: 1, 2, 3, 4, 6
#Plots that tests part 3: 5, 7, 8

plotTest1.ps: unit plotTest1.gnuplot #RUN THIS ONE WITH nflows = 1 and dieTime = 30
	unit 600000 60000  > dataTest1; gnuplot plotTest1.gnuplot; echo "Done. Graph is in plotTest1.ps."

plotTest2.ps: unit plotTest2.gnuplot #RUN THIS ONE WITH nflows = 2 and dieTime = 30
	unit 600000 60000  > dataTest2; gnuplot plotTest2.gnuplot; echo "Done. Graph is in plotTest2.ps."

plotTest3.ps: unit plotTest3.gnuplot #RUN THIS ONE WITH nflows = 5 and dieTime = 60
	unit 600000 60000  > dataTest3; gnuplot plotTest3.gnuplot; echo "Done. Graph is in plotTest3.ps."

plotTest4.ps: unit plotTest4.gnuplot #RUN THIS ONE WITH nflows = 4 and dieTime = 20
	unit 400000 100000 > dataTest4; gnuplot plotTest4.gnuplot; echo "Done. Graph is in plotTest4.ps."

plotTest5.ps: unit plotTest5.gnuplot #RUN THIS ONE WITH nflows = 2 and dieTime = 30
	unit 600000 60000 stfq > dataTest5; gnuplot plotTest5.gnuplot; echo "Done. Graph is in plotTest5.ps."

plotTest6.ps: unit plotTest6.gnuplot #RUN THIS ONE WITH nflows = 3 and dieTime = 30
	unit 1500000 250000 > dataTest6; gnuplot plotTest6.gnuplot; echo "Done. Graph is in plotTest6.ps."

plotTest7.ps: unit plotTest7.gnuplot #RUN THIS ONE WITH nflows = 6 and dieTime = 40
	unit 1500000 250000 stfq > dataTest7; gnuplot plotTest7.gnuplot; echo "Done. Graph is in plotTest7.ps."

plotTest8.ps: unit plotTest8.gnuplot #RUN THIS ONE WITH nflows = 4 and dieTime = 60
	unit 13500000 950000 stfq > dataTest8; gnuplot plotTest8.gnuplot; echo "Done. Graph is in plotTest8.ps."

plot1.ps: sendAndRecv plot1.gnuplot
		sendAndRecv  > data1; gnuplot plot1.gnuplot; echo "Done. Graph is in plot1.ps."

plot2.ps: sendAndRecv plot2.gnuplot
		sendAndRecv 100000000 1000000 > data2; gnuplot plot2.gnuplot; echo "Done. Graph is in plot2.ps."

plot3.ps: sendAndRecv plot3.gnuplot
		sendAndRecv 100000000 1000000 stfq > data3; gnuplot plot3.gnuplot; echo "Done. Graph is in plot3.ps."

plot1b.ps: sendAndRecv plot1.gnuplot
		sendAndRecv 2000000000 > data1b; gnuplot plot1b.gnuplot; echo "Done. Graph is in plot1b.ps."

plot2b.ps: sendAndRecv plot2b.gnuplot
		sendAndRecv 5000000 1000000 > data2b; gnuplot plot2b.gnuplot; echo "Done. Graph is in plot2b.ps."

plot3b.ps: sendAndRecv plot3b.gnuplot
		sendAndRecv 5000000 1000000 stfq > data3b; gnuplot plot3b.gnuplot; echo "Done. Graph is in plot3b.ps."

plots: plot1.pdf plot2.pdf plot3.pdf plot1b.pdf plot2b.pdf plot3b.pdf #plotTest1.pdf plotTest2.pdf plotTest3.pdf plotTest4 plotTest5.pdf plotTest6


%.pdf: %.ps
	ps2pdf $< 

clean:
	/bin/rm -f sender receiver unit *.o core *~ TAGS tmp.dat tmp.ps *.log *.aux
	cd SocketLibrary && make clean

realclean: clean
	/bin/rm -f *.pdf *ps data1 data2 data3 data1b data2b data3b #dataTest1 dataTest2 dataTest3 dataTest4 dataTest5


#
# You should change this rule if you use a program
# other than latex for your report.
#
report.pdf: report.tex #plot1.pdf plot2.pdf plot3.pdf plot1b.pdf plot2b.pdf plot3b.pdf
	pdflatex report.tex

report: report.pdf


TURNIN := /lusr/bin/turnin
GRADER := impjdi
LAB_NAME := handin-372-labT
handin: handin.tar
	echo "Turning in handin.tar containing the following files:"
	tar tf handin.tar
	$(TURNIN) --submit $(GRADER) $(LAB_NAME) handin.tar


handin.tar: clean
	tar cf handin.tar `find . -type f | grep -v '^\.*$$' | grep -v '/CVS/' | grep -v '/\.svn/' | grep -v '/\.git/' | grep -v 'lab[0-9].*\.tar\.gz' | grep -v '/\~/' | grep -v 'labThread.html' | grep -v '/\.tar/'` 



