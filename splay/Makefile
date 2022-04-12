CXX = g++
CXXFLAGS = -g 
SRCM = splay.cpp test-utils.cpp
OBJM = $(SRCM:.cpp=.o)
LINKFLAGS = -lcppunit
SRCTEST = test-splay.cpp
OBJTEST= $(SRCTEST:.cpp=.o)


# run cppunit test. if that exits successfully, 
# then run valgrind memory leak test 
all: test memcheck

test: test-splay $(OBJM)
	# run cppunit test 
	./test-splay.test

# check for memory leaks with valgrind 
memcheck: test-splay $(OBJM)
	@((valgrind --leak-check=full -q --error-exitcode=1 ./test-splay.test	\
		> /dev/null 2> /dev/null) \
		&& echo "OK: No memory leaks detected.") \
		   || echo "ERROR: Memory leak detected!"


# if client program fails and valgrind gets error -> exit code = 1
# if client program fails and valgrind gets no error -> extit code = 1
# --> we want to report no memory leaks in above case. 
#     temp fix: only run memcheck if test succeeds 
# if client program succeeds and valgrind gets error -> exit code = 1
# if client program succeeds and valgrind gets no error -> exit code = 0
vmemcheck: test-splay $(OBJM)
	(valgrind --leak-check=full -q --error-exitcode=1 ./test-splay.test \
		&& echo "OK: No memory leaks detected.") \
		   || echo "ERROR: Memory leak detected!"

test-splay: $(OBJM) $(SRCTEST)
	$(CXX) $(CXXFLAGS) -o $@.test $(SRCTEST) $(OBJM) $(LINKFLAGS)

# just compile all the cpp files 
compile: $(OBJM) $(OBJTEST)

splay.o : splay.cpp splay.h
test-utils.o : test-utils.h splay.h

# default compile 
# $< evaluates to "xxx.cpp"
# and $@ evaluates to "xxx.o"
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean: 
	rm -f *.o
