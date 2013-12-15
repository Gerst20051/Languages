{
	i=4
	if ($0 ~ /Mary/) {
		while (i) {
			print i ":" $0;
			i--;
		}
	}
}
