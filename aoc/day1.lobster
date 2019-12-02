import std

def iterate_to_zero(n, fun):
    while n > 0: n = fun(n)

def collect_to_zero(n, fun):
    let ns = []
    iterate_to_zero(n):
        ns.push(_)
        fun(_)
    return ns

// // 100756 and its fuel is:
// // 33583 + 11192 + 3728 + 1240 + 411 + 135 + 43 + 12 + 2 = 50346.
// let modules = [100756]

let input = read_file("aoc/day1_input")
assert input
let tokens = tokenize(input, "\r\n", " ")
let modules = map(tokens) token: string_to_int(token)

var module_fuels = map(modules) n: n / 3 - 2
print("part 1: " + sum(module_fuels))

module_fuels = map(modules) module: sum(collect_to_zero(module / 3 - 2) f: f / 3 - 2)
print("part 2: " + sum(module_fuels))
