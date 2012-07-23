
main:
	g++ -g driver.cc sqlite-objects.cc -DSHOW_TIME -lsqlite3 -lkyotocabinet -I google-sparsehash -o benchmark
