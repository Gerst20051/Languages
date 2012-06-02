
all: netcat realpath

clean:
	rm -f helpers/realpath
	rm -rf helpers/netcat-hack
	rm -rf /tmp/netcat

netcat:
	rm -rf helpers/netcat-hack
	mkdir helpers/netcat-hack
	mkdir -p /usr/local/bin
	mkdir -p /usr/local/man/man1
	cd helpers/netcat-hack; \
		curl 'http://www.opensource.apple.com/tarballs/netcat/netcat-7.tar.gz' | tar -xzf - ; \
		cd netcat-7; \
		patch -p0 < ../../netcat.patch ; \
		make ;

realpath:
	cd helpers ; cc realpath.c -o realpath

install:
	sudo cp /tmp/netcat/Build/nc /usr/local/bin
	sudo cp /tmp/netcat/Build/nc.1.gz /usr/local/man/man1
	sudo cp helpers/realpath /usr/local/bin

