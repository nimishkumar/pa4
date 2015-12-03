(*(Nil=Nil)*)
(*let x=x in x+4*)
(*let y = readInt in lambda y.y*)
(*#("cat"@2@3)*)
(*lambda x.x@4*)
(*let list=1@2@3@4 in list*)
(*if "cat"<>"dog" then 1@1 else 2@2*)
(*(lambda x.(x+(lambda x.x+5 6)) 3)*)
(*fun pi with x,y = x+y+2 in 
fun pi2 with x,y = x+y+4 in
(pi (pi2 1 2) (pi2 3 4))*)
(*lambda x.(lambda y.(x+y+5))*)

(*fun foo with x = let y = readString+x in let z = x+5 in print 0 in
print "hi"*)
(*fun foo with x = x+2 in print foo*)
(*fun foo with x,y = x+y+5 in (foo 5 5)*)
(*(lambda x.x+1 5)*)
(*(lambda x.x lambda x.x lambda x.x lambda x.x 5)*)
(lambda x. (lambda y . x+y 3) 5)
