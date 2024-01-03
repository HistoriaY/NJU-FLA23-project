#Q = {start,mv_test_a,mv_test_b,2nd_to_left,write_c_find_a,write_c_btimes,halt_accept,reject,reject1,reject2,reject3,reject4,reject5,reject6,reject7,reject8,reject9,reject10,reject11,reject12,reject13,reject14}

#S = {a,b}

#G = {a,b,c,_,I,l,e,g,n,p,u,t}

#q0 = start

#B = _

#F = {halt_accept}

#N = 2

; the transition functions

start a_ a_ ** mv_test_a
start b_ __ r* reject
start __ __ ** reject

; mv_test_a move a from 1st to 2nd
mv_test_a a_ _a rr mv_test_a
mv_test_a b_ b_ ** mv_test_b
mv_test_a __ __ ** reject

; mv_test_b move b from 1st to 2nd
mv_test_b b_ _b rr mv_test_b
mv_test_b a_ __ r* reject
mv_test_b __ __ *l 2nd_to_left

; write_c_* write i*j's c
2nd_to_left _b _b *l 2nd_to_left
2nd_to_left _a _a *l 2nd_to_left
2nd_to_left __ __ *r write_c_find_a
write_c_find_a _a __ *r write_c_btimes
write_c_find_a _b _b l* halt_accept
write_c_btimes _a _a *r write_c_btimes
write_c_btimes _b cb rr write_c_btimes
write_c_btimes __ __ *l 2nd_to_left

; reject write Illegal_Input on 1st tape
reject *_ __ r* reject
reject __ __ ** reject1

reject1 __ I_ r* reject2
reject2 __ l_ r* reject3
reject3 __ l_ r* reject4
reject4 __ e_ r* reject5
reject5 __ g_ r* reject6
reject6 __ a_ r* reject7
reject7 __ l_ r* reject8
reject8 __ __ r* reject9
reject9 __ I_ r* reject10
reject10 __ n_ r* reject11
reject11 __ p_ r* reject12
reject12 __ u_ r* reject13
reject13 __ t_ ** reject14


