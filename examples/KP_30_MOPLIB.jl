using MooBase

z1 = [77,94,71,63,96,82,85,75,72,91,99,63,84,87,79,94,90,60,69,62,60,74,62,76,82,88,100,96,83,89]
z2 = [65,90,90,77,95,84,70,94,66,92,74,97,60,60,65,97,93,60,69,74,85,66,100,68,92,84,63,79,72,95]
weigths = [80,87,68,72,66,77,99,85,70,93,98,72,100,89,67,86,91,79,71,99,69,83,76,94,61,88,62,77,64,75]
capacity = 1188

z1,z2,sol_weigths,res = solveKP(KP(z1,z2,weigths,capacity))





# solver = KP_Jorge2010()
# z1,z2,weights,res = solveKP(id, solver)