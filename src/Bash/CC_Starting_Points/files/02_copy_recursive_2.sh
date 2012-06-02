
# This preserves the source directory structure.
# For example, this results in destination/file1,
# destination/dir2/file2, and so on.

tar -czf - file1 dir2/file2 dir3/file3 | \
    { cd destination ; tar -xzf - ; }
