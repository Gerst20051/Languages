@echo off & setLocal EnableDelayedExpansion
for /f "skip=10 tokens=* delims= " %%a in ('dir/b/o-d *.mdb') do (
del %%a
)