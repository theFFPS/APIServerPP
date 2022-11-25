CXX                ?= g++
CXX_OPTS           ?= -Wall -Wextra -Werror -lAPIServer++
all:
	$(CXX) $(CXX_OPTS) APIKey.cc -o APIKey
	$(CXX) $(CXX_OPTS) NoAPIKey.cc -o NoAPIKey
	$(CXX) $(CXX_OPTS) APIFromFilesUsage.cc -o APIFromFilesUsage
	$(CXX) $(CXX_OPTS) WithSpecialKey.cc -o WithSpecialKey
clean:
	rm -rf APIKey NoAPIKey APIFromFilesUsage WithSpecialKey