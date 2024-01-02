#Q = {start,no_u,cp_u,cmp_u,accept,accept1,accept2,accept3,halt_accept,reject_clear,reject,reject1,reject2,reject3,reject4,reject5}

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
no_u *_ *_ ** reject_clear 

cp_u a_ _a rr cp_u
cp_u b_ _b rr cp_u
cp_u c_ __ rl cmp_u_2nd_to_left
cp_u __ __ ** reject

cmp_u_2nd_to_left _* __ ** reject
cmp_u_2nd_to_left ** ** *l cmp_u_2nd_to_left
cmp_u_2nd_to_left *_ *_ *r cmp_u

cmp_u aa __ rr cmp_u
cmp_u bb __ rr cmp_u
cmp_u __ __ ** accept
cmp_u *_ __ r* reject_clear
cmp_u _* __ ** reject
cmp_u ab __ rr reject_clear
cmp_u ba __ rr reject_clear

accept __ t_ r* accept1
accept1 __ r_ r* accept2
accept2 __ u_ r* accept3
accept3 __ e_ ** halt_accept

accept _* t_ r* accept1
accept1 _* r_ r* accept2
accept2 _* u_ r* accept3
accept3 _* e_ ** halt_accept

reject_clear __ __ ** reject
reject_clear *_ __ r* reject_clear
reject_clear _* __ ** reject
reject_clear ** __ r* reject_clear

reject __ f_ r* reject1
reject1 __ a_ r* reject2
reject2 __ l_ r* reject3
reject3 __ s_ r* reject4
reject4 __ e_ ** reject5




