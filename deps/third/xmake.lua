includes("zycore")
includes("zydis")
includes("polyhook_2")
includes("glaze")
includes("imgui")
includes("glad")
includes("raw_pdb")

add_requires("zycore v1.5.0", { debug = is_mode_debug(), configs = {runtimes = get_mode_runtimes()} })
add_requires("zydis v4.1.0", { debug = is_mode_debug(), configs = {runtimes = get_mode_runtimes()} })
add_requires("polyhook_2", { debug = is_mode_debug(), configs = {runtimes = get_mode_runtimes()} })