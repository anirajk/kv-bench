
main:
	g++ -g driver.cc sqlite-objects.cc -DSHOW_TIME -lsqlite3 -lkyotocabinet -ljemalloc -I google-sparsehash -o benchmark
