module main

import rand
import rand.seed
import rand.pcg32


#flag -I.
#include "JoshuaTrees.c"

// TODO: V needs a bindgen tool until translate is perfect, signatures are almost isotonic

// C bindings
struct C.JoshTree {}

struct C.StitchedArray {}

struct C.Tree {}

fn C.new_JoshuaTree() &C.JoshTree
fn C.build(chunksize int) &C.StitchedArray
fn C.dealloc_node(target &C.JoshTree)
fn C.dealloc_cond(target &C.JoshTree, chunksize int)

// fn C.delete_node(target &C.JoshTree)
fn C.get_node(a &C.StitchedArray, index int) &C.JoshTree

// fn C.get_chunk(a &C.StitchedArray, index int) &int
fn C.get(a &C.StitchedArray, index int, size u32) voidptr
fn C.insert(a &C.StitchedArray, index int, data voidptr, size u32)

struct C.route_t {}

fn C.route(root &C.StitchedArray, index int) C.route_t

// static void dealloc_node(Tree *target, int chunksize) {
fn C.dealloc_node(target &C.JoshTree, chunksize int)

// static void dealloc_stitched_array(StitchedArray *target) {
fn C.cond_dealloc_stitched_array(target &C.StitchedArray)

fn C.delete_stitched_array(a &C.StitchedArray, size u32)
// fn C.defrag(a &C.StitchedArray)

// wrapped C bindings
struct JoshuaTree {
	root      &C.StitchedArray
	chunksize int
	size u32
}

fn (jt JoshuaTree) idx(index int, size u32) voidptr {
	return C.get(jt.root, index, size)
}

fn (jt JoshuaTree) ins(index int, data voidptr, size u32) {
	C.insert(jt.root, index, data, size)
}

fn (jt JoshuaTree) idx_node(a &C.StitchedArray, index int, size u32) &C.JoshTree {
	return C.get_node(a, index)
}

// fn (jt JoshuaTree) idx_chunk(a &C.StitchedArray, index int) &int {
//	return C.get_chunk(a, index)
//}

fn (jt JoshuaTree) get_route(index int) C.route_t {
	return C.route(*jt.root, index)
}

fn build_stitched_array(chunksize int) &C.StitchedArray {
	return C.build(chunksize)
}

fn build_jtree(chunksize int, size_t u32) &JoshuaTree {
	return &JoshuaTree{
		root: build_stitched_array(chunksize)
		chunksize: chunksize
		size: size_t
	}
}

// TODO: defrag routine to free unused chunks
fn (jt JoshuaTree) defrag() {
	C.cond_dealloc_stitched_array(jt.root)
}

fn (mut jt JoshuaTree) free() {
	//C.delete_stitched_array(jt.root, jt.chunksize, jt.size )
	C.delete_stitched_array(jt.root, jt.size)
}

struct Mystringtype {
mut:
	data int 
	other_data f32
}

fn gen_test_data() Mystringtype {
	rande := rand.intn(10000000) or { panic('error') }
	store := Mystringtype{
	data: rande
	other_data: rand.f32()
	}
	return store
}

//NOTE: cannot have pointers inside the struct, for some reason V doesnt want anything on the heap so it drops too early
//NOTE: don't touch it it works.
fn main() {
	println('Hello World')

	// TODO: extract to unittest raw bindings
	// josh_tree := C.new_JoshuaTree()
	// josh_tree := C.build(8)
	// C.insert(josh_tree, 0, 1)
	// C.insert(josh_tree, 1, 2)
	// res:=C.get(josh_tree, 0)
	// println(res)
	// second_res := C.get(josh_tree, 1)
	// println(second_res)
	// TODO: end of extraction

	// TODO: native V unittest
	mut josh_tree_v := build_jtree(5, sizeof(Mystringtype))
	for i := 0; i < 10000; i++ {
		// TODO: need a formal generic abstraction so all the memory stuff is abstracted away in the bindgen wrapper
		unsafe {
			store := gen_test_data()
			if i % 10000 == 0 {
				print('store.data:')
				println(store.data)
				print('sizeof(Mystringtype):')
				println(sizeof(Mystringtype))
				print('sizeof(store):')
				println(sizeof(store))
			}

			josh_tree_v.ins(i, voidptr(&store), sizeof(Mystringtype))
		}
		res_v := josh_tree_v.idx(i, sizeof(Mystringtype))
		if res_v != 0 {
			if i % 10000 == 0 {
				println('got a value\n')
				// print the current iteration
				println(i)
				println(res_v)
				unsafe {
					cast_to_string := &Mystringtype(res_v)
					println(cast_to_string)
				}
				println('')
			}
		}
	}

	for i := 0; i < 10000; i++ {
		// get
		println(i)
		res_v := josh_tree_v.idx(i, sizeof(Mystringtype))
		if res_v != 0 {
			println('got a retrieval value')
			// print the current iteration
			println(i)
			println(res_v)
			unsafe {
				cast_to_string := &Mystringtype(res_v)
				println(cast_to_string)
			}
		}
		//	//TODO: dropping references
		//	//if i%1000 == 0{
		//	//}
	}
	println('done')
	// defrag
	josh_tree_v.defrag()
}
