module MooBase

export LAP, LAPsolver, solveLAP, LAP_Przybylski2008

type LAP
    nAssignments::Cint
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
    solve
end

function solveLAP(id::LAP, solver::LAPsolver)
    solver.solve(id)
end

function LAP_Przybylski2008()::LAPsolver
    if isfile(joinpath(dirname(@__FILE__),"libLAP.so"))
        println("ok")
    else
        println("nok")
    end
    mylibvar = joinpath(dirname(@__FILE__),"libLAP.so")
    f = (id::LAP) -> @eval ccall( (:solve_bilap_exact, $mylibvar), Void, (Ref{Cint},Ref{Cint}, Cint), $id.C1, $id.C2, $id.nAssignments)
    return LAPsolver(nothing, f)
end


end # module
