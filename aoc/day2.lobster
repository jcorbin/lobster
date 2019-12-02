import std

def run_intcode(vm) -> string?:
    var ip = 0
    forever():
        switch vm[ip]:
            case 99: return nil
            case 1: vm[vm[++ip]] = vm[vm[++ip]] + vm[vm[++ip]]
            case 2: vm[vm[++ip]] = vm[vm[++ip]] * vm[vm[++ip]]
            default: return "invalid opcode " + vm[ip] + " @" + ip
        ip++
    return nil

//// test

// 1,0,0,0,99 becomes 2,0,0,0,99 (1 + 1 = 2).
// 2,3,0,3,99 becomes 2,3,0,6,99 (3 * 2 = 6).
// 2,4,4,5,99,0 becomes 2,4,4,5,99,9801 (99 * 99 = 9801).
// 1,1,1,4,99,5,6,0,99 becomes 30,1,1,4,2,5,6,0,99.

// let vm = [1,1,1,4,99,5,6,0,99]
// // 30,1,1,4,2,5,6,0,99
// print(vm)
// fatal(run_intcode(vm))
// print(vm)

//// part 1

let input = read_file("aoc/day2_input")
assert input
let tokens = tokenize(input, ",", "")
let ins = map(tokens) token: string_to_int(token)

def run(noun, verb):
    let vm = copy(ins)
    vm[1] = noun
    vm[2] = verb
    fatal(run_intcode(vm))
    return vm[0]

print("part 1: " + run(12, 2))

//// part 2

let goal = 19690720
for(100) noun: for(100) verb:
    if run(noun, verb) == goal:
        print("part 2: " + (100 * noun + verb))
        return
