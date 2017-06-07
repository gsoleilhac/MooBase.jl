
type KP
    z1::Vector{Int}
    z2::Vector{Int}
    z3::Vector{Int}
    weigths::Vector{Int}
    capacity::Int
end

KP(z1, z2, weigths, capacity) = begin
    if length(z1) != length(z2)
        error("dimensions incorrectes")
    end
    KP(z1,z2,Int[],weigths,capacity)
end

type KPsolver
    parameters
    solve#::Function(id::KP) -> ...
end

function solveKP(id::KP, solver::KPsolver = KP_Jorge2010())
    z1,z2,solW,res = solver.solve(id)
    printKP(z1,z2,solW,res)
    return z1,z2,solW,res
end

function printKP(z1,z2,solW,res)
    println("\nz1\tz2\t∑wi\tsolution")
    for i = 1:length(z1)
        print("(", z1[i], ",\t", z2[i], ")\t",solW[i], " :\t")
        for j = 1 : size(res,2)
            print(res[i,j] == 1 ? "1" : "0")
        end
        println()
    end
end

function KP_Jorge2010()::KPsolver
    mylibvar = joinpath(LIBPATH,"libKP.so")


    f = (id::KP) -> begin 
        nsize = length(id.z1)
        p_z1,p_z2,p_solW,p_res,p_nbSol = Ref{Ptr{Cint}}(), Ref{Ptr{Cint}}(), Ref{Ptr{Cint}}(), Ref{Ptr{UInt8}}(), Ref{Cint}()
        @eval ccall(
            (:bi2phases, $mylibvar),
            Void,
            (Cint, Cint, Ref{Cint}, Ref{Cint}, Ref{Cint}, Cint, Ref{Ptr{Cint}}, Ref{Ptr{Cint}}, Ref{Ptr{Cint}}, Ref{Ptr{UInt8}}, Ref{Cint}),
            2, $nsize, $(convert(Vector{Cint},id.z1)), $(convert(Vector{Cint},id.z2)), $(convert(Vector{Cint},id.weigths)), $(id.capacity), $p_z1, $p_z2, $p_solW, $p_res, $p_nbSol)

        nbSol = p_nbSol.x

        z1 = convert(Vector{Int}, unsafe_wrap(Array, p_z1.x, nbSol, true))
        z2 = convert(Vector{Int}, unsafe_wrap(Array, p_z2.x, nbSol, true))
        solW = convert(Vector{Int}, unsafe_wrap(Array, p_solW.x, nbSol, true))
        res = reshape(convert(Vector{Bool}, unsafe_wrap(Vector{UInt8}, p_res.x, nbSol*nsize, true)), (nbSol, nsize))

        # z1,z2,solW,res = p_z1,p_z2,p_solW,p_res

        return z1,z2,solW,res
    end

    return KPsolver(nothing, f)
end
