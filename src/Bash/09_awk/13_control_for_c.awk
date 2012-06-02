{
	if ($0 ~ /Mary/) {
		for (i=0; i<4; i++) {
			if (i==2) continue;
			print i+1 ":" $0;
		}
	}
}
