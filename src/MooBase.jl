module MooBase

export LAP, LAPsolver, solveLAP, LAP_Przybylski2008

type LAP
    nSize::Cint
    C1::Array{Cint, 1}
    C2::Array{Cint ,1}
    p_z1::Ref{Ptr{Cint}}
    p_z2::Ref{Ptr{Cint}}
    p_solutions::Ref{Ptr{Cint}}
    p_nbsolutions::Ref{Cint}
end
LAP(n::Int, c1::Matrix{Int}, c2::Matrix{Int}) = begin
    if !(size(c1,1) == size(c1, 2) == size(c2,1) == size(c2 , 2) == n)
        error("dimensions incorrectes")
    end
    C1 = convert(Matrix{Cint}, c1)
    C2 = convert(Matrix{Cint}, c2)
    LAP(convert(Cint, n), reshape(C1, size(C1,1)^2), reshape(C2, size(C2,1)^2), Ref{Ptr{Cint}}() , Ref{Ptr{Cint}}(), Ref{Ptr{Cint}}(), Ref{Cint}())
end
LAP(c1::Matrix{Int}, c2::Matrix{Int}) = LAP(size(c1,1), c1::Matrix{Int}, c2::Matrix{Int})



type LAPsolver
    parameters
    solve#::Function(id::LAP) -> ...
end

function solveLAP(id::LAP, solver::LAPsolver)
    solver.solve(id)
end

function LAP_Przybylski2008()::LAPsolver
    if isfile(joinpath(dirname(@__FILE__),"libLAP.so"))
        cd(() -> run(`rm libLAP.so`), dirname(@__FILE__)) #for debugging
    end
    if isfile(joinpath(dirname(@__FILE__),"2phrpasf2.o"))
        cd(() -> run(`rm 2phrpasf2.o`), dirname(@__FILE__)) #for debugging
    end
    if !isfile(joinpath(dirname(@__FILE__),"libLAP.so"))
        cd(() -> run(`make`), dirname(@__FILE__))
    end
    mylibvar = joinpath(dirname(@__FILE__),"libLAP.so")
    f = (id::LAP) -> begin 
        @eval ccall(
            (:solve_bilap_exact, $mylibvar),
            Void,
            (Ref{Cint},Ref{Cint}, Cint, Ref{Ptr{Cint}}, Ref{Ptr{Cint}}, Ref{Ptr{Cint}}, Ref{Cint}),
            $id.C1, $id.C2, $id.nSize, $id.p_z1, $id.p_z2, $id.p_solutions, $id.p_nbsolutions)
        nbSol = id.p_nbsolutions.x
        z1,z2 = convert(Array{Int,1},unsafe_wrap(Array, id.p_z1.x, nbSol, true)), convert(Array{Int,1},unsafe_wrap(Array, id.p_z2.x, nbSol, true))
        solutions = convert(Array{Int,2},reshape(unsafe_wrap(Array, id.p_solutions.x, nbSol*id.nSize, true), (id.nSize, nbSol)))
        return z1, z2, solutions'
    end
    return LAPsolver(nothing, f)
end


end # module
