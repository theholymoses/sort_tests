# --------------------------------------------------------------------------------
src_bubble = algorithm/bubble.nasm
src_shaker = algorithm/shaker.nasm
src_insertion = algorithm/insertion.nasm
src_selection = algorithm/selection.nasm
src_gnome = algorithm/gnome.nasm
src_odd_even = algorithm/odd-even.nasm
src_cycle = algorithm/cycle.nasm
src_sort = $(src_bubble) $(src_shaker) $(src_insertion) $(src_selection) $(src_gnome) $(src_odd_even) $(src_cycle)

obj_bubble = bubble.o
obj_shaker = shaker.o
obj_insertion = insertion.o
obj_selection = selection.o
obj_gnome = gnome.o
obj_odd_even = odd-even.o
obj_cycle = cycle.nasm
obj_sort = $(obj_bubble) $(obj_shaker) $(obj_insertion) $(obj_selection) $(obj_gnome) $(obj_odd_even) $(obj_cycle)

hdr_test = sort_test.hpp
src_test = sort_test.cpp
exe_test = run_sort_test

# --------------------------------------------------------------------------------
f_nasm = -f elf64
f_cpp = -std=c++20 -Wall -Wextra -Wpedantic -Werror -O2

# --------------------------------------------------------------------------------
$(exe_test): $(obj_sort) $(hdr_test) $(src_test)
	g++ $(f_cpp) $(obj_sort) $(src_test) -o $@

# --------------------------------------------------------------------------------
$(obj_bubble): $(src_bubble)
	nasm $(f_nasm) $^ -o $@

$(obj_shaker): $(src_shaker)
	nasm $(f_nasm) $^ -o $@

$(obj_insertion): $(src_insertion)
	nasm $(f_nasm) $^ -o $@

$(obj_selection): $(src_selection)
	nasm $(f_nasm) $^ -o $@

$(obj_gnome): $(src_gnome)
	nasm $(f_nasm) $^ -o $@

$(obj_odd_even): $(src_odd_even)
	nasm $(f_nasm) $^ -o $@

$(obj_cycle): $(src_cycle)
	nasm $(f_nasm) $^ -o $@

# --------------------------------------------------------------------------------
clean:
	rm -f $(obj_sort) $(exe_test)

