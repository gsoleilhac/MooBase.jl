module MooBase

export LAP, LAPsolver, solveLAP, LAP_Przybylski2008

const LIBPATH = joinpath(dirname(@__FILE__),"..","deps")

type LAP
    nSize::Cint
    C1::Array{Cint, 1}
    C2::Array{Cint ,1}
end

LAP(n::Int, c1::Matrix{Int}, c2::Matrix{Int}) = begin
    if !(size(c1,1) == size(c1, 2) == size(c2,1) == size(c2 , 2) == n)
        error("dimensions incorrectes")
    end
    C1 = convert(Matrix{Cint}, c1)
    C2 = convert(Matrix{Cint}, c2)
    LAP(convert(Cint, n), reshape(C1, size(C1,1)^2), reshape(C2, size(C2,1)^2))
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
    mylibvar = joinpath(LIBPATH,"libLAP.so")

    f = (id::LAP) -> begin 
        p_z1,p_z2,p_solutions,p_nbsolutions = Ref{Ptr{Cint}}() , Ref{Ptr{Cint}}(), Ref{Ptr{Cint}}(), Ref{Cint}()
        @eval ccall(
            (:solve_bilap_exact, $mylibvar),
            Void,
            (Ref{Cint},Ref{Cint}, Cint, Ref{Ptr{Cint}}, Ref{Ptr{Cint}}, Ref{Ptr{Cint}}, Ref{Cint}),
            $id.C1, $id.C2, $id.nSize, $p_z1, $p_z2, $p_solutions, $p_nbsolutions)
        nbSol = p_nbsolutions.x
        z1,z2 = convert(Array{Int,1},unsafe_wrap(Array, p_z1.x, nbSol, true)), convert(Array{Int,1},unsafe_wrap(Array, p_z2.x, nbSol, true))
        solutions = convert(Array{Int,2},reshape(unsafe_wrap(Array, p_solutions.x, nbSol*id.nSize, true), (id.nSize, nbSol)))
        return z1, z2, solutions'
    end

    return LAPsolver(nothing, f)
end


end # module
