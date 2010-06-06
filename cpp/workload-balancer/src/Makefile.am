bin_PROGRAMS = test_balance test_concurrent
test_concurrent_LDFLAGS = -lboost_thread-mt
test_concurrent_CXXFLAGS = -Wall
test_concurrent_SOURCES = \
	simulated.hpp \
	concurrent_runner.hpp concurrent_runner.cpp \
	main_concurrent.cpp

test_balance_LDFLAGS = -lboost_thread-mt
test_balance_CXXFLAGS = -Wall
test_balance_SOURCES = main.cpp \
	simulated.hpp \
	balanced_worker.hpp balanced_worker.cpp \
	balancer.hpp balancer.cpp \
	balancer_types.hpp \
	balance_method.hpp balance_method.cpp