#Q = {start,no_u,cp_u,cmp_u,2nd_to_left,accept,accept1,accept2,accept3,halt_accept,reject,reject1,reject2,reject3,reject4,reject5,reject6}

#S = {a,b,c}

#G = {a,b,c,_,t,r,u,e,f,l,s}

#q0 = start

#B = _

#F = {halt_accept}

#N = 2

; the transition functions
start __ __ ** reject
start c_ __ r* no_u
start a_ a_ ** cp_u
start b_ b_ ** cp_u 

no_u __ __ ** accept
no_u *_ __ r* reject 

cp_u a_ _a rr cp_u
cp_u b_ _b rr cp_u
cp_u c_ __ rl 2nd_to_left
cp_u __ __ ** reject

2nd_to_left _* _* ** reject
2nd_to_left ** ** *l 2nd_to_left
2nd_to_left *_ *_ *r cmp_u

cmp_u aa __ rr cmp_u
cmp_u bb __ rr cmp_u
cmp_u __ __ ** accept
cmp_u *_ __ r* reject
cmp_u _* __ *r reject
cmp_u ab __ rr reject
cmp_u ba __ rr reject
cmp_u c* __ rr reject

accept __ t_ r* accept1
accept1 __ r_ r* accept2
accept2 __ u_ r* accept3
accept3 __ e_ ** halt_accept

reject ** __ rr reject
reject *_ __ r* reject
reject _* __ *r reject
reject __ __ ** reject1

reject1 __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ ** reject6






