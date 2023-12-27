# experimental scripts for the MAPF benchmark

import CSV
import Dates
import Base.Threads
import YAML
import Glob: glob
using DataFrames
using Query
import Statistics: mean, median

# round
r = (x) -> round(x, digits = 3)

function main(config_files...)
    # load experimental setting
    config = merge(map(YAML.load_file, config_files)...)
    exec_file = get(config, "exec_file", joinpath(@__DIR__, "..", "build", "main"))
    seed_start = get(config, "seed_start", 1)
    seed_end = get(config, "seed_end", seed_start)
    time_limit_sec = get(config, "time_limit_sec", 10)
    time_limit_sec_force = get(config, "time_limit_sec_force", time_limit_sec + 30)
    scen = get(config, "scen", "scen-random")
    num_min_agents = get(config, "num_min_agents", 10)
    num_max_agents = get(config, "num_max_agents", 1000)
    num_interval_agents = get(config, "num_interval_agents", 10)
    solver_name = get(config, "solver_name", "unnamed")
    solver_options = get(config, "solver_options", [])
    maps = get(config, "maps", Vector{String}())
    date_str = replace(string(Dates.now()), ":" => "-")
    root_dir = joinpath(get(config, "root", joinpath(pwd(), "..", "data", "exp")), date_str)
    !isdir(root_dir) && mkpath(root_dir)

    # save configuration file
    io = IOBuffer()
    versioninfo(io, verbose = true)
    additional_info = Dict(
        "git_hash" => read(`git log -1 --pretty=format:"%H"`, String),
        "date" => date_str,
        "nthreads" => Threads.nthreads(),
        "env" => String(take!(io)),
    )
    YAML.write_file(joinpath(root_dir, "config.yaml"), merge(config, additional_info))

    # generate iterators
    loops =
        readdir(joinpath(@__DIR__, "scen", scen)) |>
        @filter(x -> !isnothing(match(r".scen$", x))) |>
        @map(x -> joinpath(@__DIR__, "scen", scen, x)) |>
        @filter(
            x -> last(split(match(r"\d+\t(.+).map\t.+", readlines(x)[2])[1], "/")) in maps
        ) |>
        @map(
            x -> begin
                lines = readlines(x)
                N_max = min(length(lines) - 1, num_max_agents)
                map_name = joinpath(
                    @__DIR__,
                    "map",
                    last(split(match(r"\d+\t(.+).map\t(.+)", lines[2])[1], "/")) * ".map",
                )
                agents = collect(num_min_agents:num_interval_agents:N_max)
                (isempty(agents) || last(agents) != N_max) && push!(agents, N_max)
                vcat(
                    Iterators.product(
                        [x],
                        [map_name],
                        agents,
                        collect(seed_start:seed_end),
                    )...,
                )
            end
        ) |>
        collect |>
        x -> vcat(x...) |> x -> enumerate(x) |> collect

    # prepare tmp directory
    tmp_dir = joinpath(@__DIR__, "tmp")
    !isdir(tmp_dir) && mkpath(tmp_dir)

    # main loop
    num_total_tasks = length(loops)
    cnt_fin = Threads.Atomic{Int}(0)
    cnt_solved = Threads.Atomic{Int}(0)
    result = Vector{Any}(undef, num_total_tasks)
    t_start = Base.time_ns()

    Threads.@threads for (k, (scen_file, map_file, N, seed)) in loops
        output_file = joinpath(tmp_dir, "result-$(k).txt")
        command = [
            "timeout",
            "$(time_limit_sec_force)s",
            exec_file,
            "-m",
            map_file,
            "-i",
            scen_file,
            "-N",
            N,
            "-o",
            output_file,
            "-t",
            time_limit_sec,
            "-s",
            seed,
            "-l",
            solver_options...,
        ]
        try
            run(pipeline(`$command`))
        catch e
            nothing
        end

        # store results
        row = Dict(
            :solver => solver_name,
            :num_agents => N,
            :map_name => last(split(map_file, "/")),
            :scen => last(split(scen_file, "/")),
            :seed => seed,
            :solved => 0,
            :comp_time => 0,
            :soc => 0,
            :soc_lb => 0,
            :makespan => 0,
            :makespan_lb => 0,
            :sum_of_loss => 0,
            :sum_of_loss_lb => 0,
            :checkpoints => "",
            :comp_time_initial_solution => 0,
            :cost_initial_solution => 0,
            :search_iteration => 0,
            :num_high_level_node => 0,
            :num_low_level_node => 0,
        )
        if isfile(output_file)
            for line in readlines(output_file)
                m = match(r"soc=(\d+)", line)
                !isnothing(m) && (row[:soc] = parse(Int, m[1]))
                m = match(r"soc_lb=(\d+)", line)
                !isnothing(m) && (row[:soc_lb] = parse(Int, m[1]))
                m = match(r"makespan=(\d+)", line)
                !isnothing(m) && (row[:makespan] = parse(Int, m[1]))
                m = match(r"makespan_lb=(\d+)", line)
                !isnothing(m) && (row[:makespan_lb] = parse(Int, m[1]))
                m = match(r"sum_of_loss=(\d+)", line)
                !isnothing(m) && (row[:sum_of_loss] = parse(Int, m[1]))
                m = match(r"sum_of_loss_lb=(\d+)", line)
                !isnothing(m) && (row[:sum_of_loss_lb] = parse(Int, m[1]))
                m = match(r"comp_time=(\d+)", line)
                !isnothing(m) && (row[:comp_time] = parse(Int, m[1]))
                m = match(r"solved=(\d+)", line)
                if !isnothing(m)
                    row[:solved] = parse(Int, m[1])
                    (row[:solved] == 1) && (Threads.atomic_add!(cnt_solved, 1))
                end
                m = match(r"checkpoints=(.+)", line)
                !isnothing(m) && (row[:checkpoints] = m[1])
                m = match(r"comp_time_initial_solution=(\d+)", line)
                !isnothing(m) && (row[:comp_time_initial_solution] = parse(Int, m[1]))
                m = match(r"cost_initial_solution=(\d+)", line)
                !isnothing(m) && (row[:cost_initial_solution] = parse(Int, m[1]))
                m = match(r"search_iteration=(\d+)", line)
                !isnothing(m) && (row[:search_iteration] = parse(Int, m[1]))
                m = match(r"num_high_level_node=(\d+)", line)
                !isnothing(m) && (row[:num_high_level_node] = parse(Int, m[1]))
                m = match(r"num_low_level_node=(\d+)", line)
                !isnothing(m) && (row[:num_low_level_node] = parse(Int, m[1]))
            end
            rm(output_file)
        end
        result[k] = NamedTuple{Tuple(keys(row))}(values(row))
        Threads.atomic_add!(cnt_fin, 1)
        print(
            "\r" *
            "$(r((Base.time_ns() - t_start) / 1.0e9)) sec" *
            "\t$(cnt_fin[])/$(num_total_tasks) " *
            "($(r(cnt_fin[]/num_total_tasks*100))%)" *
            " tasks have been finished, " *
            "solved: $(cnt_solved[])/$(cnt_fin[]) ($(r(cnt_solved[]/cnt_fin[]*100))%)",
        )
    end

    # save result
    result_file = joinpath(root_dir, "result.csv")
    CSV.write(result_file, result)
    println()
    rm(tmp_dir; recursive = true)
end
