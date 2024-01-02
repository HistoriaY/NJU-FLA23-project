#Q = {start,mv_test_a,mv_test_b,write_c_to_left,write_c_find_a,write_c_btimes,halt_accept,reject_clear,reject,reject1,reject2,reject3,reject4,reject5,reject6,reject7,reject8,reject9,reject10,reject11,reject12,reject13}

#S = {a,b}

#G = {a,b,c,_,I,l,e,g,n,p,u,t}

#q0 = start

#B = _

#F = {halt_accept}

#N = 2

; the transition functions

start a_ a_ ** mv_test_a
start b_ b_ ** reject_clear
start __ __ ** reject_clear

; mv_test_a move a from 1st to 2nd
mv_test_a a_ _a rr mv_test_a
mv_test_a b_ b_ ** mv_test_b
mv_test_a __ __ ** reject_clear

; mv_test_b move b from 1st to 2nd
mv_test_b b_ _b rr mv_test_b
mv_test_b a_ a_ ** reject_clear
mv_test_b __ __ *l write_c_to_left

; write_c_* write i*j's c
write_c_to_left _b _b *l write_c_to_left
write_c_to_left _a _a *l write_c_to_left
write_c_to_left __ __ *r write_c_find_a
write_c_find_a _a __ *r write_c_btimes
write_c_find_a _b _b l* halt_accept
write_c_btimes _a _a *r write_c_btimes
write_c_btimes _b cb rr write_c_btimes
write_c_btimes __ __ *l write_c_to_left

; reject write Illegal_Input on 1st tape
reject_clear *_ __ r* reject_clear
reject_clear ** _* r* reject_clear
reject_clear __ __ ** reject
reject_clear _* _* ** reject

reject _* I* r* reject1
reject1 _* l* r* reject2
reject2 _* l* r* reject3
reject3 _* e* r* reject4
reject4 _* g* r* reject5
reject5 _* a* r* reject6
reject6 _* l* r* reject7
reject7 _* _* r* reject8
reject8 _* I* r* reject9
reject9 _* n* r* reject10
reject10 _* p* r* reject11
reject11 _* u* r* reject12
reject12 _* t* ** reject13

reject __ I_ r* reject1
reject1 __ l_ r* reject2
reject2 __ l_ r* reject3
reject3 __ e_ r* reject4
reject4 __ g_ r* reject5
reject5 __ a_ r* reject6
reject6 __ l_ r* reject7
reject7 __ __ r* reject8
reject8 __ I_ r* reject9
reject9 __ n_ r* reject10
reject10 __ p_ r* reject11
reject11 __ u_ r* reject12
reject12 __ t_ ** reject13


