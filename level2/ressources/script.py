from pwn import *

context.binary = "./level2"

p = process("./level2")

p.sendline(cyclic(200))

p.wait()

core = p.corefile

eip = core.eip

print("Offset =", cyclic_find(eip))