open Printf;;

type particle = {mutable weight:float; mutable index:int};;

let init_array n conc i = Array.init n (fun k-> {weight = conc; index = i});;

let total_weight a = Array.fold_left (fun sum p -> sum +. p.weight) 0.0 a;;

let sum_max_weight a  = Array.fold_left 
			(fun (sum, max) p -> 
				let new_sum = sum +. p.weight in
				if p.weight > max then (new_sum, p.weight)
					else (new_sum, max))
			(0.0,0.0) a;;

let min_wt a b = if (a.weight < b.weight) then a.weight else b.weight;;

let max_wt a b = if (a.weight > b.weight) then a.weight else b.weight;;

let make_cluster a b mwt = 
	let sum_index = a.index+b.index in
	a.weight <- a.weight -. mwt;
	b.weight <- b.weight -. mwt;
	if (Random.float 1.0 < mwt /. a.weight) then (a.index <- sum_index;) else ();
	if (Random.float 1.0 < mwt /. b.weight) then (b.index <- sum_index;) else ();;

let collision a n dt old_nc = 
	let sum_max = sum_max_weight a in
	let max_weight = snd sum_max in
	let p_all = (fst sum_max)*.dt in
	let nc = ref (old_nc +. (Array.fold_left (fun sum p -> sum +. p.weight *. (1.0 -. exp(p.weight *.dt -. p_all))) 0.0 a)) in
	while !nc > 0.0 do
		let p = Random.int n in
		let q = ((Random.int (n-1)) + p + 1) mod n in
		if (p = q) then printf "fuck";
		let ap = a.(p) in
		let aq = a.(q) in
		let mwt = min_wt ap aq in
		let mxt = max_wt ap aq in
		let prob = mxt/. max_weight in
		if ((Random.float 1.0) < prob)	then (
				nc := !nc -. 2.0 *. mwt;
				make_cluster ap aq (0.5*.mwt); 
		)
	done;
	!nc;;

let np = 10000;;
let conc = 1.0 /. (float_of_int np);;
let p = init_array np conc 1;;
let step = ref 0;;
let nc = ref 0.0;;
let dt = 0.01;;

while !step < 5000 do
	nc := collision p np dt !nc;
	printf "%i; %f; %f;\n" !step ((float_of_int !step)*.dt) (total_weight p);
	step := !step+1;
done;;

