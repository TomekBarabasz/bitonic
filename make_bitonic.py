import sys,re

# sorting networks copied from here
# https://bertdobbelaere.github.io/sorting_networks.html

tuple_rx = re.compile(r'\((\d+),(\d+)\)')

def make_layer(layer,input_size,program_string):
    print(f'{layer=}')
    perm_indices = list(range(input_size))
    sort_direction = ['1']*input_size #[1 is asc 0 is desc ]
    for a,b in layer:
        perm_indices[a] = b
        sort_direction[a] = '1'
        perm_indices[b] = a
        sort_direction[b] = '0'
    pi = ','.join( map(str,reversed(perm_indices)) )
    program_string += f'\tidx  = _mm256_set_epi16({pi});\n'
    program_string += '\tperm = _mm256_permutexvar_epi16(idx,input);\n'
    program_string += '\tpmin = _mm256_min_epi16(perm,input);\n'
    program_string += '\tpmax = _mm256_max_epi16(perm,input);\n'
    program_string += f'\tinput = _mm256_mask_mov_epi16(pmin,0b{"".join(sort_direction)},pmax);\n'
    return program_string

def main(input,input_size,output):
    Layers = []
    program_string = '__m256i bitonic_sort(__m256i input)\n{\n\t__m256i idx, perm, pmin, pmax;\n'
    for line in input:
        layer = eval(line)
        program_string = make_layer( layer, input_size, program_string )
        Layers.append( layer )
    program_string += '\treturn input;\n}'    
    #print(program_string)
    output.write(program_string)

if __name__ == '__main__':
    input_fn = sys.argv[1]
    input_size = int(sys.argv[2])
    output_fn = sys.argv[3]

    main(open(input_fn,'r'),input_size,open(output_fn,'w'))

